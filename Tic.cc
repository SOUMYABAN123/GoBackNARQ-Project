#include <omnetpp.h>
#include <queue>
#include <TicToc.h>

using namespace omnetpp;

class Tic : public cSimpleModule
{
  private:
    cMessage *timeoutEvent = nullptr;
    bool canSend = false;

    std::queue<int> sendBuffer;
    int baseSeq;
    int nextSeq;
    int windowSize;

    double dataRate;
    simtime_t timeoutInterval = 1.0;

  protected:
    virtual void initialize() override {
        baseSeq = 0;
        nextSeq = 0;
        windowSize = 0;
        canSend = false;

        dataRate = par("dataRate").doubleValue();
        timeoutEvent = new cMessage("timeoutEvent");

        ControlPacket *query = new ControlPacket("QUERY_REQUEST");
        send(query, "out");

        scheduleAt(simTime() + timeoutInterval, timeoutEvent);
    }

    void startSending() {
        canSend = true;
        scheduleAt(simTime(), new cMessage("sendTimer"));
    }

    void sendDataPacket() {
        if (!canSend)
            return;

        if ((int)sendBuffer.size() < windowSize) {
            DataPacket *pkt = new DataPacket(nextSeq);
            EV << "[Tic] Sending packet seq=" << nextSeq << "\n";
            send(pkt, "out");

            sendBuffer.push(nextSeq);
            nextSeq = (nextSeq + 1) % 8;

            if ((int)sendBuffer.size() < windowSize) {
                scheduleAt(simTime() + dataRate, new cMessage("sendTimer"));
            }
        }
    }

    void handleTimeout() {
        EV << "[Tic] TIMEOUT => Resending all unacknowledged packets\n";
        std::queue<int> tempQueue = sendBuffer;
        while (!tempQueue.empty()) {
            int seq = tempQueue.front();
            tempQueue.pop();

            DataPacket *dp = new DataPacket(seq);
            EV << "    Retransmit seq=" << seq << "\n";
            send(dp, "out");
        }

        if (timeoutEvent->isScheduled()) {
            cancelEvent(timeoutEvent);
        }
        scheduleAt(simTime() + timeoutInterval, timeoutEvent);
    }

    virtual void handleMessage(cMessage *msg) override {
        if (msg == timeoutEvent) {
            handleTimeout();
            return;
        }
        else if (msg->isName("sendTimer")) {
            sendDataPacket();
            delete msg;
            return;
        }

        if (ControlPacket *ctrl = dynamic_cast<ControlPacket*>(msg)) {
            const char* t = ctrl->type;

            if (!strcmp(t, "QUERY_REPLY")) {
                windowSize = ctrl->windowSize;
                EV << "[Tic] Got QUERY_REPLY => W=" << windowSize << "\n";

                if (timeoutEvent->isScheduled()) {
                    cancelEvent(timeoutEvent);
                }
                startSending();
            }
            else if (!strcmp(t, "RR")) {
                int ack = ctrl->ackNum;
                EV << "[Tic] Got RR => ackNum=" << ack << "\n";

                while (!sendBuffer.empty() && sendBuffer.front() != (ack + 1) % 8) {
                    sendBuffer.pop();
                    baseSeq = (baseSeq + 1) % 8;
                }
                if (!sendBuffer.empty() && sendBuffer.front() == (ack + 1) % 8) {
                    sendBuffer.pop();
                    baseSeq = (baseSeq + 1) % 8;
                }

                if (sendBuffer.empty()) {
                    if (timeoutEvent->isScheduled())
                        cancelEvent(timeoutEvent);
                } else {
                    if (timeoutEvent->isScheduled())
                        cancelEvent(timeoutEvent);
                    scheduleAt(simTime() + timeoutInterval, timeoutEvent);
                }

                canSend = true;
                scheduleAt(simTime(), new cMessage("sendTimer"));
            }
            else if (!strcmp(t, "RNR")) {
                EV << "[Tic] Got RNR => pausing sends\n";
                canSend = false;
            }

            delete ctrl;
        }
        else {
            EV << "[Tic] Unknown msg => discarding\n";
            delete msg;
        }
    }

    virtual ~Tic() {
        cancelAndDelete(timeoutEvent);
    }
};

Define_Module(Tic);
