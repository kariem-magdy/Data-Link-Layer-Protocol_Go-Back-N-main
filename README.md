# Data Link Layer Protocols Simulation

This repository simulates Data Link Layer protocols between two OMNeT++ modules connected via a noisy channel. It implements the **Go-Back-N ARQ** protocol with byte-stuffing framing, parity-based error detection, and simulated errors (modification, loss, duplication, delay).

---

## Table of Contents
1. [System Overview](#system-overview)
2. [Features](#features)
3. [Prerequisites](#prerequisites)
4. [Building the Simulator](#building-the-simulator)
5. [Configuration & Inputs](#configuration--inputs)
6. [Running the Simulation](#running-the-simulation)
7. [Output & Logs](#output--logs)
8. [Project Structure](#project-structure)
9. [License](#license)

---

## System Overview
```
Coordinator.ned      # Coordinator module (initialization)
    |
    +-- Node.ned    # Node0 <--> Noisy Channel <--> Node1
```
- **Coordinator**: Reads `coordinator.txt`, selects starting node and start time.
- **Nodes**: Each loads its message list (`input0.txt` / `input1.txt`), performs framing, ARQ, error handling, and exchanges data/control frames.
- **Noisy Channel**: Introduces bit-level errors based on 4-bit codes.

## Features
- **Go-Back-N ARQ** with adjustable sender window *WS* and timeout *TO*.
- **Byte Stuffing** framing (`flag='$'`, `escape='/'`).
- **Even Parity** error detection (parity byte in trailer).
- **Channel Errors**: single-bit modification, frame loss, duplication, and delay in any combination.
- **ACK/NACK Loss**: configurable loss probability for control frames.
- **Detailed Logging** to console and `output.txt`.

## Prerequisites
- **OMNeT++ v5.x** (or compatible) installed and in your `PATH`.
- C++17 compiler (g++, clang++).
- Python (optional, for parsing logs).

## Building the Simulator
1. **Clone** this repository:
   ```bash
   git clone https://github.com/yourusername/data-link-sim.git
   cd data-link-sim
   ```
2. **Generate Makefile** (if needed):
   ```bash
   opp_makemake -f -o Makefile src/*.cc
   ```
3. **Compile**:
   ```bash
   make
   ```

## Configuration & Inputs
- **config.ini**: Simulation parameters
  ```ini
  # Window sizes
  WS=5         # Sender window size
  WR=1         # Receiver window size (fixed)
  
  # Timeouts & Delays
  TO=10        # Timeout interval (sec)
  PT=0.5       # Processing delay per frame (sec)
  TD=1.0       # Transmission delay (sec)
  ED=4.0       # Error-induced delay (sec)
  DD=0.1       # Duplication delay (sec)
  
  # Frame loss
  LP=0.1       # ACK/NACK loss probability
  ```
- **coordinator.txt**:
  ```txt
  <Node_id> <start_time>
  # e.g.: 0 2.0  => Node0 starts at t=2.0 seconds
  ```
- **input0.txt** / **input1.txt**:
  ```
  <4-bit error code> <Message payload>
  # e.g.: 1010 Hello World
  ```

### Error Codes
| Code  | Errors                                          |
|:----:|:------------------------------------------------|
| 0000 | No error                                        |
| 0001 | Delay                                           |
| 0010 | Duplication                                     |
| 0011 | Duplication + Delay                             |
| 0100 | Loss                                            |
| 0101 | Loss + Delay                                    |
| 0110 | Loss of original + duplicate                    |
| 0111 | Loss of both + Delay                            |
| 1000 | Modification                                    |
| 1001 | Modification + Delay                            |
| 1010 | Modification + Duplication                      |
| 1011 | Modification + Duplication + Delay              |
| 1100 | Modification + Loss                             |
| 1101 | Modification + Loss + Delay                     |
| 1110 | Modification + Loss of both copies              |
| 1111 | Modification + Delay + Loss of both copies      |

## Running the Simulation
Execute the compiled binary with your `.ini` and input files:
```bash
./dataLinkSim config.ini coordinator.txt input0.txt input1.txt
```
Or launch from the OMNeT++ IDE / `opp_run` with `-u Cmdenv`:
```bash
opp_run -u Cmdenv -f config.ini -l . dataLinkSim
```

## Output & Logs
- **Console**: Real-time log of events (reads, sends, receives, timeouts, control frames).
- **output.txt**: Same detailed logs written sequentially.

Log format highlights:
1. **Reading**:
   ```
   At time [t], Node[id], Introducing channel error with code=[bits]
   ```
2. **Frame Tx/Rx**:
   ```
   At time [t], Node[id] [sent/received] frame seq=[x], payload="…", trailer=[…], Modified=[bit|−1], Lost=[Yes/No], Dup=[0|1|2], Delay=[0|δ]
   ```
3. **Timeout**:
   ```
   Time out at [t], Node[id], frame seq=[x]
   ```
4. **Control**:
   ```
   At time [t], Node[id] Sending [ACK/NACK] num=[x], loss=[Yes/No]
   ```

## Project Structure
```
├── src/                         # All .cc and .h source files
│   ├─ Coordinator.cc            # Coordinator logic
│   ├─ Coordinator.h
│   ├─ Node.cc                   # Node sending/receiving logic
│   ├─ Node.h
│   ├─ errorHandling.cc          # Error code handling
│   ├─ loadMessages.cc           # Input parsing
│   ├─ payloadFraming.cc         # Byte stuffing logic
│   ├─ sendingReceivingMessage.cc# Frame send/receive handlers
│   ├─ timerAcks.cc              # Timeout & ACK timer
│   ├─ printOutputFile.cc        # Logging routines
│   └─ Makefile                  # Build instructions
├── ned/                         # Network definitions
│   ├─ Coordinator.ned
│   ├─ Node.ned
│   └─ package.ned               # Package & channel config
├── msg/                         # Message definitions
│   └─ MyMessage.msg             # Defines frame structure
│   ├─ MyMessage_m.cc            # Generated
│   └─ MyMessage_m.h
├── config.ini                   # Simulation parameters
├── coordinator.txt              # Coordinator input
├── input0.txt / input1.txt      # Message inputs
├── output.txt                   # Log output
└── README.md                    # This documentation
```

---
*Developed by Your Name Karim Magdy*

