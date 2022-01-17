#pragma once

#include <tlm_utils/simple_target_socket.h>
#include <systemc>

using namespace std;
using namespace sc_core;
using namespace tlm_utils;

struct Unreal : public sc_core::sc_module {

	simple_target_socket<Unreal> tsock;
	
	void info();

	Unreal(sc_module_name);
	void transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
};
