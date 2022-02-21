#pragma once

enum UnrealComponent { VP_API_2 };
enum UnrealFunction { Print_Float, Enable_Electromagnet };

static const char* get_display_name(uint32_t index) {
    const char* unreal_display_name[1] = { "VP_2" };
    return unreal_display_name[index];
}

static const char* get_function_name(uint32_t index) {
    const char* unreal_function_name[2] = { "Print Float", "Enable Electromagnet" };
    return unreal_function_name[index];
}