#pragma once

enum UnrealComponent { VP_API_2 };
enum UnrealFunction { Print_Float, Set_Electromagnet_Power, Get_HallSensor_Value };

static const char* get_display_name(uint32_t index) {
    const char* unreal_display_name[1] = { "VP_2" };
    return unreal_display_name[index];
}

static const char* get_function_name(uint32_t index) {
    const char* unreal_function_name[3] = { "Print Float", "Set Electromagnet Power", "Get Hall Sensor Value" };
    return unreal_function_name[index];
}