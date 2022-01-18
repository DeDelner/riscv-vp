#include <string>

struct UnrealData {

    static const int len = 100;

    char component[len];
    char function_name[len];

    UnrealData() {

    };
};

UnrealData* volatile const unrealData = (UnrealData * volatile const)(0x73000000);

using namespace std;

template<typename T>
void setUnrealData(T parameter, string value) {
    for (int i = 0; i < unrealData->len; i++) {
	    if (i < value.length()) {
            parameter[i] = value[i];
        } else {
            parameter[i] = '\0';
        }
    }
}

int main() {

    setUnrealData(unrealData->component, "PointLight_1.LightComponent0");
    setUnrealData(unrealData->function_name, "SetIntensity");

	return 0;
}