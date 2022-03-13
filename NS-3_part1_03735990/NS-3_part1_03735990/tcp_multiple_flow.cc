/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * TODO Update this header
 */

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/tcp-header.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TCPMultiFlow");

// ===========================================================================
//
// Sender1 ----+				+ -------Receiver1
//             | 10Mbit/s, 20ms	|
//          Switch------------Switch
//             |				|
// Sender2-----+				+-------- Receiver2
//          50Mbit/s, 5ms
// ===========================================================================
//
class MyApp : public Application {
public:

    MyApp();

    virtual ~MyApp();

    void Setup(Ptr <Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate);

private:
    virtual void StartApplication(void);

    virtual void StopApplication(void);

    void ScheduleTx(void);

    void SendPacket(void);

    Ptr <Socket> m_socket;
    Address m_peer;
    uint32_t m_packetSize;
    uint32_t m_nPackets;
    DataRate m_dataRate;
    EventId m_sendEvent;
    bool m_running;
    uint32_t m_packetsSent;
};

MyApp::MyApp()
        : m_socket(0),
          m_peer(),
          m_packetSize(0),
          m_nPackets(0),
          m_dataRate(0),
          m_sendEvent(),
          m_running(false),
          m_packetsSent(0) {
}

MyApp::~MyApp() {
    m_socket = 0;
}

void
MyApp::Setup(Ptr <Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate) {
    m_socket = socket;
    m_peer = address;
    m_packetSize = packetSize;
    m_nPackets = nPackets;
    m_dataRate = dataRate;
}

void
MyApp::StartApplication(void) {
    m_running = true;
    m_packetsSent = 0;
    m_socket->Bind();
    m_socket->Connect(m_peer);
    SendPacket();
}

void
MyApp::StopApplication(void) {
    m_running = false;

    if (m_sendEvent.IsRunning()) {
        Simulator::Cancel(m_sendEvent);
    }

    if (m_socket) {
        m_socket->Close();
    }
}

void
MyApp::SendPacket(void) {
    Ptr <Packet> packet = Create<Packet>(m_packetSize);
    m_socket->Send(packet);

    if (m_nPackets == 0 || (++m_packetsSent < m_nPackets)) {
        ScheduleTx();
    }
}

void
MyApp::ScheduleTx(void) {
    if (m_running) {
        Time tNext(Seconds(m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate())));
        m_sendEvent = Simulator::Schedule(tNext, &MyApp::SendPacket, this);
    }
}

// Trace functions
static void
CwndChange(Ptr <OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd) {
    *stream->GetStream() << Simulator::Now().GetSeconds() << "\t" << oldCwnd << "\t" << newCwnd << std::endl;
}

// TODO uncomment for Task 7.2.5
/*
static void
ChangeDelay(std::string newDelay, std::string channelId) {
    std::cout << Simulator::Now().GetSeconds() << " Setting new delay." << std::endl;
    std::string specificNode = "/ChannelList/" + channelId + "/$ns3::PointToPointChannel/Delay";
    Config::Set(specificNode, StringValue(newDelay));
}
*/

