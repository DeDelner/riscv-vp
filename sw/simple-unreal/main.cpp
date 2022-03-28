#include <string>
#include <iostream>
#include <math.h>       /* pow */
#include <iomanip>
#include <cmath>
#include <bit>
#include "unreal.hpp"

using namespace std;

void wait(uint32_t time) {
	for (uint32_t i = 128 * time; i > 0; i--) {}
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

float ieee_float(uint32_t f) {
  return std::bit_cast<float>(f);
}

int main() {

    const int32_t THRESHOLD = 50;
    const int32_t ELEVATION_THRESHOLD = 55;

    int32_t hallsensor_value = 0;
    int32_t previous_hallsensor_value = 0;

    int32_t velocity = 0;

    while (true) {
        velocity = hallsensor_value - previous_hallsensor_value;
        previous_hallsensor_value = hallsensor_value;

        int32_t wait_time = clamp((hallsensor_value - THRESHOLD) * 16, (int32_t)46 - velocity, (int32_t)1000);

        int32_t height = max(hallsensor_value - 28, (int32_t)0);

            set_electromagnet_power(500);
            wait(ELEVATION_THRESHOLD - height - velocity);
            set_electromagnet_power(0);
            //wait(max(velocity * 6, (int32_t)0));

        std::cout << "height: " << height << std::endl;
        std::cout << "\033c"; // clear line


        hallsensor_value = get_hallsensor_value();
    }

	return 0;
}