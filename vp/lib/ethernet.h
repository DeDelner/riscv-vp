#ifndef RISCV_VP_ETHERNET_H
#define RISCV_VP_ETHERNET_H

#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <ios>
#include <iomanip>
#include <list>
#include <map>
#include <unistd.h>

#include <systemc>

#include <tlm_utils/simple_target_socket.h>

#include "irq_if.h"
#include "tlm_map.h"

struct arp_eth_header {
    uint16_t htype;
    uint16_t ptype;
    uint8_t hlen;
    uint8_t  plen;
    uint16_t oper;
    uint8_t sender_mac[6];
    uint8_t sender_ip[4];
    uint8_t target_mac[6];
    uint8_t target_ip[4];
};

class ArpCache
{
	/* Format for fscanf() to read the 1st, 4th, and 6th space-delimited fields */
	static const char arpLineFormat[];
	static const char arpCachePath[];
	static const uint16_t bufferLength = 1024;

	//maps ip (4Byte) to MAC (6Byte)
	std::map<uint32_t, uint64_t> cache;

	void readKernelArpCache();
public:
	bool getHwidByIp(const uint32_t* ip, uint8_t* hwid);
};

class ArpResponder
{
public:
	static const uint16_t arpPacketSize = 60;
private:
	uint8_t packet[arpPacketSize];
	ArpCache cache;
public:
	bool isArpReq(uint8_t* eth, uint16_t len);
	uint8_t* buildResponseFrom(uint8_t* eth);
};

struct EthernetDevice : public sc_core::sc_module {
    tlm_utils::simple_target_socket<EthernetDevice> tsock;

    interrupt_gateway *plic = 0;
    uint32_t irq_number = 0;
    sc_core::sc_event run_event;

    // memory mapped data frame
    std::array<uint8_t, 64> data_frame;

    // memory mapped configuration registers
    uint32_t status = 0;
    uint32_t receive_size = 25;
    uint32_t receive_dst = 0;
    uint32_t send_src = 0;
    uint32_t send_size = 0;
    uint32_t mac[2];

    uint8_t* VIRTUAL_MAC_ADDRESS = reinterpret_cast<uint8_t*>(mac);
    uint8_t BROADCAST_MAC_ADDRESS[ 6 ] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    uint8_t *mem = nullptr;

    vp::map::LocalRouter router;

    int send_sockfd = 0;
    int recv_sockfd = 0;
    int interfaceIdx = 0;

    static const uint16_t MTU_SIZE   = 1500;
	static const uint16_t FRAME_SIZE = 1514;

    uint8_t recv_frame_buf[FRAME_SIZE];
    bool has_frame;
    ArpResponder arpResponder;

    static const uint16_t STATUS_REG_ADDR       = 0x00;
	static const uint16_t RECEIVE_SIZE_REG_ADDR = STATUS_REG_ADDR       + sizeof(uint32_t);
	static const uint16_t RECEIVE_DST_REG_ADDR  = RECEIVE_SIZE_REG_ADDR + sizeof(uint32_t);
	static const uint16_t SEND_SRC_REG_ADDR     = RECEIVE_DST_REG_ADDR  + sizeof(uint32_t);
	static const uint16_t SEND_SIZE_REG_ADDR    = SEND_SRC_REG_ADDR     + sizeof(uint32_t);
	static const uint16_t MAC_HIGH_REG_ADDR     = SEND_SIZE_REG_ADDR    + sizeof(uint32_t);
	static const uint16_t MAC_LOW_REG_ADDR      = MAC_HIGH_REG_ADDR     + sizeof(uint32_t);

	enum : uint16_t
	{
		RECV_OPERATION = 1,
		SEND_OPERATION = 2,
	};

    SC_HAS_PROCESS(EthernetDevice);

    EthernetDevice(sc_core::sc_module_name, uint32_t irq_number, uint8_t *mem);

    void init_raw_sockets();

    void send_raw_frame();
    bool try_recv_raw_frame();

    void register_access_callback(const vp::map::register_access_t &r) {
        assert (mem);

        if (r.read && r.vptr == &receive_size) {
            assert (has_frame);
        }

        r.fn();

        if (r.write && r.vptr == &status) {
            if (r.nv == RECV_OPERATION) {
                //std::cout << "[ethernet] recv operation" << std::endl;
                assert (has_frame);
                for (int i=0; i<receive_size; ++i) {
                    auto k = receive_dst + i;
                    mem[k - 0x80000000] = recv_frame_buf[i];
                }
                has_frame = false;
            } else if (r.nv == SEND_OPERATION) {
                send_raw_frame();
            } else {
                throw std::runtime_error("unsupported operation");
            }
        }
    }

    void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
        router.transport(trans, delay);
    }

    void run() {
        while (true) {
            run_event.notify(sc_core::sc_time(10000, sc_core::SC_US));
            sc_core::wait(run_event);  // 10000 times per second by default

            // check if data is available on the socket, if yes store it in an internal buffer
            if (!has_frame) {
				while (!try_recv_raw_frame())
					;
                if (has_frame)
                    plic->gateway_incoming_interrupt(irq_number);
            }
        }
    }
};

#endif //RISCV_VP_ETHERNET_H
