#include "Node.h"
#include <iostream>
#include <string>


string Node::framingPayload(string message) {
    string stuffedMessage;

    stuffedMessage.push_back(FRAME_FLAG);

    for (size_t i = 0; i < message.size(); ++i) {
        char byte = message[i];
        if (byte == FRAME_FLAG || byte == ESC) {
            stuffedMessage.push_back(ESC);
            stuffedMessage.push_back(byte);
        } else {
            stuffedMessage.push_back(byte);
        }
    }

    stuffedMessage.push_back(FRAME_FLAG);

    return stuffedMessage;
}

string Node::deframingPayload(string stuffedMessage) {
    string message;
    bool isEscaped = false;

    for (size_t i = 1; i < stuffedMessage.size() - 1; ++i) {
        char byte = stuffedMessage[i];
        if (isEscaped) {
            message.push_back(byte);
            isEscaped = false;
        } else if (byte == ESC) {
            isEscaped = true;
        } else {
            message.push_back(byte);
        }
    }

    return message;
}

