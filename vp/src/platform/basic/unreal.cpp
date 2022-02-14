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
std::map<std::string, uint32_t> parameters;
PHASE phase;
uint32_t component_name;
uint32_t function_name;
std::string param_name;

Unreal::Unreal(sc_module_name) {
    phase = COMPONENT;
	tsock.register_b_transport(this, &Unreal::transport);
}

void clear_data() {
    // TODO
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

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.dump().c_str());


        //enable to spit out information for debugging
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); 

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

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

    std::stringstream ss;

    switch (phase) {
        case COMPONENT:
            component_name = value;
            phase = FUNCTION;
            break;
        case FUNCTION:
            function_name = value;
            phase = PARAM_NAME;
            break;
        case PARAM_NAME:
            ss << "0x" << std::setfill ('0') << std::setw(2) << std::hex << value;
            param_name = ss.str();
            phase = PARAM_VALUE;
            break;
        case PARAM_VALUE:
            parameters[param_name] = value;
            phase = EXECUTE;
            break;
        case EXECUTE:
            json body = 
            {
                {"objectPath", game_path + "." + get_display_name(component_name)},
                {"functionName", get_function_name(function_name)},
                {"parameters", {}},
                {"generateTransaction", true}
            };

            for (const auto& [key, value] : parameters) {
                body["parameters"] = { { key, value } };
            }

            curl(body);
            std::cout << body.dump() << std::endl;
            clear_data();

            phase = COMPONENT;
            break;
    }
    
}