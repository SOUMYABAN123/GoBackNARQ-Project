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
//ned

simple Tic
{
    parameters:
        volatile double dataRate @unit("s");    // Rate in seconds/packet
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

simple Toc
{
    parameters:
        int windowSize;        // Receiver buffer size
        int nValue;            // Number of packets after which we ACK
        double errorRate;      // Probability of random loss
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

network GoBackN
{
    @display("bgb=475,289");
    submodules:
        tic: Tic {
            dataRate = default(0.2s);
            @display("p=126,142");
        }
        toc: Toc {
            windowSize = default(4);
            nValue = default(2);
            errorRate = default(0.1);
            @display("p=345,142");
        }

    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        toc.out --> {  delay = 100ms; } --> tic.in;
}
