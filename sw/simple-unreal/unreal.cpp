#include <string>

#include "unreal.hpp"

namespace Unreal {

    char* volatile unreal_data = (char * volatile const)(0x73000000);
    
    void send_unreal_data(string value) {
        for (int i = 0; i < value.length(); i++) {
            *unreal_data = value[i];
        }
        *unreal_data = '\n';
    }

    void execute(string component_name, string function_name, const map<string, string>& parameters) {
        send_unreal_data(component_name);
        send_unreal_data(function_name);
        for (const auto& [key, value] : parameters) {
            send_unreal_data(key);
            *unreal_data = ':';
            send_unreal_data(value);
        }
        *unreal_data = '\0';
    }

}