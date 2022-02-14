#include <string>

#include "unreal.hpp"

namespace Unreal {

    char* volatile unreal_data = (char * volatile const)(0x73000000);
    
    void send_unreal_data(std::variant<double, int, string> value) {

        switch (value.index()) {
            case 0:
                *unreal_data = std::get<double>(value);
                break;
            case 1:
                *unreal_data = std::get<int>(value);
                break;
            default:
                for (int i = 0; i < std::get<string>(value).length(); i++) {
                    *unreal_data = std::get<string>(value)[i];
                }
                *unreal_data = '\n';
                break;
        }
    }

    void execute(string component_name, string function_name, const map<string, std::variant<double, int, string>>& parameters) {
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