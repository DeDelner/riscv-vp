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

    const int32_t THRESHOLD = 50;
    const int32_t ELEVATION_THRESHOLD = 55;

    int32_t hallsensor_value = 0;
    int32_t previous_hallsensor_value = 0;

    int32_t velocity = 0;

    // Kp -  proportional gain
    // Ki -  Integral gain
    // Kd -  derivative gain
    // dt -  loop interval time
    // max - maximum value of manipulated variable
    // min - minimum value of manipulated variable
    double dt = 0.0000001;
    double max = 1.0;
    double min = -1.0;
    double Kp = 0.04;
    double Kd = 1;
    double Ki = 1;
    PID pid = PID(dt, max, min, Kp, Kd, Ki);

    int setpoint = 30;

    int val = get_hallsensor_value();
    while (true) {
        int inc = (int)pid.calculate(setpoint, get_hallsensor_value());
        std::cout << "val: " << val << ", inc: " << inc << std::endl;
        std::cout << "\033c"; // clear line
        val += inc * inc *inc;

        if (val > 0) {
            set_electromagnet_power(500);
        } else {
            set_electromagnet_power(0);
        }
    }

	return 0;
}