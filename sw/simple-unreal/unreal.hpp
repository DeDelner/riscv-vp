#include <string>
#include <map>

using namespace std;

namespace Unreal {

    void send_unreal_data(string value);

    void execute(string component_name, string function_name, map<string, string> parameters);

}