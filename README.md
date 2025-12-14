## PROJECT -1
# A SIMULATION REPORT ON GO BACK N ARQ USING OMNET++

# Abstract:

Ensuring dependable data transmission across unreliable or noisy networks remains a key concern in the design of communication protocols. This study offers a simulation-driven assessment of the Go-Back-N ARQ (Automatic Repeat reQuest) protocol, utilizing the Sliding Window approach to investigate its behavior under diverse error conditions. Go-Back-N allows the sender to dispatch several frames consecutively within a defined window size, without waiting for individual acknowledgments. While this boosts transmission efficiency, any detection of a missing or corrupted frame triggers the retransmission of that frame and all subsequent ones, potentially reducing bandwidth utilization.
The simulation, built using the OMNeT++ framework, replicates the dynamics between sender and receiver, including frame numbering, acknowledgment processing, timeout handling, and retransmission strategies. The Sliding Window protocol facilitates flow control and guarantees sequential delivery of frames. Key performance indicators—such as data throughput, frequency of retransmissions, and overall channel efficiency—are evaluated across varying window sizes and simulated error rates.
Findings underscore the protocol’s reliability and straightforward implementation, while also exposing its drawbacks in environments with frequent transmission errors. The insights gained from this analysis lay the groundwork for future exploration of more advanced error-handling protocols like Selective Repeat ARQ and Hybrid ARQ.

# Introduction:

Reliable data transmission is a key requirement in computer networks, especially when operating over noisy or error-prone channels. To ensure that data arrives correctly and in order, various error control protocols have been developed. One widely used method is Go-Back-N Automatic Repeat reQuest (ARQ), a type of sliding window protocol that balances simplicity with performance.
In Go-Back-N ARQ, the sender can transmit multiple frames before waiting for acknowledgments (ACKs) from the receiver. If a frame is lost or corrupted, the sender retransmits that frame along with all subsequent frames in the window. This mechanism ensures reliability but may lead to bandwidth inefficiencies when errors are frequent.
This project presents a simulation-based study of the Go-Back-N ARQ protocol using OMNeT++, a modular network simulation framework. The simulation models two key components: Tic (sender) and Toc (receiver). The sender transmits packets using a configurable data rate, and the receiver processes them based on a defined window size (W) and acknowledgment threshold (N). The receiver sends an ACK after receiving N packets, where N is less than or equal to W. The simulation also introduces random packet loss using exponential or uniform distributions to mimic real-world channel errors.
To maintain protocol correctness, each packet includes a sequence number field limited to either 3 bits or 8 bits, allowing for a maximum of 8 or 256 unique sequence numbers before wraparound. These parameters—data rate, window size, ACK frequency, error rate, and sequence number size—are all configurable via the omnetpp.ini file, enabling flexible experimentation.
The goal of this study is to evaluate the performance of Go-Back-N ARQ under varying network conditions. Key metrics such as throughput, retransmission count, and channel efficiency are analyzed to understand the protocol’s behavior and limitations. The results offer insights into how Go-Back-N performs in realistic scenarios and provide a foundation for exploring more advanced protocols like Selective Repeat ARQ.

Here we can observe how a Go-Back-N Protocol works based on the pictorial representation for window size 4.

# Methodology:

This simulation models the Go-Back-N ARQ protocol using OMNeT++, a discrete event simulator for network systems. The protocol is implemented using two modules: Sender (Tic) and Receiver (Toc). The communication between them follows the Go-Back-N logic, where multiple frames are sent in a window, and lost or corrupted frames trigger retransmissions.
# Simulation Setup
•	Tool Used: OMNeT++ (with INET framework)
•	Modules: Tic (sender) and Toc (receiver)
•	Protocol: Go-Back-N ARQ with sliding window
•	Parameters:
o	Window size (N)
o	Timeout interval
o	Error probability
o	Frame sequence numbers
# Process Flow
1. Sender Initialization
•	The sender starts by generating a sequence of data frames.
•	It maintains a sliding window of size N.
•	Frames within the window are sent without waiting for ACKs.
2. Frame Transmission
•	Each frame is tagged with a sequence number.
•	Frames are sent to the receiver one by one until the window is full.
3. Receiver Behavior
•	The receiver checks each incoming frame:
o	If the frame is correct and in order, it sends an ACK.
o	If the frame is corrupted or out of order, it discards it and does not send an ACK.
4. Timeout and Retransmission
•	The sender starts a timer after sending each frame.
•	If an ACK is not received before the timeout:
o	The sender goes back to the last unacknowledged frame.
o	It retransmits that frame and all subsequent frames in the window.
5. Sliding Window Update
•	When ACKs are received:
o	The sender slides the window forward.
o	New frames are sent to fill the window.
6. Simulation End
•	The process continues until all frames are successfully acknowledged.
•	Statistics are collected on:
o	Total frames sent
o	Number of retransmissions
o	Throughput
o	Channel efficiency
Output Metrics
•	Throughput: Number of successfully delivered frames per unit time
•	Retransmission Count: Total number of frames resent due to errors
•	Efficiency: Ratio of useful data to total transmitted data

