#include "radar.h"
#include "threadpool.h"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <regex>
#include <mutex>
#include <future>
#include <atomic>
#include <algorithm>

using namespace std;

Radar::Radar(string network, int port, string banner): network(network), port(port), banner(banner) {}

vector<Node> Radar::getNodes() {
    return nodes;
}

uint16_t Radar::calculate_checksum(void* buff, int len) {
    uint32_t sum = 0;
    uint16_t* ptr = reinterpret_cast<uint16_t*>(buff);

    while (len > 1) {
        sum += *ptr++;
        len -= 2;
    }

    if (len == 1) {
        sum += *reinterpret_cast<uint8_t*>(ptr);
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    return static_cast<uint16_t>(~sum);
}

bool Radar::ping(string addr) {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd == -1) {
        perror("socket() failed - try sudo");  // Add error reporting
        return false;
    }

    sockaddr_in target;
    memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    if (inet_pton(AF_INET, addr.c_str(), &target.sin_addr) != 1) {
        cerr << "Invalid address: " << addr << endl;  // Add address validation
        close(sockfd);
        return false;
    }

    struct icmp_header {
        uint8_t type;
        uint8_t code;
        uint16_t checksum;
        uint16_t id;
        uint16_t sequence;
    } icmph;
    
    icmph.type = 8;
    icmph.code = 0;
    icmph.checksum = 0;
    icmph.id = getpid();
    icmph.sequence = 1;
    icmph.checksum = calculate_checksum(&icmph, sizeof(icmph));

    /**timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        close(sockfd);
        return false;
    }

    if (sendto(sockfd, &icmph, sizeof(icmph), 0, (sockaddr*)&target, sizeof(target)) <= 0) {
        close(sockfd);
        return false;
    }*/

    // Set shorter timeout
    timeval timeout{1, 0};  // 1 second
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // Send with error checking
    if (sendto(sockfd, &icmph, sizeof(icmph), 0, 
              (sockaddr*)&target, sizeof(target)) <= 0) {
        perror("sendto() failed");
        close(sockfd);
        return false;
    }


    char buff[1024];
    sockaddr_in receiver;
    socklen_t addr_len = sizeof(receiver);
    
    ssize_t len = recvfrom(sockfd, buff, sizeof(buff), 0, (sockaddr*)&receiver, &addr_len);
    close(sockfd);
    //return len > 0;
    // Verify it's actually an ICMP reply
    if (len > 0) {
        struct iphdr* ip = (struct iphdr*)buff;
        if (ip->protocol == IPPROTO_ICMP) {
            struct icmphdr* received_icmp = (struct icmphdr*)(buff + (ip->ihl << 2));
            return (received_icmp->type == 0);  // ICMP_ECHOREPLY
        }
    }
    return false;
}
bool Radar::checkBanner(string addr) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        //perror("failed to create socket");
        return false;
    }

    sockaddr_in target;
    memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    inet_pton(AF_INET, addr.c_str(), &target.sin_addr);
    target.sin_port = htons(port);

    // Set timeout for connect
    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    if (connect(sockfd, (sockaddr*)&target, sizeof(target)) < 0) {
        close(sockfd);
        return false;
    }

    const char* request = "IDENTITY";
    if (send(sockfd, request, strlen(request), 0) <= 0) {
        close(sockfd);
        return false;
    }

    char buffer[1024];
    ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    close(sockfd);

    if (bytes_received <= 0) {
        return false;
    }

    buffer[bytes_received] = '\0';
    string response(buffer);
    return response.find(banner) != string::npos;
}

void Radar::findNodes() {
    mutex cout_mutex;
    // More robust regex pattern that trims whitespace
    regex s(R"(^\s*(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})\/(8|16|24)\s*$)");

    smatch m;

    // Trim whitespace from network string
    network.erase(remove_if(network.begin(), network.end(), ::isspace), network.end());

    if (!regex_search(network, m, s)) {
        cerr << "ERROR: Invalid network format. Expected format like: 192.168.1.0/24" << endl;
        cerr << "You provided: '" << network << "'" << endl;
        return;
    }

    // Validate octets
    for (int i = 1; i <= 4; i++) {
        int octet = stoi(m[i].str());
        if (octet < 0 || octet > 255) {
            cerr << "Invalid octet value: " << octet << endl;
            return;
        }
    }
    
    int cidr = stoi(m[5].str());
    vector<string> octets = {m[1].str(), m[2].str(), m[3].str(), m[4].str()};
    string ip = "";
    vector<string> addrs;

    switch(cidr) {
        case 8:
            ip = octets[0] + ".";
            for (int i = 0; i < 256; i++) {
                addrs.push_back(ip + to_string(i) + ".0.1");
            }
            break;
        case 16:
            ip = octets[0] + "." + octets[1] + ".";
            for (int i = 0; i < 256; i++) {
                addrs.push_back(ip + to_string(i) + ".1");
            }
            break;
        case 24:
            ip = octets[0] + "." + octets[1] + "." + octets[2] + ".";
            for (int i = 1; i < 255; i++) {  // Skip 0 and 255
                addrs.push_back(ip + to_string(i));
            }
            break;
        default:
            cout << "Unsupported network size" << endl;
            return;
    }

    cout << "Scanning " << addrs.size() << " addresses..." << endl;

    ThreadPool pool;
    mutex nodes_mutex;
    atomic<int> active_threads(0);
    atomic<int> scanned_count(0);//new
    const size_t total_addrs= addrs.size();
    const int max_concurrent = 100;  // Limit concurrent scans

    for (string addr : addrs) {
        active_threads++;
        pool.enqueue([this, addr, &nodes_mutex, &cout_mutex, &active_threads, &scanned_count, total_addrs] {
            try {
                bool is_alive= ping(addr);
                bool is_valid= false;

                if (is_alive)
                {
                    is_valid= checkBanner(addr);
                }

                {
                    lock_guard<mutex> lock(cout_mutex);
                    cout << "\rScanned: " << ++scanned_count << "/" << total_addrs 
                    << " | Active: " << active_threads 
                    << " | Found: " << nodes.size() << flush;
           }

           if (is_alive && is_valid) {
               lock_guard<mutex> lock(nodes_mutex);
               nodes.push_back(Node(addr));
               
               lock_guard<mutex> cout_lock(cout_mutex);
               cout << "\nFound valid node: " << addr << endl;
           }
       } catch (...) {
           lock_guard<mutex> lock(cout_mutex);
           cerr << "\nError scanning " << addr << endl;
       }
       active_threads--;
    });

    while (active_threads >= max_concurrent) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

while (active_threads > 0) {
    this_thread::sleep_for(chrono::milliseconds(100));
}

cout << "\nScan complete. Found " << nodes.size() << " valid nodes." << endl;
}

                
                /**if (ping(addr) && checkBanner(addr)) {
                    lock_guard<mutex> lock(nodes_mutex);
                    nodes.push_back(Node(addr));
                    cout << "Found valid node: " << addr << endl;
                }
            } catch (...) {
                cerr << "Error scanning " << addr << endl;
            }
            active_threads--;
        });

        // Simple throttling
        while (active_threads >= max_concurrent) {
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

    // Wait for all threads to complete
    while (active_threads > 0) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    cout << "Scan complete. Found " << nodes.size() << " valid nodes." << endl;
}*/