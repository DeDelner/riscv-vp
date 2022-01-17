#include "unreal.h"

#include <string>
//#include <curl/curl.h>

struct UnrealData {
    char Level;
    char Component;
    char Property;
    char Value;

    UnrealData() {

    };
};

Unreal::Unreal(sc_module_name) {
	tsock.register_b_transport(this, &Unreal::transport);
}

void Unreal::transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
    tlm::tlm_command cmd = trans.get_command();
	unsigned addr = trans.get_address();
	auto *ptr = trans.get_data_ptr();
	auto len = trans.get_data_length();

	assert((addr + len <= sizeof(UnrealData)) && "Access display out of bounds");

    if (cmd == tlm::TLM_WRITE_COMMAND) {    
    }

    info();
}

void Unreal::info() {

    //CURL *curl;
    //CURLcode res;
    //std::string readBuffer;

    //curl = curl_easy_init();
//    if (curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
//        res = curl_easy_perform(curl);
//        curl_easy_cleanup(curl);
//
//        std::cout << readBuffer << std::endl;
//    }

}