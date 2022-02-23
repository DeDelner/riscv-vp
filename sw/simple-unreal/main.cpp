#include <string>
#include <iostream>
#include "unreal.hpp"

using namespace std;

void activeWait(float factor = 0.5) {
	for (uint16_t i = 0; i < UINT16_MAX * factor; i++) {
	};
}

void set_electromagnet_power(uint32_t power) {
    Unreal::execute(
        UnrealComponent::VP_API_2, 
        UnrealFunction::Set_Electromagnet_Power, 
        power
    );
} 

uint32_t get_hallsensor_value() {
    Unreal::execute(
        UnrealComponent::VP_API_2, 
        UnrealFunction::Get_HallSensor_Value, 
        0
    );
    return Unreal::read_unreal_data();
}

int main() {

    uint32_t threshold = 150;
    uint32_t hallsensor_value = 0;

    while (true) {
        hallsensor_value = get_hallsensor_value();
        std::cout << "hallsensor: " << hallsensor_value << std::endl;

        if (hallsensor_value > threshold) {
            set_electromagnet_power(0);
        } else {
            set_electromagnet_power(hallsensor_value);
        }
        
        activeWait(0.01);
    }

	return 0;
}