#include <string>

#include "unreal.hpp"

namespace Unreal {

    struct temp {
        static const int len = 100;

        char component_name[len];
        char function_name[len];

        char parameter1[len];
        char parameter2[len];
        char parameter3[len];
        char parameter4[len];
        char parameter5[len];

        char value1[len];
        char value2[len];
        char value3[len];
        char value4[len];
        char value5[len];

    };

/*     template<typename T>
    struct UnrealData {
        static const int len = 100;

        enum class Command : uint8_t {
            none = 0,
            CALL,
            PROPERTY,
        } volatile command;

        T json;

        char component_name[len];
        char function_name[len];

        char parameter1[len];
        char parameter2[len];
        char parameter3[len];

        char value1[len];
        char value2[len];
        char value3[len];
    }; */

    char* volatile unreal_data = (char * volatile const)(0x73000000);
    
    void send_unreal_data(string value) {
        for (int i = 0; i < value.length(); i++) {
            *unreal_data = value[i];
        }
        *unreal_data = '\n';
    }

    void execute(string component_name, string function_name, map<string, string> parameters) {
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