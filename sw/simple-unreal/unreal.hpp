#pragma once
#include <variant>
#include <string>
#include <map>

#include "../../env/unreal/unreal_components.hpp"

using namespace std;

namespace Unreal {

    void write_unreal_data(uint32_t value);
    uint32_t read_unreal_data();

    void execute(UnrealComponent component, UnrealFunction function, uint32_t param_value);

}