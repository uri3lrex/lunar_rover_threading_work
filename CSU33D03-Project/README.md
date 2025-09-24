# CSU33D03-Project
Group 3 Project Implementation

## Protocol

*Current Phase: 1*

Working in C++ and using Unix based implementations.

*TODO*

- [ ] Fix multithreading
- [ ] Set up VPN for testing
- [ ] Decide on communication dictionary for negotiation and agreement between machines
- [ ] Figure out how to simulate the distance from earth to moon
    - transmission delay and bit error
        - formulas in general class group gc
    - delay the comms?
    - add more hops between nodes?
    - not sure

### Phase 1 Discover [ ]

*Associated files: radar.\**

(Sufficiently realistically) discover other proximate lunar rovers that you can potentially interact and collaborate with. (10 marks) 

- ICMP protocol – model nmap tool / ping command to discover nodes in the network 
    - Use threading to get through the network faster?
    - Thread pooling?

- Documentation on nmap and ping online we can use 

- Based on ICMP probe response we can populate a list? Or other container to hold the addresses of the available nodes 

- Might also want to ask for banner / identifier from machine to check if it is a viable node (lunar rover / raspberry pi) 

**References**
- [RFC792](https://datatracker.ietf.org/doc/html/rfc792)
- [RFC1071](https://datatracker.ietf.org/doc/html/rfc1071)
- [Socket programming](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/socket.2.html)
- [Gen ICMP](https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol)
- [Thread Pooling](https://en.wikipedia.org/wiki/Thread_pool)
    - [In C++](https://www.geeksforgeeks.org/thread-pool-in-cpp/)

### Phase 2 Negotiate [ ]

Negotiate with the nodes, as required, to discover their offered services, capabilities, data and constraints, and to advise them of your offered services, capabilities and constraints. (15 marks)  

- Might have to set up the raspberry pis or vms to run a simple program to interface with communication? 

- TCP handshake-esque – ask if node is up, wait for response, ask to connect and communicate, wait for response, ask for their ‘language’ or services, send ours back 

### Phase 3 Agreement [ ]

Establish agreement and consensus on a collaborative activity, service or task to be provided. (15 marks)  

- Check that it understands our protocol 

### Phase 4 Action [ ]

An action that demonstrates the meaningful and effective use of the agreed consensus/outcome which you have just achieved. (10 marks) 

- Communicate with the node 

- Send and receive data 

- Mimic movement 
    - ‘rover’ can send back “forward”, “back”, “left”, “right” 



## Bonuses
Some info for working on the bonuses

### 1 
---

### 2 
---

### 3

[Ping of death](http://www.ping127001.com/pingpage.htm)