# Message Sequence Chart:
<img width="977" height="423" alt="image" src="https://github.com/user-attachments/assets/5401ad67-e81c-49c4-aca4-b351339b70de" />


# Message Sequence Chart: Go-Back-N ARQ Protocol:
<img width="871" height="452" alt="image" src="https://github.com/user-attachments/assets/4786ba4d-8150-4659-82ef-ebfd7d7804d8" />

Explanation:
<img width="628" height="705" alt="image" src="https://github.com/user-attachments/assets/4844b7b6-bb44-4009-990d-05f5770a8369" />

It’s a chronological interaction between two modules:
•	Tic (Sender): Sends data packets and handles retransmissions.
•	Toc (Receiver): Receives packets, buffers them, and sends control messages like ACKs (RR) or flow control (RNR).
Each row in the chart shows a message being sent or received, and how each module reacts.
Initialization Phase
Tic sends QUERY_REQUEST → Toc
•	This is the sender asking: “How many packets can I send at once?”
•	It’s a handshake to establish the receiver’s window size.
Toc replies with QUERY_REPLY(W=7)
•	Receiver says: “You can send up to 7 packets before waiting for ACKs.”
•	This sets the sender’s windowSize = 7.
Data Transmission Phase
Tic sends packets seq=0 to seq=7
•	These are the first 8 packets sent, one at a time.
•	Each packet is pushed into sendBuffer and tracked for acknowledgment.
Toc receives and buffers each packet
•	It checks if the packet is in order (seqNum == expectedSeq).
•	If yes, it accepts and increments expectedSeq.
•	After receiving 7 packets, it sends an ACK (RR) with ackNum=7.
ACK Handling
Tic receives RR(ackNum=7)
•	It pops all acknowledged packets from sendBuffer.
•	Updates baseSeq to reflect the new window start.
•	Cancels the timeout timer since all packets were acknowledged.

TimeOut and Retransmission Scenario:
# Explanation:

Next Round Begins
Tic sends packets seq=0 to seq=4 again (wraparound)
•	Sequence numbers wrap around due to %8 logic.
•	Receiver continues buffering and extracting in-order packets.
Toc sends another RR(ackNum=4) after receiving 4 in-order packets
•	This is based on nValue = 4 from your .ini file.
•	ACKs are sent every 4 packets to reduce control overhead.
Timeout & Retransmission
If a packet is lost (e.g., seq=2), and no ACK is received:
•	Tic's timeoutEvent triggers.
•	It resends all unacknowledged packets from baseSeq onward.
•	This is the core of Go-Back-N: resend everything from the first missing packet.
Flow Control
If Toc's buffer is full:
•	It sends RNR (Receive Not Ready).
•	Tic sets canSend = false and pauses transmission.
•	Once buffer space is available, Toc sends RR to resume flow.

# Simplified Flow Chart:

 

# This Flow Chart Demonstrates

Concept	Role in Chart
Sliding Window	Controls how many packets are sent before waiting for ACKs
ACK Handling (RR)	Confirms successful delivery of packets
Flow Control (RNR)	Prevents buffer overflow at receiver
Timeout & Retransmit	Ensures reliability when packets or ACKs are lost
Sequence Numbers	Maintains order and detects missing frames

# Test Parameter:

 

