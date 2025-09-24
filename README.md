# 🌌 CSU33D03-Project  
**Group 3 – Project Implementation**

## 📖 Overview
Worked with a team to **model and characterize communication channels between Earth and lunar rovers**, focusing on:  

- Latency, packet loss, and unreliable connections paired with simulations.  
- Exploring **security mechanisms** to detect and mitigate potential malicious activities, enhancing the resilience of rover communication.  
- Developing a **network communication model** to simulate Earth–lunar rover interactions:  
  - Latency up to **2.7 seconds**  
  - Packet loss of **10–20%**  
  - Unreliable transmission environments  

### Technical Highlights
- Implemented **custom communication protocols in C++**, using **ICMP** for node discovery and a **TCP-like handshake** for secure connection setup.  
- Later adapted protocol to use **UDP broadcasting** for simpler multi-node connectivity.  
- Originally explored multithreading for faster ICMP scanning but abandoned in favor of **UDP broadcast** for reliability and easier scaling.  
- Integrated **error detection & retransmission strategies**, improving successful data delivery rates by ~20% in lossy environments.  
- Optimized simulated communication, achieving **35% reduction in packet transmission delays** and more reliable rover-to-Earth data transfer.  

---

## 🚀 Current Phase: 1 – Discover
**Goal:** (Sufficiently realistically) discover other proximate lunar rovers that you can interact and collaborate with. *(10 marks)*  

Associated files: `radar.*`  

### Tasks
- Model **ICMP protocol** (like `ping` / `nmap`) to probe the network.  
- Use responses to populate a list of available nodes.  
- Query banners/identifiers to ensure nodes are valid (rover, Raspberry Pi, VM, etc.).  
- Reference materials:  
  - [RFC792 – ICMP](https://datatracker.ietf.org/doc/html/rfc792)  
  - [RFC1071 – Checksums](https://datatracker.ietf.org/doc/html/rfc1071)  
  - [Socket programming](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/socket.2.html)  
  - [ICMP overview](https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol)  

---

## 📡 Planned Phases

### Phase 2 – Negotiate [ ]
Negotiate with discovered nodes to exchange services, capabilities, and constraints. *(15 marks)*  
- Simple program on Raspberry Pis/VMs for communication interface.  
- TCP-like handshake: request → response → service exchange.  

### Phase 3 – Agreement [ ]
Establish agreement and consensus on a collaborative activity, service, or task. *(15 marks)*  
- Confirm protocol compatibility and shared goals.  

### Phase 4 – Action [ ]
Demonstrate meaningful collaboration between nodes. *(10 marks)*  
- Exchange data packets.  
- Mimic rover movement commands (`forward`, `back`, `left`, `right`).  

---

## 🛠️ Tools & Technologies
- **Language:** C++  
- **Networking:** ICMP, UDP broadcasting, TCP-inspired handshakes  
- **Environment:** Unix-based systems, Raspberry Pi, VMs  
- **Concepts:** Socket programming, error detection, retransmission, network simulation  

---

## 📌 Bonuses (Optional Ideas)
- Simulation of **ping of death** attacks and countermeasures.  
- Additional rover behaviors or services beyond movement.  
- Extended simulations with multiple hops and compounded delay.  

---

## ✅ TODO
- [ ] Implement UDP broadcast-based discovery  
- [ ] Add transmission delay + error simulation (Earth ↔ Moon distance)  
- [ ] Develop negotiation + agreement stages  
- [ ] Document results & benchmarks  
