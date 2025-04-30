//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see http://www.gnu.org/licenses.
//

#include "Coordinator.h"
#include "string.h"
#include <iostream>
#include <fstream>
#include "MyMessage_m.h"
#include "Node.h"

Define_Module(Node);

void Node::initialize() {
    // TODO - Generated method body

}

void Node::handleMessage(cMessage *msg) {

       double pt = par("PT").doubleValue();
       double ed = par("ED").doubleValue();
       double dd = par("DD").doubleValue();
       int ws =par("WS").intValue();
       //int ws =4;
   // string received_coord = msg->getName();

    if (msg->isSelfMessage()) {
        handleTimeout();
        cout << "timedout" << endl;
    }

    if (!msg->isSelfMessage()) {
        

        if (strcmp(msg->getSenderGate()->getName(), "out0") == 0 || strcmp(msg->getSenderGate()->getName(), "out1") == 0) {

            if (strcmp(getName(), "Node0") == 0) {
                fillBuffer("..\\simulations\\node0.txt", true);
                seq_c = ws;
            }
            else {
                fillBuffer("..\\simulations\\node1.txt", true);
                seq_c = ws;
            }
        }
    }

    if (this->sender_node == true) {
        if (!msg->isSelfMessage()) {
            if (strcmp(msg->getSenderGate()->getName(), "out") == 0) {
                MyMessage_Base *message = check_and_cast<MyMessage_Base *>(msg);
                if (message->getM_Type() == 1) {
                    handleAck(message);
                }
            }
        }

        for (int i = sender_index; i <= end; i++) {
            sender_index++;
            double ms = simTime().dbl();
            time = time_frame - ms;

            if (time < 0) {
                time = pt;
            } else {
                time += pt;
            }

            // In case of error delay
            if (buffer[i].second == "0001" || buffer[i].second == "0011" || buffer[i].second == "1001" || buffer[i].second == "1011") {
                time += ed;
            }

            // In case of no loss 
            if (buffer[i].second != "0100" && buffer[i].second != "0101" &&
                buffer[i].second != "0110" && buffer[i].second != "0111" &&
                buffer[i].second != "1100" && buffer[i].second != "1101" &&
                buffer[i].second != "1110" && buffer[i].second != "1111") {
                if (!check_buffer[i]) {
                    if (buffer[i].second == "0001" || buffer[i].second == "0011" || buffer[i].second == "1001" || buffer[i].second == "1011") {
                        ofstream file("..\\simulations\\output.txt", ios_base::app);
                        if (!file.is_open()) {
                            cerr << "Error: Unable to open output file!" << endl;
                            return;
                        }

                        file << "At [" << simTime().dbl() + time - pt - ed << "] " << this->getName() << " introducing channel error with code =[" << buffer[i].second << "]" << endl;
                        EV << "At [" << simTime().dbl() + time - pt - ed << "] " << this->getName() << " introducing channel error with code =[" << buffer[i].second << "]" << endl;
                        file.close();
                    } else {
                        ofstream file("..\\simulations\\output.txt", ios_base::app);
                        if (!file.is_open()) {
                            cerr << "Error: Unable to open output file!" << endl;
                            return;
                        }
                        file << "At [" << simTime().dbl() + time - pt << "] " << this->getName() << " introducing channel error with code =[" << buffer[i].second << "]" << endl;
                        EV << "At [" << simTime().dbl() + time - pt << "] " << this->getName() << " introducing channel error with code =[" << buffer[i].second << "]" << endl;
                        file.close();
                    }
                    check_buffer[i] = true;
                }

                cout << "time2=" << time;
                if(buffer[i].second == "0001" || buffer[i].second == "0011" || buffer[i].second == "1001" || buffer[i].second == "1011"){
                    sending((i % (ws + 1)), buffer[i].first, time - ed, buffer[i].second);

                }
                else{
                    sending((i % (ws + 1)), buffer[i].first, time, buffer[i].second);

                }

            }

            // In case of loss
            if (buffer[i].second == "0100" || buffer[i].second == "0101" || buffer[i].second == "0110" || buffer[i].second == "0111" ||
                buffer[i].second == "1100" || buffer[i].second == "1101" || buffer[i].second == "1111" || buffer[i].second == "1110") {
                timer_start_Ack(time);
                if (!check_buffer[i]) {
                    ofstream file("..\\simulations\\output.txt", ios_base::app);
                    if (!file.is_open()) {
                        cerr << "Error: Unable to open output file!" << endl;
                        return;
                    }
                    file << "At [" << simTime().dbl() + time - pt << "] " << this->getName() << " introducing channel error with code =[" << buffer[i].second << "]" << endl;
                    EV << "At [" << simTime().dbl() + time - pt << "] " << this->getName() << " introducing channel error with code =[" << buffer[i].second << "]" << endl;
                    file.close();

                    check_buffer[i] = true;
                }
            }

            if (buffer[i].second == "1111" || buffer[i].second == "1110" || buffer[i].second == "0111" || buffer[i].second == "0110") {
                double error_time2 = time + dd;
            }

            if (buffer[i].second == "0001" || buffer[i].second == "0011" || buffer[i].second == "1001" || buffer[i].second == "1011") {
                time -= ed;
            }

            // Loss without duplications
            if (buffer[i].second == "1100") {
                int errorbit=-1;
                string frammedPayload = framingPayload(buffer[i].first);
                string modifiedPayload = modifyPayload(frammedPayload, errorbit);
                print_output(errorbit, 1, 0, time, (i % (ws + 1)), 0, modifiedPayload, 0);
            }
            if (buffer[i].second == "1101") {
                int errorbit=-1;
                string frammedPayload = framingPayload(buffer[i].first);
                string modifiedPayload = modifyPayload(frammedPayload, errorbit);
                print_output(errorbit, 1, 0, time, (i % (ws + 1)), 0, modifiedPayload, ed);
            }
            if (buffer[i].second == "0100") {
                string frammedPayload = framingPayload(buffer[i].first);
                print_output(-1, 1, 0, time, (i % (ws + 1)), 0, frammedPayload, 0);
            }
            if (buffer[i].second == "0101") {
                string frammedPayload = framingPayload(buffer[i].first);
                print_output(-1, 1, 0, time, (i % (ws + 1)), 0, frammedPayload, ed);
            }

            // Duplication + loss conditions
            if (buffer[i].second == "0110") {
                string frammedPayload = framingPayload(buffer[i].first);
                print_output(-1, 1, 1, time, (i % (ws + 1)), 0, frammedPayload, 0);
                print_output(-1, 1, 2, time + dd, (i % (ws + 1)), 0, frammedPayload, 0);
            }
            if (buffer[i].second == "0111") {
                string frammedPayload = framingPayload(buffer[i].first);
                print_output(-1, 1, 1, time, (i % (ws + 1)), 0, frammedPayload, ed);
                print_output(-1, 1, 2, time + dd, (i % (ws + 1)), 0, frammedPayload, ed);
            }
            if (buffer[i].second == "1110") {
                int errorbit=-1;
                string frammedPayload = framingPayload(buffer[i].first);
                string modifiedPayload = modifyPayload(frammedPayload, errorbit);
                print_output(errorbit, 1, 1, time, (i % (ws + 1)), 0, modifiedPayload, 0);
                print_output(errorbit, 1, 2, time + dd, (i % (ws + 1)), 0, modifiedPayload, 0);
            }
            if (buffer[i].second == "1111") {
                int errorbit=-1;
                string frammedPayload = framingPayload(buffer[i].first);
                string modifiedPayload = modifyPayload(frammedPayload, errorbit);
                print_output(errorbit, 1, 1, time, (i % (ws + 1)), 0, modifiedPayload, 0);
                print_output(errorbit, 1, 2, time + dd, (i % (ws + 1)), 0, modifiedPayload, 0);
            }


            time_frame = ms + time;
        }
    } else if (this->sender_node == false) {
        recieving(msg);
    }
}