# Simulation Output:
Event #1: QUERY_REQUEST
•	t=0.1: The receiver (Toc) receives a QUERY_REQUEST from the sender (Tic).
•	This is a control message asking: “What’s your window size?”
QUERY_REPLY (w=7)
•	t=0.2: Toc replies with a QUERY_REPLY, stating its window size is 7.
•	This means Tic can send up to 7 packets before needing an acknowledgment.
Sending Packets
•	t=0.2: Tic sends packet with sequence number 0.
•	t=0.3: A timer (sendTimer) is triggered internally, prompting Tic to send packet with sequence number 1.
•	These are the first two data packets in the transmission.
Receiving Packets
•	t=0.4: Toc receives packet seq=0.
o	It adds it to the buffer (buffer size becomes 1).
o	Then it accepts the in-order packet, reducing buffer size back to 0.
•	t=0.5: Toc receives packet seq=1.
o	Same process: buffer size goes to 1, then back to 0 after accepting the packet.
Key Events in the below Log
Packet Reception
•	"Received Packet with seq=7, buffer=7"
o	The receiver successfully receives packet #7.
o	Its buffer size is 7, meaning it can hold 7 packets at a time.
Timestamped Function Calls
•	"t=4.1 GoBackN.tic(Toc, Tic, 5)"
•	"t=5.4 GoBackN.tic(Toc, Tic, 6)"
•	These entries show the sender (Tic) transmitting packets to the receiver (Toc) at specific times.
•	The numbers (5, 6, etc.) are the sequence numbers of the packets being sent.
Receiver Not Ready (RNR)
•	"Received RNR. Pausing sends."
o	The receiver signals that it cannot accept more packets—likely because its buffer is full.
o	The sender halts transmission until the receiver becomes ready again.
Timeout Event
•	"t=6.1 Timeout. Retransmitting all unacknowledged packets."
o	The sender waited for an acknowledgment but didn’t receive one in time.
o	This triggers a Go-Back-N retransmission, where the sender resends all packets from the last unacknowledged one onward.
Retransmission
•	"Retransmitting: 5, 6, 7, 0, 1"
o	The sender resends packets starting from #5 through #1.
o	This includes:
	Packets that were sent but not acknowledged.
	Possibly packets that were lost or corrupted.
	Packet #0 and #1 might be part of a wrap-around if the sequence numbers are modulo-based.
# Output Logs and Graphs:
<img width="617" height="337" alt="image" src="https://github.com/user-attachments/assets/26319a62-698f-4cba-bed0-d7457f4acba4" />

Top Panel: Event Timeline
•	This is a sequence chart showing how packets and control messages flow between modules over time.
•	X-axis: Time (in milliseconds).
•	Y-axis: Modules (e.g., Tic, Toc, GoBackN).
•	Arrows:
o	Blue arrows labeled sendTimer: Packet transmissions triggered by timers.
o	Purple arrow labeled QUERY REPLY: Control message from receiver to sender.
o	Other arrows may show retransmissions, ACKs, or dropped packets.
Bottom Panel: Simulation Log
•	This is the console output from OMNeT++ during simulation.
•	It logs:
o	Module initialization (GoBackN, TGoBackN.tx, etc.).
o	Gate creation (used for connecting modules).
o	Event handling (e.g., packet sent, received, dropped).

# Conclusion:
This project successfully simulated the Go-Back-N Automatic Repeat reQuest (ARQ) protocol using the OMNeT++ network simulation framework. By implementing sender and receiver modules with configurable parameters such as window size, data rate, ACK frequency, and packet loss probability, the simulation provided a clear understanding of how Go-Back-N handles reliable data transmission over unreliable channels.
The results demonstrated the protocol’s ability to maintain in-order delivery and error recovery through retransmissions triggered by timeouts and missing acknowledgments. While Go-Back-N offers simplicity and robustness, the simulation also highlighted its limitations—particularly the inefficiency caused by retransmitting multiple packets even when only one is lost.
Overall, this study reinforces the importance of sliding window protocols in network communication and lays the groundwork for future exploration of more advanced techniques such as Selective Repeat ARQ and Hybrid ARQ, which aim to improve bandwidth utilization and reduce unnecessary retransmissions.












































