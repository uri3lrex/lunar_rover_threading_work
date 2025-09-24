// read config files based on specified attributes

////////////////////////////
//      UASAGE
////////////////////////////
// attribs order in constructor must mirror args order in read function call
//
//  CONFIG FILE EXPECTATIONS
////////////////////////////
// ATTRIB: value
// INT before int attribs
// MAP before maps and delimit end with END
// key: val
///////////////////////////

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <type_traits>
#include <unordered_map>

using namespace std;

class Reader {

    vector<string> attributes;

public:

    Reader(initializer_list<string> attribs);

    template <typename ...Args>
    void read(string file, Args & ... args);

};

template<typename ...Args>
void Reader::read(string file, Args & ... args) {

    string line;

    ifstream f(file);

    int i = 0;

    ([&]
    {
        if (i >= attributes.size())
            return;

        string curr_attrib = attributes[i];

        if (getline(f, line)) {
            auto pos = line.find(curr_attrib);

            if (pos != string::npos) {
                if (line.substr(0, pos).find("INT") != string::npos) {

                    string val = line.substr(pos + curr_attrib.length() + 2); // accounts for :_

                    // check for proper type matching
                    if constexpr (std::is_same_v<std::decay_t<Args>, int>) {
                        args = stoi(val);
                    } else {
                        cout << "Unexpected type mismatch for " + curr_attrib << endl;
                    }

                }  else if (line.substr(0, pos).find("MAP") != string::npos) {
                    
                    while (getline(f, line)) {
                        if (line.find("END") != string::npos)
                            break;

                        pos = line.find(": ");
                        string key = line.substr(0, pos);
                        string val = line.substr(pos+2); // account for :_

                        // check for proper type matching
                        if constexpr (std::is_same_v<std::decay_t<Args>, unordered_map<string, string>>) {
                            args.emplace(key, val);
                        } else {
                            cout << "Unexpected type mismatch for " + curr_attrib << endl;
                        }

                    }
                } else {

                    string val = line.substr(pos + curr_attrib.length() + 2); // accounts for :_

                    if constexpr (std::is_same_v<std::decay_t<Args>, string>) {
                        args = val;
                    } else {
                        cout << "Unexpected type mismatch for " + curr_attrib << endl;
                    }

                }
            }
            
        } else {
            cout << "Missing " + curr_attrib + " in config file" << endl;
        }

        ++i;

    } (), ...);
}