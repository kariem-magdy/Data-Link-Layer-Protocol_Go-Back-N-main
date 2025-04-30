#include "Node.h"
#include <iostream>


bool Node::between(int a, int b, int c) {
    if (c > a) {
        if (a <= b && b < c) {
            return true;
        }
    } else if (c < a) {
        if (a <= b) {
            return true;
        }
        if (b < c) {
            return true;
        }
    }
    return false;
}

void Node::timer_start_Ack(double time_delay){
    double to = getParentModule()->par("TO").doubleValue();
    cMessage* timer = new cMessage();
    scheduleAt(simTime() + to + time_delay, timer);
    timeouts.push_back(timer);
}

void Node::handleTimeout()
{
    int seqnum=(begin%((int)getParentModule()->par("WS").intValue()+1));;



    while(!scheduledMsg.empty())
    {
        cMessage* temp_msg = scheduledMsg[0].first;

        if(scheduledMsg[0].second > simTime().dbl())
        {
            seqnum=scheduledMsg[0].first->getSeq_Num();

            cancelAndDelete(temp_msg);
        }
        scheduledMsg.erase(scheduledMsg.begin());

    }
    while(!timeouts.empty())
    {
        cMessage* temp_msg = timeouts[0];
        cancelAndDelete(temp_msg);
        timeouts.erase(timeouts.begin());

    }

    ofstream file("..\\simulations\\output.txt", ios_base::app);
    if (!file.is_open()) {
        cerr << "Error: Unable to open output file!" << endl;
        return;
    }
    file <<"Timeout event at time["<<simTime().dbl()<<"] "<<this->getName()<<"for frame with seq_num=["<<seqnum<<"]"<<endl;
    EV <<"Timeout event at time["<<simTime().dbl()<<"] "<<this->getName()<<"for frame with seq_num=["<<seqnum<<"]"<<endl;

    file.close();

    sender_index = begin;
    buffer[begin].second = "0000";
}

void Node::handleAck(MyMessage_Base*msg)
{
   if(between(seq_a,msg->getAck_Num(),seq_c))
   {
       while(!scheduledMsg.empty())
       {
           if((begin%((int)getParentModule()->par("WS").intValue()+1) != msg->getAck_Num()))
           {
               cMessage* temp_msg_1 = timeouts[0];
               cMessage* temp_msg_2 = scheduledMsg[0].first;
               timeouts.erase(timeouts.begin());
               scheduledMsg.erase(scheduledMsg.begin());
               cancelAndDelete(temp_msg_1);
               //scheduledMsg_time.erase(scheduledMsg_time.begin());
               begin += 1;
               if(end+1 != buffer.size())
               {
                  end += 1;
               }
           }
           else
           {
               break;
           }

       }

      if(strcmp(this->getName(),"Node0")==0)
      {

          ofstream file("..\\simulations\\output.txt", ios_base::app);
          if (!file.is_open()) {
              cerr << "Error: Unable to open output file!" << endl;
              return;
          }
          file <<"At time["<<simTime().dbl()-getParentModule()->par("TD").doubleValue()<<"] Node1 sending[ACK] with number["<<msg->getAck_Num()<<"], loss [No]"<<endl;
          file.close();
      }
      else
      {
          ofstream file("..\\simulations\\output.txt", ios_base::app);
          if (!file.is_open()) {
              cerr << "Error: Unable to open output file!" << endl;
              return;
          }
          file <<"At time["<<simTime().dbl()-getParentModule()->par("TD").doubleValue()<<"] Node0 sending[ACK] with number["<<msg->getAck_Num()<<"], loss [No]"<<endl;
          file.close();
      }

      cMessage* temp_msg_1 = timeouts[0];
      cMessage* temp_msg_2 = scheduledMsg[0].first;
      timeouts.erase(timeouts.begin());
      scheduledMsg.erase(scheduledMsg.begin());
      cancelAndDelete(temp_msg_1);
      cancelAndDelete(msg);
      //scheduledMsg_time.erase(scheduledMsg_time.begin());
      begin += 1;
      if(end + 1 != buffer.size())
      {
         end += 1;
      }
      seq_a=(begin%((int)getParentModule()->par("WS").intValue()+1));
      seq_c=((end+1)%((int)getParentModule()->par("WS").intValue()+1));
   }
}
