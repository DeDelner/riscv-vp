#include "unreal.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

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
        nlohmann::json json = 
        {
            {"objectPath", "/Game/FirstPersonBP/Maps/FirstPersonExampleMap.FirstPersonExampleMap:PersistentLevel." + component_name},
            {"functionName", function_name},
            {"parameters", {}},
            {"generateTransaction", true}
        };

        for (const auto& [key, value] : parameters) {
            //std::cout << key << " = " << value << "; ";
            json["parameters"] = { { key, value } };
        }

        std::cout << json.dump(4) << std::endl;
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

    
    
/*     if (addr < 1000) {
        // Transmitting...
            component_name += unreal_data->component;
        if (addr >= 100 && addr < 200) { // null terminator
            function_name += unreal_data->component;
        }
        std::cout << "component_name: " << component_name << std::endl;
        std::cout << "function_name: " << function_name << std::endl;
        std::cout << "addr: " << addr << std::endl;
    } else {
        // Done transmitting
        nlohmann::json json = 
        {
            {"objectPath", "/Game/FirstPersonBP/Maps/FirstPersonExampleMap.FirstPersonExampleMap:PersistentLevel." + component_name},
            {"functionName", function_name},
            {"parameters", {
                {"NewIntensity", 10.0}
            }},
            {"generateTransaction", true}
        };

        std::cout << json.dump(4) << std::endl;
        clear_data();
    } */
    
}

void curl() {
    CURL *curl;
    CURLcode res;
    long http_code;

    nlohmann::json json = 
    {
        {"objectPath", "/Game/FirstPersonBP/Maps/FirstPersonExampleMap.FirstPersonExampleMap:PersistentLevel.PointLight_1.LightComponent0"},
        {"functionName", "SetIntensity"},
        {"parameters", {
            {"NewIntensity", 10.0}
        }},
        {"generateTransaction", true}
    };

    curl = curl_easy_init();
    curl_global_init(CURL_GLOBAL_ALL);

    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");

        curl_easy_setopt(curl, CURLOPT_URL, "host.docker.internal:30010/remote/object/call");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.dump().c_str());


        //enable to spit out information for debugging
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); 

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        std::cout << "get http return code" << std::endl;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        std::cout << "http code: " << http_code << std::endl;


        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
}