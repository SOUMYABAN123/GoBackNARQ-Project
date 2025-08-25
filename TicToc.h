/*
 * TicToc.h
 *
 *  Created on: 20 Aug-2025
 *      Author: soumya
 */

#ifndef TICTOC_H_
#define TICTOC_H_

#include <omnetpp.h>
#include <queue>

using namespace omnetpp;

class DataPacket : public cMessage {
  public:
    int seqNum;
    DataPacket(int seq) : cMessage("data"), seqNum(seq) {}
    DataPacket(const DataPacket& other) : cMessage(other) {
        seqNum = other.seqNum;
    }
    virtual DataPacket *dup() const override {
        return new DataPacket(*this);
    }
};

class ControlPacket : public cMessage {
  public:
    int windowSize = 0;
    int ackNum = -1;
    const char *type;
    ControlPacket(const char *t) : cMessage(t), type(t) {}
    ControlPacket(const ControlPacket& other) : cMessage(other) {
        windowSize = other.windowSize;
        ackNum = other.ackNum;
        type = other.type;
    }
    virtual ControlPacket *dup() const override {
        return new ControlPacket(*this);
    }
};



#endif /* TICTOC_H_ */
