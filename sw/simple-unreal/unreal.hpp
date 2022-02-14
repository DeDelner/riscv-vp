#include <variant>
#include <string>
#include <map>

using namespace std;

namespace Unreal {

    void send_unreal_data(std::variant<double, int, string> value);

    void execute(string component_name, string function_name, const map<string, std::variant<double, int, string>>& parameters);

}