int
main(int argc, char *argv[]) {
    std::string experiment_name = "both_new_reno";
    uint32_t run = 0;
    std::string tcp_variant = "TcpNewReno";

    CommandLine cmd;
    cmd.AddValue("expname", "Experiment name used as basename for trace files.", experiment_name);
    cmd.AddValue("run", "Run index (for setting repeatable seeds)", run);
    cmd.AddValue("tcpvariant", "TCP Variant used for simulations", tcp_variant);
    cmd.Parse(argc, argv);

    SeedManager::SetSeed(1);
    SeedManager::SetRun(run);


    /* -------- TOPOLOGY --------
     * Add the topology here
     */
    NodeContainer nodes;
    nodes.Create(6);

    NodeContainer tx1s1 = NodeContainer (nodes.Get (0), nodes.Get (2));
    NodeContainer tx2s1 = NodeContainer (nodes.Get (1), nodes.Get (2));
    NodeContainer s1s2 = NodeContainer (nodes.Get (2), nodes.Get (3));
    NodeContainer rx1s2 = NodeContainer (nodes.Get (3), nodes.Get (4));
    NodeContainer rx2s2 = NodeContainer (nodes.Get (3), nodes.Get (5));

    // We create the channels first without any IP addressing information
    PointToPointHelper p2p;
    // Create the bottleneck link with correct link rate and delay
    // <---- TODO  Task 7.2.1: Set link rate and delay---- >
    p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
    //p2p.SetChannelAttribute ("Delay", StringValue ("20ms"));
    // Task 7.2.6
    p2p.SetChannelAttribute ("Delay", StringValue ("5ms"));

    NetDeviceContainer devices_s1s2 = p2p.Install (s1s2);

    // Create channels for the other links with correct link rate and delay
    // < ---- TODO  Task 7.2.1: Set link rate, delay and install links ---- >
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("50Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("5ms"));

    NetDeviceContainer devices_tx1s1 = pointToPoint.Install(tx1s1);
    NetDeviceContainer devices_tx2s1 = pointToPoint.Install(tx2s1);
    NetDeviceContainer devices_rx1s2 = pointToPoint.Install(rx1s2);
    NetDeviceContainer devices_rx2s2 = pointToPoint.Install(rx2s2);

    /* -------- END TOPOLOGY -------- */

    /* -------- IP and transport layer ------- */

    // Set TCP version before any IP related thing
    // < ---- TODO  Task 7.2.2:  Set TCP version ---- >
    std::stringstream tcp_variant_fullname;
    tcp_variant_fullname << "ns3::"  << tcp_variant;
    Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue (tcp_variant_fullname.str()));

    // Install IP stack and set addresses
    InternetStackHelper internet;
    // Nodes is a NodeContainer with all nodes
    internet.Install (nodes);
    // Later, we add IP addresses.
    NS_LOG_INFO ("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");

    Ipv4InterfaceContainer if_tx1s1 = ipv4.Assign (devices_tx1s1);
    ipv4.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer if_tx2s1 = ipv4.Assign (devices_tx2s1);
    ipv4.SetBase ("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer if_s1s2 = ipv4.Assign (devices_s1s2);
    ipv4.SetBase ("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer if_rx1s2 = ipv4.Assign (devices_rx1s2);
    ipv4.SetBase ("10.1.5.0", "255.255.255.0");
    Ipv4InterfaceContainer if_rx2s2 = ipv4.Assign (devices_rx2s2);

    // Create router nodes, initialize routing database and set up the routing
    // tables in the nodes.
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    /* -------- END IP and transport layer ------- */

    /* -------- Flows -------- */
    uint16_t sinkPort = 8080;

    // ------- Flow 1 --------
    Address sinkAddress(InetSocketAddress(if_rx1s2.GetAddress(1), sinkPort));
    // < ---- TODO  Task 7.2.2: Create sockets and app for flow 1 and install on node---- >
    PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
    ApplicationContainer sinkApps = packetSinkHelper.Install ( nodes.Get (4));
    sinkApps.Start (Seconds (0.));
    sinkApps.Stop (Seconds (60.));
    Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (nodes.Get (0), TcpSocketFactory::GetTypeId ());
    

    Ptr<MyApp> appFlow1 = CreateObject<MyApp> ();
    appFlow1->Setup (ns3TcpSocket, sinkAddress, 1040, 0, DataRate ("20Mbps"));
    nodes.Get (0)->AddApplication (appFlow1);
    appFlow1->SetStartTime (Seconds (1.));
    appFlow1->SetStopTime (Seconds (60.));


    // -------- Flow 2 --------
    Address sinkAddress2(InetSocketAddress(if_rx2s2.GetAddress(1), sinkPort));
    // < ---- TODO  Task 7.2.2: Create sockets and app for flow 2 and install on node ---- >
    PacketSinkHelper packetSinkHelper2 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
    ApplicationContainer sinkApps2 = packetSinkHelper2.Install ( nodes.Get (5));
    sinkApps.Start (Seconds (0.));
    sinkApps.Stop (Seconds (60.));
    Ptr<Socket> ns3TcpSocket2 = Socket::CreateSocket (nodes.Get (1), TcpSocketFactory::GetTypeId ());
   
    Ptr<MyApp> appFlow2 = CreateObject<MyApp> ();
    appFlow2->Setup (ns3TcpSocket2, sinkAddress2, 1040, 0, DataRate ("20Mbps"));
    nodes.Get (1)->AddApplication (appFlow2);
    appFlow2->SetStartTime (Seconds (11.));
    appFlow2->SetStopTime (Seconds (60.));

    // < ---- TODO BONUS Task 7.2.6: Add UDP flow ---- >
    OnOffHelper server("ns3::UdpSocketFactory", sinkAddress);
    server.SetConstantRate (DataRate ("10Mbps"));

    ApplicationContainer appUdp = server.Install (nodes.Get (0));
    appUdp.Start(Seconds (30.));
    appUdp.Stop(Seconds (60.));

    PacketSinkHelper client("ns3::UdpSocketFactory", sinkAddress);
    appUdp = client.Install (nodes.Get(4));
    appUdp.Start (Seconds (31.0));
    appUdp.Stop (Seconds (60));

    /* -------- END Flows -------- */

    /* -------- Traces and Output -------- */
    // < ---- TODO  Task 7.2.2: Add traces and output writer ---- >
    std::stringstream fname_cwnd;
    fname_cwnd << "multiple_tcp_flow1_" << experiment_name << ".cwnd";
    AsciiTraceHelper asciiTraceHelper;
    Ptr <OutputStreamWrapper> streamCwnd = asciiTraceHelper.CreateFileStream(fname_cwnd.str());
    ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CwndChange, streamCwnd));
    std::stringstream fname2_cwnd;
    fname2_cwnd << "multiple_tcp_flow2_" << experiment_name << ".cwnd";
    Ptr <OutputStreamWrapper> streamCwnd2 = asciiTraceHelper.CreateFileStream(fname2_cwnd.str());
    ns3TcpSocket2->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CwndChange, streamCwnd2));

    // < ---- TODO  BONUS Task 7.2.4: Measure flow throughput over time ---- >

    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    // Increase the delay of the link to receiver 1
    // < ---- TODO  Task 7.2.5: Increase the delay of link to Receiver 1---- >
    //Simulator::Schedule (Seconds(20.0),&ChangeDelay, "20ms", "devices_rx1s2");

    // Run simulation and stop after 60s
    // < ---- TODO  Task 7.2.3: run simulation ---- >
    Simulator::Stop (Seconds (60));
    Simulator::Run ();

    std::stringstream fname_flowmon;
    fname_flowmon << "multi_tcp_" << experiment_name << ".flowmonitor";
    flowMonitor->SerializeToXmlFile(fname_flowmon.str(), true, true);

    Simulator::Destroy();

    return 0;
}

