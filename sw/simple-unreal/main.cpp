#include <string>
#include "unreal.hpp"

using namespace std;

void activeWait(float factor = 0.5) {
	for (uint16_t i = 0; i < UINT16_MAX * factor; i++) {
	};
}

int main() {

    Unreal::execute(
        "PointLight1.LightComponent0", 
        "SetIntensity", 
        {{"NewIntensity", "0.0"}}
    );

    Unreal::execute(
        "PointLight2.LightComponent0", 
        "SetIntensity", 
        {{"NewIntensity", "0.0"}}
    );

    int i = 0;
    while (i < 50) {
        i++;
        Unreal::execute(
            "PointLight2.LightComponent0", 
            "SetIntensity", 
            {{"NewIntensity", std::to_string(i)}}
        );
        activeWait(0.005);
    }

	return 0;
}