#include "Node.h"
#include <iostream>
#include <bitset>
using namespace std;
char Node:: errorDetection(string payload) {
    bitset<8> xorResult(payload[0]);

    for (size_t i = 1; i < payload.size(); ++i) {
        std::bitset<8> currentByte(payload[i]);
        xorResult ^= currentByte;
    }

    return static_cast<char>(xorResult.to_ulong());
}

string Node::modifyPayload(string msg,int &bit) {

    int charIndex = static_cast<int>(uniform(0, msg.size()));
    int bitIndex = static_cast<int>(uniform(0, 8));

    bitset<8> charBits(msg[charIndex]);

    charBits.flip(bitIndex);

    msg[charIndex] = static_cast<char>(charBits.to_ulong());

    bit = charIndex * 8 + bitIndex;

    return msg;
}

