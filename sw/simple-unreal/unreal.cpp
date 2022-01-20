#include <string>
#include "unreal.hpp"

namespace Unreal {

    struct UnrealData {
        static const int len = 100;

        char component_name[len];
        char function_name[len];

        char parameter1[len];
        char parameter2[len];
        char parameter3[len];

        char value1[len];
        char value2[len];
        char value3[len];
    };

    UnrealData* volatile const unreal_data = (UnrealData * volatile const)(0x73000000);

    template<typename T>
    void set_unreal_data(T parameter, string value) {
        for (int i = 0; i < unreal_data->len; i++) {
            if (i < value.length()) {
                parameter[i] = value[i];
            } else {
                parameter[i] = '\0';
            }
        }
    }

    void execute(string component_name, string function_name) {
        set_unreal_data(unreal_data->component_name, component_name);
        set_unreal_data(unreal_data->function_name, function_name);
    }

}