#include "radar.h"
//#include "negotiator.h"
#include "shared/config_reader.h"

#include <string>
#include <unordered_map>

using namespace std;

int main() {

    string network;
    int port = 0;
    string banner;
    unordered_map<string, string> svcs;

    // populate from config file
    //Reader config{"NETWORK", "PORT", "BANNER", "SERVICE: CONSTRAINTS"};
   //config.read("./config.txt", network, port, banner, svcs);

   
    //Radar radar(network, port, banner);
    //radar.findNodes();
    // Temporarily comment out negotiator
Radar radar("127.0.0.1/8", 1111, "lunar rover");
radar.findNodes();

    //Negotiator negotiator(radar.getNodes(), port, svcs);
    //negotiator.negotiate();

    // loop showing rovers last online
    // allow selection of rover
    // allow sending actions to rover and viewing feedback

    return 0;
}