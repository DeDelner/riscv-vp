#include <string>
#include "unreal.hpp"

using namespace std;

int main() {

    map<string, string> parameters { {"NewIntensity", "10.0"} };

    Unreal::execute("PointLight", "SetIntensity", parameters);

	return 0;
}