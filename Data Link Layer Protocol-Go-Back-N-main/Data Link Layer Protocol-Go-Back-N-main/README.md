# Go-Back-N Protocol Simulation

## Overview
This project simulates a Go-Back-N (GBN) protocol for data link layer communication between two nodes connected via a noisy channel. The simulation handles various types of errors, including packet modification, loss, duplication, and delays, to emulate realistic network conditions.

## Features
- Implements the Go-Back-N protocol with sender and receiver windows.
- Uses Byte Stuffing for framing and Even Parity Byte for error detection.
- Configurable parameters for transmission delays, timeouts, and error probabilities via an INI file.
- Logs detailed information about message transmission, errors, and acknowledgments.

## Error Codes and Effects
| Error Code | Effect                          |
|------------|---------------------------------|
| 0000       | No error                       |
| 0001       | Delay                          |
| 0010       | Duplication                    |
| 0011       | Duplication and Delay          |
| 0100       | Loss                           |
| 0101       | Loss and Delay                 |
| 0110       | Loss of both original and duplicate messages |
| 0111       | Loss of both original and duplicate messages with delay |
| 1000       | Modification                   |
| 1001       | Modification and Delay         |
| 1010       | Modification and Duplication   |
| 1011       | Modification, Duplication, and Delay |
| 1100       | Modification and Loss          |
| 1101       | Modification, Loss, and Delay  |
| 1110       | Modification and Loss for both original and duplicate messages |
| 1111       | Modification, Delay, and Loss for both original and duplicate messages |

## System Components
1. **Nodes**: Two nodes (Node0 and Node1) exchange messages.
   - Messages are read from input text files (`input0.txt` and `input1.txt`).
   - Each message has a 4-bit binary prefix indicating potential errors.

2. **Coordinator**:
   - Initializes the simulation by determining the starting node and time.
   - Reads parameters from `coordinator.txt`.

3. **Channel**:
   - Simulates transmission delays and introduces errors as specified in the configuration.

## Configuration
The system parameters are defined in an INI file:
- **WS**: Sender window size (default: 5)
- **TO**: Timeout interval in seconds (default: 10)
- **PT**: Processing time for each frame (default: 0.5 seconds)
- **TD**: Transmission delay for any frame (default: 1.0 seconds)
- **ED**: Error delay for any frame (default: 4.0 seconds)
- **DD**: Duplication delay before sending the second version (default: 0.1 seconds)
- **LP**: ACK/NACK frame loss probability (default: 10%)

## Output
The simulation generates a log file (`output.txt`) with details of each message transmission:
- Processing time, sending time, and sequence numbers.
- Error codes and their effects.
- ACK/NACK frames with loss status.


