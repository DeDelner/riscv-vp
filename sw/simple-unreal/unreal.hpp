#pragma once
#include <variant>
#include <string>
#include <map>

#include "../../env/unreal/unreal_components.hpp"

using namespace std;

namespace Unreal {

    void send_unreal_data(uint32_t value);

    void execute(UnrealComponent component, UnrealFunction function, const map<uint32_t, uint32_t>& parameters);

}