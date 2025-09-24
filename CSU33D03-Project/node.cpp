#include "node.h"

using namespace std;

Node::Node(string ip) {
    this->ip = ip;

    this->data = new char[this->buf_size]; // allocate for future use
}

Node::~Node() {
    // delete[] data;
}

string Node::get_ip() {
    return ip;
}

void Node::update_service(string service, string port) {
    this->services[service] = tuple(true, port);
}

tuple<bool, string> Node::get_port(string service) {
    return services[service];
}

void Node::get_data() {

}

void Node::add_action(string action) {
    this->actions.push(action);
}

bool Node::send_actions(int num) {
    bool success = true;

    for (int i = 0; i < num; ++i) {
        string action = this->actions.front();

        // send and wait for confirmation





        

        this->actions.pop();
    }

    return success;
}