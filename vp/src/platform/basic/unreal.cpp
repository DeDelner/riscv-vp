#include "unreal.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "../../../../env/unreal/unreal_settings.h"
#include "../../../../env/unreal/unreal_components.hpp"

using json = nlohmann::json;

enum PHASE { COMPONENT, FUNCTION, PARAM_NAME, PARAM_VALUE, EXECUTE };
PHASE phase;
uint32_t component_name;
uint32_t function_name;
uint32_t param_value;
json response_body;

Unreal::Unreal(sc_module_name) {
    phase = COMPONENT;
	tsock.register_b_transport(this, &Unreal::transport);
}

void clear_data() {
    component_name = 0;
    function_name = 0;
    param_value = 0;
    phase = COMPONENT;
}

size_t writeCallback(char *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void curl(json body) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    curl_global_init(CURL_GLOBAL_ALL);

    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");

        std::string url = host + ":" + port + "/remote/object/call";

        std::string readBuffer;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.dump().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);


        //enable to spit out information for debugging
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); 

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        response_body = json::parse(readBuffer);

        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
}

void Unreal::transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
    tlm::tlm_command cmd = trans.get_command();
	unsigned addr = trans.get_address();
	auto *ptr = trans.get_data_ptr();
	auto len = trans.get_data_length();

    uint32_t value = *reinterpret_cast<uint32_t *>(ptr);

    if (cmd == tlm::TLM_WRITE_COMMAND) {
        switch (phase) {
            case COMPONENT:
                component_name = value;
                phase = FUNCTION;
                break;
            case FUNCTION:
                function_name = value;
                phase = PARAM_VALUE;
                break;
            case PARAM_VALUE:
                param_value = value;
            case EXECUTE:
                json request_body = 
                {
                    {"objectPath", game_path + "." + get_display_name(component_name)},
                    {"functionName", get_function_name(function_name)},
                    {"parameters", {{ "0", param_value }}},
                    {"generateTransaction", true}
                };

                //std::cout << request_body.dump() << std::endl;
                curl(request_body);
                clear_data();
                break;
        }
    } else {
        std::cout << "read detected" << std::endl;
        *((uint32_t *)ptr) = response_body["Return"];
    }
    
}