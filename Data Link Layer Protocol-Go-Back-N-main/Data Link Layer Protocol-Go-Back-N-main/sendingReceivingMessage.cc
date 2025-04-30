#include "Node.h"
#include <iostream>
#include <string>
#include <bitset>
using namespace std;

void Node::sending(int seq_num, string payload, double num, string code) {
    int modifiedBit = -1;

    double pt = getParentModule()->par("PT").doubleValue();
    double td = getParentModule()->par("TD").doubleValue();
    double ed = getParentModule()->par("ED").doubleValue();
    double dd = getParentModule()->par("DD").doubleValue();

    
    MyMessage_Base* msg = new MyMessage_Base();

    string frammedPayload = framingPayload(payload);
    msg->setM_Payload(frammedPayload.c_str());
    string newPayload = msg->getM_Payload();
    char trailer = errorDetection(newPayload);

    msg->setSeq_Num(seq_num);
    msg->setM_Type(0);
    msg->setMycheckbits(trailer); 

    if (code == "1000" || code == "1001" || code == "1010" || code == "1011" || code == "1100" || code == "1101" || code == "1110" || code == "1111") {
        string modifiedPayload = modifyPayload(newPayload, modifiedBit);
        msg->setM_Payload(modifiedPayload.c_str());
    }

    
    bitset<4> initialxorbits(trailer);

    
    if (code == "0000") {        // No error
        print_output(-1, 0, 0, num, seq_num, initialxorbits, msg->getM_Payload(), 0);
    } else if (code == "0001") { // Error delay
        print_output(-1, 0, 0, num, seq_num, initialxorbits, msg->getM_Payload(), ed);
    } else if (code == "0010") { // Error Duplication
        print_output(-1, 0, 1, num, seq_num, initialxorbits, msg->getM_Payload(), 0);
    } else if (code == "0011") { // Error Duplications & Delay
        print_output(-1, 0, 1, num, seq_num, initialxorbits, msg->getM_Payload(), ed);
    } else if (code == "0100" ) { // Error Loss
         print_output(-1, 1, 0, num, seq_num, initialxorbits, msg->getM_Payload(), 0);
    } else if (code == "0101") { // Error Loss & Delay
         print_output(-1, 1, 0, num , seq_num, initialxorbits, msg->getM_Payload(), ed);
    } else if (code == "0110" || code == "0111") { // Error Loss & Duplications
         print_output(-1, 1, 1, num, seq_num, initialxorbits, msg->getM_Payload(), 0);
    } else if (code == "1000") { // Error Modification
        print_output(modifiedBit, 0, 0, num, seq_num, initialxorbits, msg->getM_Payload(), 0);
    } else if (code == "1001") { // Error Modification & Delay
        print_output(modifiedBit, 0, 0, num, seq_num, initialxorbits, msg->getM_Payload(), ed);
    } else if (code == "1010") { // Error Modification & Duplication
        print_output(modifiedBit, 0, 1, num, seq_num, initialxorbits, msg->getM_Payload(), 0);
    } else if (code == "1011") { // Error Modification & Duplication & Delay
        print_output(modifiedBit, 0, 1, num, seq_num, initialxorbits, msg->getM_Payload(), ed);
    } else if (code == "1100") { // Error Loss & Modification
        print_output(modifiedBit, 1, 0, num, seq_num, initialxorbits, msg->getM_Payload(), 0);
    } else if (code == "1101") { // Error Loss & Modification & Delay
        print_output(modifiedBit, 1, 0, num, seq_num, initialxorbits, msg->getM_Payload(), ed);
    } else if (code == "1110") { // Error Loss & Modification & Duplication
        print_output(modifiedBit, 1, 1, num, seq_num, initialxorbits, msg->getM_Payload(), 0);
    } else if (code == "1111") { // Error Loss & Modification & Duplication & Delay
        print_output(modifiedBit, 1, 1, num, seq_num, initialxorbits, msg->getM_Payload(), ed);
    }

    if (code == "0000" || code == "0010" || code == "1000" || code == "1010" ) {              
        sendDelayed(msg, num + pt + td, "out");
    } else if (code == "0001" || code == "0011" || code == "1001" || code == "1011") {        
        sendDelayed(msg, num + pt + td + ed, "out");
    }

    // Handle Error Duplication
    if (code == "0010") { // Error Duplication
        MyMessage_Base* duplicateMsg = new MyMessage_Base();

        string frammedPayload = framingPayload(payload);
        duplicateMsg->setM_Payload(frammedPayload.c_str());
        string newPayload = msg->getM_Payload();
        char trailer = errorDetection(newPayload);

        duplicateMsg->setSeq_Num(seq_num);
        duplicateMsg->setM_Type(0);
        duplicateMsg->setMycheckbits(trailer);

        sendDelayed(duplicateMsg, num + dd + td+ pt, "out");

        bitset<4> initialxorbits(trailer);
        print_output(-1, 0, 2, num + dd, seq_num, initialxorbits, duplicateMsg->getM_Payload(), 0);

    } else if(code =="1010") { // Error Modification & Duplication
        MyMessage_Base* duplicateMsg = new MyMessage_Base();

        string frammedPayload = framingPayload(payload);
        string modifiedPayload = modifyPayload(frammedPayload, modifiedBit);
        duplicateMsg->setM_Payload(modifiedPayload.c_str());
        string newPayload = msg->getM_Payload();
        char trailer = errorDetection(newPayload);

        duplicateMsg->setSeq_Num(seq_num);
        duplicateMsg->setM_Type(0);
        duplicateMsg->setMycheckbits(trailer);

        sendDelayed(duplicateMsg, num + dd+ pt + td, "out");

        bitset<4> initialxorbits(trailer);
        print_output(modifiedBit, 0, 2, num + dd, seq_num, initialxorbits, duplicateMsg->getM_Payload(), 0);

    } else if(code == "0011") { // Error Duplications & Delay
        MyMessage_Base* duplicateMsg = new MyMessage_Base();

        string frammedPayload = framingPayload(payload);
        duplicateMsg->setM_Payload(frammedPayload.c_str());
        string newPayload = duplicateMsg->getM_Payload();
        char trailer = errorDetection(newPayload);

        duplicateMsg->setSeq_Num(seq_num);
        duplicateMsg->setM_Type(0);
        duplicateMsg->setMycheckbits(trailer);

        sendDelayed(duplicateMsg, num + dd + td + ed + pt, "out");

        bitset<4> initialxorbits(trailer);
        print_output(-1, 0, 2, num + dd, seq_num, initialxorbits, duplicateMsg->getM_Payload(), ed);

    } else if(code == "1011") { // Error Modification & Duplication & Delay
        MyMessage_Base* duplicateMsg = new MyMessage_Base();

        string frammedPayload = framingPayload(payload);
        string modifiedPayload = modifyPayload(frammedPayload, modifiedBit);
        duplicateMsg->setM_Payload(modifiedPayload.c_str());
        string newPayload = msg->getM_Payload();
        char trailer = errorDetection(newPayload);

        duplicateMsg->setSeq_Num(seq_num);
        duplicateMsg->setM_Type(0);
        duplicateMsg->setMycheckbits(trailer);

        sendDelayed(duplicateMsg, num + dd + td + ed + pt, "out");

        bitset<4> initialxorbits(trailer);
        print_output(modifiedBit, 0, 2, num + dd, seq_num, initialxorbits, duplicateMsg->getM_Payload(), ed);

    }

      timer_start_Ack(num);

    pair<MyMessage_Base*, double> sentMessage;
    sentMessage.first = msg;
    sentMessage.second = num + td + pt;
    scheduledMsg.push_back(sentMessage);
}

