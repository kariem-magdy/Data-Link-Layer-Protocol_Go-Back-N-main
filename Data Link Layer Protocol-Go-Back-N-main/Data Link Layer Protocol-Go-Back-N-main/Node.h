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

#ifndef __CN_PROJECT_NODE_H_
#define __CN_PROJECT_NODE_H_
#include "Coordinator.h"
#include "string.h"
#include <iostream>
#include <fstream>
#include "MyMessage_m.h"
#include <iostream>
#include <omnetpp.h>
#include "MyMessage_m.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <random>
#include <vector>
using namespace std;
using namespace omnetpp;


class Node : public cSimpleModule
{
    public:

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    void sending(int seq_num, string payload, double num ,string code );
    void recieving(cMessage *msg);

    virtual char errorDetection(string payload);
    virtual string modifyPayload(string msg,int &bit);

    virtual string framingPayload(string message);
    virtual string deframingPayload(string stuffedMessage);

    virtual void timer_start_Ack(double time_delay);
    virtual void handleTimeout();
    virtual void handleAck(MyMessage_Base*msg);
    virtual bool between(int a, int b, int c) ;

    virtual void fillBuffer(string txt_file, bool sender_bool);
    virtual void print_output(int modfied,int lost,int duplicate,double num , int seq_num,bitset<4> initialxorbits, string frammedPayload, int delay);

  private:

    const char FRAME_FLAG = '$';
    const char ESC = '/';

    bool sender_node = false;
    vector<pair<string, string>> buffer;
    int buffer_counter=0;
    int begin = 0;
    int end;
    int frameExpected = 0;
    int ack_expected_number=0;
    int sender_index = 0;
    double time_frame=0;
    double time=0;
    int seq_a=0;
    int seq_c;
    int counting = 0;
    vector<cMessage*> timeouts;
    vector<pair<MyMessage_Base*,double>> scheduledMsg;
    vector <bool> check_buffer;
};

#endif
