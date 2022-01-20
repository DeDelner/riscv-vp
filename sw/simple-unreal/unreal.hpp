#include <string>

using namespace std;

namespace Unreal {

    template<typename T>
    void set_unreal_data(T parameter, string value);

    void execute(string component_name, string function_name);

}