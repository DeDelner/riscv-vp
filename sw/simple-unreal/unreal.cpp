#include <string>

#include "unreal.hpp"

namespace Unreal {

    uint32_t* volatile unreal_data = (uint32_t * volatile const)(0x73000000);
    
    void send_unreal_data(uint32_t value) {
         *unreal_data = value;
    }

    void execute(UnrealComponent component, UnrealFunction function, const map<uint32_t, uint32_t>& parameters) {
        send_unreal_data(component);
        send_unreal_data(function);
        for (const auto& [key, value] : parameters) {
            send_unreal_data(key);
            send_unreal_data(value);
        }
        send_unreal_data(NULL); // Terminator
    }

}