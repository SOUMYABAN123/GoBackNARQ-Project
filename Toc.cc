#include <omnetpp.h>
#include <queue>
#include <TicToc.h>

using namespace omnetpp;

class Toc : public cSimpleModule
{
  private:
    cQueue buffer;          // The receive buffer
    int expectedSeq;        // Next in-order seq num
    int nCounter;           // Count how many packets accepted since last ACK
    bool isFull;            // Whether buffer is full
    bool rnrSent;           // Whether we've sent RNR

    int windowSize;         // from .ini
    int nValue;             // how many packets -> send ack
    double errorRate;       // random drop

  protected:
    virtual void initialize() override {
        expectedSeq = 0;
        nCounter = 0;
        isFull = false;
        rnrSent = false;

        buffer.setName("RecvBuffer");

        windowSize = par("windowSize").intValue();
        nValue = par("nValue").intValue();
        errorRate = par("errorRate").doubleValue();
    }

    void sendControlMessage(const char *type, int ackNum=-1) {
        ControlPacket* ctrl = new ControlPacket(type);
        if (!strcmp(type, "QUERY_REPLY")) {
            ctrl->windowSize = windowSize;
        } else if (!strcmp(type, "RR")) {
            ctrl->ackNum = ackNum;
        }
        send(ctrl, "out");
    }

    void tryToExtractInOrder() {
        bool extracted = true;
        while (extracted && !buffer.isEmpty()) {
            extracted = false;
            cObject* obj = buffer.front();
            if (DataPacket* dp = dynamic_cast<DataPacket*>(obj)) {
                if (dp->seqNum == expectedSeq) {
                    EV << "[Toc] Accepting in-order packet seq=" << dp->seqNum
                       << " from buffer. buffer size=" << (buffer.getLength()-1) << "\n";
                    buffer.pop();
                    delete dp;

                    expectedSeq = (expectedSeq + 1) % 8;
                    nCounter++;
                    extracted = true;
                }
            }
        }

        if ((int)buffer.getLength() < windowSize && isFull) {
            isFull = false;
            rnrSent = false;
            EV << "[Toc] Buffer not full => sending RR\n";
            sendControlMessage("RR", (expectedSeq - 1 + 8) % 8);
        }

        if (nCounter >= nValue) {
            sendControlMessage("RR", (expectedSeq - 1 + 8) % 8);
            nCounter = 0;
        }
    }

    virtual void handleMessage(cMessage *msg) override {
        if (DataPacket* dp = dynamic_cast<DataPacket*>(msg)) {
            if (uniform(0, 1) < errorRate) {
                EV << "[Toc] Packet seq=" << dp->seqNum << " lost!\n";
                delete dp;
                return;
            }

            if ((int)buffer.getLength() >= windowSize) {
                if (!rnrSent) {
                    EV << "[Toc] Buffer full => RNR => discarding seq=" << dp->seqNum << "\n";
                    sendControlMessage("RNR");
                    rnrSent = true;
                    isFull = true;
                }
                delete dp;
                return;
            }

            buffer.insert(dp);
            EV << "[Toc] Received seq=" << dp->seqNum
               << ", buffer size=" << buffer.getLength() << "\n";

            if ((int)buffer.getLength() == windowSize) {
                isFull = true;
                if (!rnrSent) {
                    sendControlMessage("RNR");
                    rnrSent = true;
                }
            }

            tryToExtractInOrder();
        }
        else if (ControlPacket* ctrl = dynamic_cast<ControlPacket*>(msg)) {
            if (!strcmp(ctrl->type, "QUERY_REQUEST")) {
                EV << "[Toc] Got QUERY_REQUEST => sending QUERY_REPLY(W="
                   << windowSize << ")\n";
                sendControlMessage("QUERY_REPLY");
            }
            delete ctrl;
        }
        else {
            EV << "[Toc] Unknown msg => discarding\n";
            delete msg;
        }
    }
};

Define_Module(Toc);
