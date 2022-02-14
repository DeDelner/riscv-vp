#include "unreal.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "../../../../env/unreal/unreal_settings.h"

using json = nlohmann::json;

struct UnrealData {
    static const int len = 100;

    char component[len];
    char function_name[len];

    UnrealData() {

    };
};

std::string component_name = "";
std::string function_name = "";
std::string parameter_name = "";
std::string parameter_value = "";

std::map<std::string, std::string> parameters;

int member_count = 0;
bool parameter = false;

Unreal::Unreal(sc_module_name) {
	tsock.register_b_transport(this, &Unreal::transport);
}

void clear_data() {
    component_name = "";
    function_name = "";
    parameter_name = "";
    parameter_value = "";

    parameters.clear();

    member_count = 0;
    parameter = false;
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

    if (*ptr == '\n') {
        if (parameter) {
            parameters[parameter_name] = parameter_value;
            parameter = false;
        } else {
            member_count++;
        }
    } else if (*ptr == '\0') {

        json body = 
        {
            {"objectPath", game_path + "." + component_name},
            {"functionName", function_name},
            {"parameters", {}},
            {"generateTransaction", true}
        };

        for (const auto& [key, value] : parameters) {
            body["parameters"] = { { key, value } };
        }

        curl(body);
        clear_data();
    } else {
        switch (member_count) {
            case 0:
                component_name += *ptr;
                break;
            case 1:
                function_name += *ptr;
                break;
            default:
                if (*ptr == ':') {
                    parameter = true;
                } else {
                    if (!parameter) {
                        parameter_name += *ptr;
                    } else {
                        parameter_value += *ptr;
                    }
                }
                break;
        }
    }
    
}