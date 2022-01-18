#include "unreal.hpp"

#include <cstring>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Unreal::Unreal(sc_module_name) {
	tsock.register_b_transport(this, &Unreal::transport);
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void Unreal::transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
    std::cout << "Hello from Unreal!" << std::endl;

    CURL *curl;
    CURLcode res;
    long http_code;
    std::string readBuffer;

/*     if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "host.docker.internal:30010/remote/info");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); 
        res = curl_easy_perform(curl);


        if (res != CURLE_OK){
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);

        std::cout << readBuffer << std::endl;
    } */
    
    json j = 
    {
        {"objectPath", "/Game/FirstPersonBP/Maps/FirstPersonExampleMap.FirstPersonExampleMap:PersistentLevel.PointLight_1.LightComponent0"},
        {"functionName", "SetIntensity"},
        {"parameters", {
            {"NewIntensity", 10.0}
        }},
        {"generateTransaction", true}
    };

/*     std::string j = R"(
        {
            "objectPath" : "/Game/FirstPersonBP/Maps/FirstPersonExampleMap.FirstPersonExampleMap:PersistentLevel.PointLight_1.LightComponent0",
            "functionName" : "SetIntensity",
            "parameters" : {
                "NewIntensity" : 10.0
            },
            "generateTransaction" : true
        }
        )"; */

    std::cout << j << std::endl;

    curl = curl_easy_init();
    curl_global_init(CURL_GLOBAL_ALL);

    if(curl){

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");

        curl_easy_setopt(curl, CURLOPT_URL, "http://host.docker.internal:30010/remote/object/call");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, j.dump().c_str());


        //enable to spit out information for debugging
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); 

        res = curl_easy_perform(curl);

        if (res != CURLE_OK){
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        std::cout << "get http return code" << std::endl;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        std::cout << "http code: " << http_code << std::endl;


        curl_easy_cleanup(curl);
        curl_global_cleanup();

    }
    
}