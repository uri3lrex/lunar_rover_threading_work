// For keeping track of the capabilities and constraints of all nodes

#pragma once

#include <string>
#include <unordered_map>
#include <queue>
#include <tuple>

using namespace std;

class Node {

    string ip;

    // service, <yes/no, port>
    unordered_map<string, tuple<bool, string>> services = {
        {"drive", tuple(false, "")},
        {"climate", tuple(false, "")},
        {"samples", tuple(false, "")},
        {"picture", tuple(false, "")},
    };

    queue<string> actions;

    const int buf_size = 1024; 
    char *data;
public:

    Node(string ip);
    ~Node();

    string get_ip();

    void update_service(string service, string port);
    tuple<bool, string> get_port(string service);
    void get_data();
    void add_action(string action);
    bool send_actions(int num = 1);

};