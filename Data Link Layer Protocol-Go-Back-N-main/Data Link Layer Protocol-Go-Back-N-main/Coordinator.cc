//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Coordinator.h"
#include "string.h"
#include <iostream>
#include <fstream>
#include "MyMessage_m.h"
using namespace std;
Define_Module(Coordinator);

void Coordinator::initialize()
{

    ifstream file("..\\simulations\\coordinator.txt");
    if (!file.is_open()) {
        EV << "Error: Could not open coordinatorrrr.txt file." <<endl;
        return;
    }

    string line;

    if (getline(file, line)) {
        int nodeId = 0;
        double startTime = 0.0;


       istringstream iss(line);
        if (!(iss >> nodeId >> startTime)) {
            EV << "Error: Invalid forkhjhkjmat in coordinator.txt file." <<endl;
            return;
        }

        EV << "Node ID: " << nodeId << ", Start Time: " <<startTime<<endl;


        MyMessage_Base *startMsg = new MyMessage_Base("start");



        startMsg->setM_Type(3);
        if (nodeId == 0) {

            string msgsend = "Node0";
            startMsg->setM_Payload(msgsend.c_str());
            sendDelayed(startMsg,startTime,"out0");
        } else if(nodeId == 1) {
            string msgsend = "Node1";
            startMsg->setM_Payload(msgsend.c_str());
            sendDelayed(startMsg,startTime,"out1");
        }

    } else {
        EV << "Error: coordinator.txt file is empty." <<endl;
    }

    file.close();
}

void Coordinator::handleMessage(cMessage *msg)
{

}
