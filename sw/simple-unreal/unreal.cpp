#include <string>

#include "unreal.hpp"

namespace Unreal {

    uint32_t* volatile unreal_data = (uint32_t * volatile const)(0x73000000);
    
    void write_unreal_data(uint32_t value) {
        *unreal_data = value;
    }

    uint32_t read_unreal_data() {
        return *unreal_data;
    }

    void execute(UnrealComponent component, UnrealFunction function, uint32_t param_value) {
        write_unreal_data(component);
        write_unreal_data(function);
        write_unreal_data(param_value);
    }

}