#include "Node.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <string>

using namespace std;

void Node::print_output(int modified, int lost, int duplicate, double num, int seq_num,
                        bitset<4> initialxorbits, string frammedPayload, int delay) {

    string lost_status = (lost == 0) ? "No" : "Yes";

    ofstream file("..\\simulations\\output.txt", ios_base::app);

    if (!file.is_open()) {
        cerr << "Error: Unable to open output file!" << endl;
        return;
    }
    if(strcmp(this->getName(),"Node0")==0){
        file << "At :" << num + simTime().dbl()
                 <<",Node:0"<< " [sent] frame with seq_number: " << seq_num
                 << " and payload = " << frammedPayload
                 << " and trailer = " << initialxorbits
                 << ",Modified = " << modified
                 << ", Lost= " << lost_status
                 << ", Duplicate = " << duplicate
                 << ", Delay =" << delay << endl;
        EV << "At :" << num + simTime().dbl()
                <<",Node:0"<< " [sent] frame with seq_number: " << seq_num
                << " and payload = " << frammedPayload
                << " and trailer = " << initialxorbits
                << ",Modified = " << modified
                << ", Lost= " << lost_status
                << ", Duplicate = " << duplicate
                << ", Delay =" << delay << endl;
   }
   else{
       file << "At :" << num + simTime().dbl()
               <<",Node:1"<< " [sent] frame with seq_number: " << seq_num
               << " and payload = " << frammedPayload
               << " and trailer = " << initialxorbits
               << ",Modified = " << modified
               << ", Lost= " << lost_status
               << ", Duplicate = " << duplicate
               << ", Delay =" << delay << endl;
       EV << "At : " << num + simTime().dbl()
               <<",Node:1"<< " [sent] frame with seq_number: " << seq_num
               << " and payload = " << frammedPayload
               << " and trailer = " << initialxorbits
               << ",Modified = " << modified
               << ", Lost= " << lost_status
               << ", Duplicate = " << duplicate
               << ", Delay =" << delay << endl;
   }

    file.close();
}

