#include "Node.h"
#include <iostream>
#include <string>
#include <bitset>
using namespace std;
void Node::fillBuffer(string txt_file, bool sender_bool) {

    int ws = par("WS").intValue();
    this->sender_node= sender_bool;
    string line;
    int i=0;
    ifstream myfile(txt_file);
    const char* text_file_char = txt_file.c_str();
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            string error_bits = "";

            error_bits+=line[0];
            error_bits+=line[1];
            error_bits+=line[2];
            error_bits+=line[3];

            line.erase(0, 5);
            pair<string, string> data_txt;
            data_txt.first = line;
            data_txt.second = error_bits;

            this->buffer.push_back(data_txt);
            i++;
            check_buffer.push_back(false);
        }
        myfile.close();
        if(i < ws){
            end = buffer.size() - 1;
        } else {
            end = ws-1;
        }
     } else {
         throw cRuntimeError("Error opening file!");
     }
}
