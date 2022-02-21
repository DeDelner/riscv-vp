#include <string>
#include <iostream>
#include "unreal.hpp"

using namespace std;

void activeWait(float factor = 0.5) {
	for (uint16_t i = 0; i < UINT16_MAX * factor; i++) {
	};
}

int main() {

/*
    while (true) {
        Unreal::execute(
            UnrealComponent::VP_API_2, 
            UnrealFunction::Enable_Electromagnet, 
            1
        );
        activeWait(0.0075);
        Unreal::execute(
            UnrealComponent::VP_API_2, 
            UnrealFunction::Enable_Electromagnet, 
            0
        );
        activeWait(0.0075);
    }
    */

    Unreal::execute(
        UnrealComponent::VP_API_2, 
        UnrealFunction::Enable_Electromagnet, 
        1
    );

    std::cout << "Electromagnet enabled: " << Unreal::read_unreal_data() << std::endl;

	return 0;
}