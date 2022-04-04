#include <string>
#include <iostream>
#include <math.h>       /* pow */
#include <iomanip>
#include <cmath>
#include <bit>
#include <stdio.h>
#include "pid.h"
#include "unreal.hpp"

using namespace std;

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

    // Kp -  proportional gain
    // Ki -  Integral gain
    // Kd -  derivative gain
    // dt -  loop interval time
    // max - maximum value of manipulated variable
    // min - minimum value of manipulated variable
    float dt = 0.01;
    float max = 100.0;
    float min = -100.0;
    float Kp = 0.1;
    float Kd = 0.01;
    float Ki = 0.0;
    PID pid = PID(dt, max, min, Kp, Kd, Ki);

    int32_t setpoint = 50; // desired height, our target

    while (true) {
        int32_t inc = (int32_t)pid.calculate(setpoint, get_hallsensor_value());
        std::cout << "inc: " << inc << std::endl;
        std::cout << "\033c"; // clear line

        if (inc > 0) {
            set_electromagnet_power(500);
        } else {
            set_electromagnet_power(0);
        }
    }

	return 0;
}