// Library to discover viable "lunar rover" nodes on the network

// TODO: 
// findNodes()
// - threading for pings

#pragma once

#include <vector>
#include <string>
#include "node.h"
#include <cstdint>  // for uint8_t, uint16_t

using namespace std;

// ICMP header
struct icmp_header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
};

class Radar {

    string network;
    int port;
    string banner;
    vector<Node> nodes; // container of viable nodes

    uint16_t calculate_checksum(void* buff, int len);

public:
    bool ping(string addr); // check if target is up
    bool checkBanner(string addr); // check if the node is a "lunar rover"

    Radar(string network, int port, string banner);
    void findNodes(); // identify all viable nodes
    vector<Node> getNodes();

};