void Node::recieving(cMessage *msg)
{
    int ws = getParentModule()->par("WS").intValue();
    double td = getParentModule()->par("TD").doubleValue();
    double lp = getParentModule()->par("LP").doubleValue();
    double pt = getParentModule()->par("PT").doubleValue();

    MyMessage_Base* message = check_and_cast<MyMessage_Base *>(msg);

    bool sendAck=false;
    char newTrailer = errorDetection(message->getM_Payload());
    bool error_detected = (message->getMycheckbits() != newTrailer);

    MyMessage_Base* ackMessage = new MyMessage_Base;
    if (message->getSeq_Num() != frameExpected) {
        ackMessage->setAck_Num((frameExpected + ws) % (1 + ws));
        ackMessage->setM_Type(1) ;

    } else if((message->getSeq_Num() == frameExpected) && error_detected == false) {
        ofstream file("..\\simulations\\output.txt", ios_base::app);
        string m =message->getM_Payload();
        string frame = deframingPayload(m);
        EV<<"At time "<<simTime().dbl()-pt<<","<<this->getName()<<" received msg = "<<frame<<endl;
        file<<"At time "<<simTime().dbl()-pt<<","<<this->getName()<<" received msg = "<<frame<<endl;
        file.close();
        ackMessage->setAck_Num((frameExpected) % (1 + ws));
        frameExpected = (frameExpected + 1) % (ws + 1);
        ackMessage->setM_Type(1);
        sendAck=true;

    } else if((message->getSeq_Num() == frameExpected) && error_detected == true) {
        ackMessage->setM_Type(2) ;
        ackMessage->setAck_Num((frameExpected) % (1 + ws));
        sendAck=true;
    }

   // message->getAck_Num(message->getSeq_Num());

    int prob = uniform(0,100);
    if(prob > lp ) {
        if(sendAck){
        if(ackMessage->getM_Type()==2) { // NACK NO LOSS

            ofstream file("..\\simulations\\output.txt", ios_base::app);

            if (!file.is_open()) {
                cerr << "Error: Unable to open output file!" << endl;
                return;
            }

            file <<"At time["<<simTime().dbl()<<"] "<<this->getName()<<" sending[NACK] with number["<<ackMessage->getAck_Num()<<"], loss [No]"<<endl; //number da eh ? w loss eh brdo?
            EV <<"At time["<<simTime().dbl()<<"] "<<this->getName()<<" sending[NACK] with number["<<ackMessage->getAck_Num()<<"], loss [No]"<<endl; //number da eh ? w loss eh brdo?

           
            file.close();

        } else { // ACK NO LOSS

            ofstream file("..\\simulations\\output.txt", ios_base::app);

            if (!file.is_open()) {
                cerr << "Error: Unable to open output file!" << endl;
                return;
            }

            file <<"At time["<<simTime().dbl()<<"] "<<this->getName()<<" sending[ACK] with number["<<ackMessage->getAck_Num()<<"], loss [No]"<<endl;
            EV <<"At time["<<simTime().dbl()<<"] "<<this->getName()<<" sending[ACK] with number["<<ackMessage->getAck_Num()<<"], loss [No]"<<endl;
            file.close();
        }
    }
        sendDelayed(ackMessage,td,"out");

    } else { // LOSS
        if(ackMessage->getM_Type()==2) { // NACK  LOSS
            ofstream file("..\\simulations\\output.txt", ios_base::app);


        if (!file.is_open()) {
            cerr << "Error: Unable to open output file!" << endl;
            return;
        }
        file <<"At time["<<simTime().dbl()<<"] "<<this->getName()<<" sending[NACK] with number["<<ackMessage->getAck_Num()<<"], loss [Yes]"<<endl;
        EV <<"At time["<<simTime().dbl()<<"] "<<this->getName()<<" sending[NACK] with number["<<ackMessage->getAck_Num()<<"], loss [Yes]"<<endl;

        file.close();
        }else{ //ack loss
            ofstream file("..\\simulations\\output.txt", ios_base::app);


        if (!file.is_open()) {
            cerr << "Error: Unable to open output file!" << endl;
            return;
        }
        file <<"At time["<<simTime().dbl()<<"] "<<this->getName()<<" sending[ACK] with number["<<ackMessage->getAck_Num()<<"], loss [Yes]"<<endl;
        EV <<"At time["<<simTime().dbl()<<"] "<<this->getName()<<" sending[ACK] with number["<<ackMessage->getAck_Num()<<"], loss [Yes]"<<endl;

        file.close();
        }

    }
    cout<<"probality="<<prob<<endl;

    cancelAndDelete(message);
}
