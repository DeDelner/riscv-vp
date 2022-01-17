#include "unreal.hpp"

Unreal::Unreal(sc_module_name) {
	tsock.register_b_transport(this, &Unreal::transport);
}

void Unreal::transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
    std::cout << "Hello from Unreal!" << std::endl;
}