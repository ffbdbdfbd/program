#include "stdafx.h"
#include "Head.h"
#include <iostream>
#include <string.h>

using namespace std;

extern int packet_kind;
extern unsigned char SourceIP[4];
extern unsigned char DestinationIP[4];
extern unsigned char SourceIPv6[16];
extern unsigned char DestinationIPv6[16];
extern int IPv6;	
extern int remain_len;

const CString TypeIP = "0800";
const CString TypeARP = "0806";
const CString TypeIPv6 = "86DD";

void Head_Ethernet::PackageAnalysis(u_char *pkt_data)
{
	CString m_kind;

	memcpy(DestinationMAC, pkt_data, 6);
	memcpy(SourceMAC, pkt_data + 6, 6);
	memcpy(Type, pkt_data + 12, 2);
	m_kind.Format("%02X%02X", Type[0], Type[1]);
	remain_len = remain_len - 14;
	if(m_kind.Compare(TypeIP) == 0)
	{
		next = new Head_IP();
		next->PackageAnalysis(pkt_data + 14);
	}
	else if(m_kind.Compare(TypeARP) == 0)
	{
		next = new Head_ARP();
		next->PackageAnalysis(pkt_data + 14);
	}
	else if(m_kind.Compare(TypeIPv6) == 0)
	{
		next = new Head_IPv6();
		next->PackageAnalysis(pkt_data + 14);
	}
	else
	{
		next = NULL;
		packet_kind = 0;
	}
}

CString Head_Ethernet::PrintData()
{
	CString thisSourceMAC;
	CString thisDestinationMAC;
	CString thisType;

	thisSourceMAC.Format("Source MAC: %02X-%02X-%02X-%02X-%02X-%02X\r\n", SourceMAC[0], SourceMAC[1], SourceMAC[2], SourceMAC[3], SourceMAC[4], SourceMAC[5]);
	thisDestinationMAC.Format("Destination MAC: %02X-%02X-%02X-%02X-%02X-%02X\r\n", DestinationMAC[0], DestinationMAC[1], DestinationMAC[2], DestinationMAC[3], DestinationMAC[4], DestinationMAC[5]);
	thisType.Format("Type IP: %02X%02X\r\n", Type[0], Type[1]);

	return "Ethernet II\r\n" + thisSourceMAC + thisDestinationMAC + thisType;
}

void Head_802_3::PackageAnalysis(u_char *pkt_data)
{
	CString m_kind;

	memcpy(DestinationMAC, pkt_data, 6);
	memcpy(SourceMAC, pkt_data + 6, 6);
	memcpy(Type, pkt_data + 20, 2);
	m_kind.Format("%02X%02X", Type[0], Type[1]);
	remain_len=remain_len-22;
	if(m_kind.Compare(TypeIP) == 0)
	{
		next=new Head_IP();
		next->PackageAnalysis(pkt_data + 22);
	}
	else if(m_kind.Compare(TypeARP) == 0)
	{
		next=new Head_ARP();
		next->PackageAnalysis(pkt_data + 22);
	}
	else if(m_kind.Compare(TypeIPv6) == 0)
	{
		next=new Head_IPv6();
		next->PackageAnalysis(pkt_data + 22);
	}
	else
	{
		next=NULL;
		packet_kind=1;
	}
}

CString Head_802_3::PrintData()
{
	CString thisSourceMAC;
	CString thisDestinationMAC;
	CString thisType;

	thisSourceMAC.Format("Source MAC: %02X-%02X-%02X-%02X-%02X-%02X\r\n", SourceMAC[0], SourceMAC[1], SourceMAC[2], SourceMAC[3], SourceMAC[4], SourceMAC[5]);
	thisDestinationMAC.Format("Destination MAC: %02X-%02X-%02X-%02X-%02X-%02X\r\n", DestinationMAC[0], DestinationMAC[1], DestinationMAC[2],DestinationMAC[3], DestinationMAC[4], DestinationMAC[5]);
	thisType.Format("Type IP: %02X%02X\r\n", Type[0], Type[1]);

	return "802.3\r\n" + thisSourceMAC + thisDestinationMAC + thisType;
}

void Head_ARP::PackageAnalysis(u_char *pkt_data)
{
	memcpy(Opcode, pkt_data + 6,2);
	memcpy(SourceMAC, pkt_data + 8,6);
	memcpy(Source_IP, pkt_data + 14,4);
	
	memcpy(DestinationMAC, pkt_data + 18,6);
	memcpy(Destination_IP, pkt_data + 24,4);

	memcpy(SourceIP, pkt_data + 14,4);
	memcpy(DestinationIP, pkt_data + 24,4);
	IPv6 = 0;
	next = NULL;
	packet_kind = 2;
}

CString Head_ARP::PrintData()
{
	CString thisOpcode;
	CString thisSourceMAC;
	CString thisDestinationMAC;
	CString thisSourceIP;
	CString thisDestinationIP;
	int mOP = 0;
	mOP = Opcode[0] << 8 | Opcode[1];
	thisOpcode.Format("Opcode: %d\r\n",mOP);
	thisSourceMAC.Format("Source MAC: %02X-%02X-%02X-%02X-%02X-%02X\r\n", SourceMAC[0], SourceMAC[1], SourceMAC[2], SourceMAC[3], SourceMAC[4], SourceMAC[5]);
    thisDestinationMAC.Format("Destination MAC: %02X-%02X-%02X-%02X-%02X-%02X\r\n", DestinationMAC[0], DestinationMAC[1], DestinationMAC[2], DestinationMAC[3], DestinationMAC[4], DestinationMAC[5]);
	thisSourceIP.Format("Source IP: %u.%u.%u.%u\r\n", Source_IP[0],Source_IP[1], Source_IP[2], Source_IP[3]);
	thisDestinationIP.Format("Destination IP: %u.%u.%u.%u\r\n", Destination_IP[0],Destination_IP[1], Destination_IP[2], Destination_IP[3]);
	
	return "ARP\r\n" + thisOpcode + thisSourceMAC + thisDestinationMAC + thisSourceIP + thisDestinationIP;
}

void Head_IP::PackageAnalysis(u_char *pkt_data)
{
	Version = (pkt_data[0] & 0xf0) >> 4;
	Length = pkt_data[0] & 0x0f;
	memcpy(sign, pkt_data+4, 2);
	TTL = pkt_data[8];
	Protocol = pkt_data[9] & 0xff;
	memcpy(Source_IP, pkt_data + 12, 4);
	memcpy(Destination_IP, pkt_data + 16, 4);
	memcpy(SourceIP, pkt_data + 12, 4);
	memcpy(DestinationIP,pkt_data + 16, 4);
	IPv6 = 0;
	remain_len = remain_len - Length * 4;

	if(remain_len == 0)
	{
		next = NULL;
		packet_kind = 3;
	}
	else if(Protocol == 1)
	{
		next = new Head_ICMP();
		next->PackageAnalysis(pkt_data + Length * 4);
	}
	else if(Protocol == 2)
	{
		next = new Head_IGMP();
		next->PackageAnalysis(pkt_data + Length * 4);
	}
	else if(Protocol == 6)
	{
		next = new Head_TCP();
		next->PackageAnalysis(pkt_data + Length * 4);
	}
	else if(Protocol == 17)
	{
		next = new Head_UDP();
		next->PackageAnalysis(pkt_data + Length * 4);
	}
	else
	{
		next = NULL;
		packet_kind = 3;
	}
}

CString Head_IP::PrintData()
{
	CString thisVersion;
	CString thisLength;
	CString thisIdentification;
	CString thisTTL;
	CString thisSourceIP;
	CString thisDestinationIP;
	int mSign = 0;
	mSign = sign[0] << 8 | sign[1];
	thisVersion.Format("Version: %d\r\n", Version);
    thisLength.Format("Length: %d\r\n", Length);
	thisIdentification.Format("Identification: %d\r\n", mSign);
	thisTTL.Format("TTL: %u\r\n", TTL);
	thisSourceIP.Format("Source IP: %u.%u.%u.%u\r\n", Source_IP[0], Source_IP[1], Source_IP[2], Source_IP[3]);
	thisDestinationIP.Format("Destination IP: %u.%u.%u.%u\r\n", Destination_IP[0], Destination_IP[1], Destination_IP[2], Destination_IP[3]);

	return "Internet Protocol (IP)\r\n" + thisVersion + thisLength + thisIdentification + thisTTL + thisSourceIP + thisDestinationIP;
}

void Head_ICMP::PackageAnalysis(u_char *pkt_data)
{
	Type = pkt_data[0] & 0xff;
	Code = pkt_data[1] & 0xff;

	next = NULL;
	packet_kind = 4;
}

CString Head_ICMP::PrintData()
{
	CString thisType;
	CString thisCode;

	thisType.Format("Type: %d\r\n", Type);
	thisCode.Format("Code: %d\r\n", Code);

	return "ICMP\r\n" + thisType + thisCode;
}

void Head_UDP::PackageAnalysis(u_char *pkt_data)
{
	memcpy(SourcePort, pkt_data, 2);
	memcpy(DestinationPort, pkt_data + 2, 2);
	
	int sPort = SourcePort[0] << 8 | SourcePort[1];
	int dPort = DestinationPort[0] << 8 | DestinationPort[1];

	if(sPort == 53 || dPort == 53)
	{
		next = new Head_DNS();
		next->PackageAnalysis(pkt_data+8);
	}
	else
	{
		next=NULL; 
		packet_kind=5;
	}
}

CString Head_UDP::PrintData()
{
	CString thisSourcePort;
	CString thisDestinationPort;
	int sPort=0,dPort=0;
	sPort = SourcePort[0] << 8 | SourcePort[1];
	dPort = DestinationPort[0] << 8 | DestinationPort[1];
	thisSourcePort.Format("Source Port: %d\r\n",sPort);
	thisDestinationPort.Format("Destination Port: %d\r\n",dPort);

	return "UDP\r\n" + thisSourcePort + thisDestinationPort;
}

void Head_IGMP::PackageAnalysis(u_char *pkt_data)
{
	Version = 3;
	Type = pkt_data[0];
	memcpy(MulticastAddress, pkt_data + 4, 4);

	next = NULL;
	packet_kind = 6;
}

CString Head_IGMP::PrintData()
{
	CString thisVersion;
	CString thisType;
	CString thisMulticastAddress;

	thisVersion.Format("Version: %d\r\n", Version);
	thisType.Format("Type: %u\r\n", Type);
	thisMulticastAddress.Format("DÀàIPµØÖ·: %u.%u.%u.%u\r\n", MulticastAddress[0], MulticastAddress[1], MulticastAddress[2], MulticastAddress[3]);
	
	return "IGMP\r\n" + thisVersion + thisType + thisMulticastAddress;
}

void Head_TCP::PackageAnalysis(u_char *pkt_data)
{
	memcpy(SourcePort, pkt_data, 2);
	memcpy(DestinationPort, pkt_data + 2, 2);
	memcpy(SYN, pkt_data + 4, 4);
	memcpy(ACK, pkt_data + 8, 4);
	memcpy(WindowSizeValue, pkt_data + 14, 2);
	
	int sPort = SourcePort[0] << 8 | SourcePort[1];
	int dPort = DestinationPort[0] << 8 | DestinationPort[1];
	Lenght = (pkt_data[12]& 0xf0) >> 4;
	remain_len = remain_len - Lenght * 4;
	if(remain_len == 0)
	{
		next = NULL;
		packet_kind = 7;
	}
	else if(sPort == 80 || dPort == 80)
	{
		next = new Head_HTTP();
		next->PackageAnalysis(pkt_data + Lenght * 4);
	}
	else if(sPort == 21 || dPort == 21)
	{
		next = new Head_FTP();
		next->PackageAnalysis(pkt_data + Lenght * 4);
	}
	else
	{
		next = NULL;
		packet_kind=  7;
	}
}

CString Head_TCP::PrintData()
{
	CString thisSourcePort;
	CString thisDestinationPort;
	CString thisSYN;
	CString thisACK;
	CString thisWindowSizeValue;
	CString thisLenght;

	int sPort = 0,dPort = 0;
	int mSYN = 0,mACK = 0;
	int mSize_Window;
	sPort = SourcePort[0] << 8 | SourcePort[1];
	dPort = DestinationPort[0] << 8 | DestinationPort[1];
	mSYN = SYN[0] << 24 | SYN[1] << 16 | SYN[2] << 8 | SYN[3];
	mACK = ACK[0] << 24 | ACK[1] << 16 | ACK[2] << 8 | ACK[3];
	mSize_Window = WindowSizeValue[0] << 8 | WindowSizeValue[1];
	
	
	thisSourcePort.Format("Source Port: %d\r\n", sPort);
	thisDestinationPort.Format("Destination Port: %d\r\n", dPort);
	//thisSYN.Format("SYN: %d\r\n", mSYN);
	//thisACK.Format("ACK: %d\r\n", mACK);
	thisWindowSizeValue.Format("Window size value: %d\r\n", mSize_Window);
	thisLenght.Format("Lenght: %d\r\n", Lenght * 4);

	return "TCP\r\n" + thisSourcePort + thisDestinationPort  + thisSYN + thisACK + thisWindowSizeValue + thisLenght;
}

void Head_IPv6::PackageAnalysis(u_char *pkt_data)
{
	Version = (pkt_data[0] & 0xf0) >> 4;
	Protocol = pkt_data[6] & 0xff;
	memcpy(S_IPv6, pkt_data + 8, 16);
	memcpy(D_IPv6, pkt_data + 24, 16);
	memcpy(SourceIPv6, pkt_data + 8, 16);
	memcpy(DestinationIPv6, pkt_data + 24, 16);
	IPv6 = 1;
	sign = 0;

	if(Protocol == 0)
	{
		Protocol = pkt_data[40] & 0xff;
		extern_len = pkt_data[41] & 0xff;
		sign = 1;
	}
	remain_len=remain_len-40;
	if(remain_len == 0)
	{
		next=NULL;
		packet_kind=8;
	}
	else if(Protocol == 6)
	{
		next= new Head_TCP();
		if(sign ==1)
			next->PackageAnalysis(pkt_data+40+8+extern_len*8);
		else
			next->PackageAnalysis(pkt_data+40);
	}
	else if(Protocol == 17)
	{
		next = new Head_UDP();
		if(sign == 1)
			next->PackageAnalysis(pkt_data+40+8+extern_len*8);
		else
			next->PackageAnalysis(pkt_data+40);
	}
	else if(Protocol == 58)
	{
		next = new Head_ICMP();
		if(sign == 1)
			next->PackageAnalysis(pkt_data+40+8+extern_len*8);
		else
			next->PackageAnalysis(pkt_data+40);
	}
	else if(Protocol == 2)
	{
		next = new Head_IGMP();
		if(sign == 1)
			next->PackageAnalysis(pkt_data+40+8+extern_len*8);
		else
			next->PackageAnalysis(pkt_data+40);
	}
	else if(Protocol ==89)
	{
		next = new Head_OSPF();
		if(sign == 1)
			next->PackageAnalysis(pkt_data+40+8+extern_len*8);
		else
			next->PackageAnalysis(pkt_data+40);
	}
	else
	{
		next = NULL;
		packet_kind = 8;
	}
}

CString Head_IPv6::PrintData()
{
	CString thisVersion;
	CString thisProtocol;
	CString thisSourceIP;
	CString thisDestinationIP;

	thisVersion.Format("Version: %d\r\n", Version);
	thisProtocol.Format("Protocol: %d\r\n", Protocol);
	thisSourceIP.Format("Source: %02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X\r\n",S_IPv6[0],S_IPv6[1],S_IPv6[2],S_IPv6[3],S_IPv6[4],S_IPv6[5],S_IPv6[6],S_IPv6[7],S_IPv6[8],S_IPv6[9],S_IPv6[10],S_IPv6[11],S_IPv6[12],S_IPv6[13],S_IPv6[14],S_IPv6[15]);
	thisDestinationIP.Format("Destination: %02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X\r\n",D_IPv6[0],D_IPv6[1],D_IPv6[2],D_IPv6[3],D_IPv6[4],D_IPv6[5],D_IPv6[6],D_IPv6[7],D_IPv6[8],D_IPv6[9],D_IPv6[10],D_IPv6[11],D_IPv6[12],D_IPv6[13],D_IPv6[14],D_IPv6[15]);
	
	return "IPv6\r\n" + thisVersion + thisProtocol + thisSourceIP + thisDestinationIP;
}

void Head_OSPF::PackageAnalysis(u_char *pkt_data)
{
	Version = pkt_data[0] & 0xff;
	Type = pkt_data[1] & 0xff;
	memcpy(RouterID, pkt_data + 4, 4);
	memcpy(AreaID, pkt_data + 8, 4);

	next = NULL;
	packet_kind = 9;
}

CString Head_OSPF::PrintData()
{
	CString thisVersion;
	CString thisType;
	CString thisRouterID;
	CString thisAreaID;

	thisVersion.Format("Version: %d\r\n", Version);
    thisType.Format("Type: %d\r\n", Type);
	thisRouterID.Format("Router ID: %u.%u.%u.%u\r\n", RouterID[0], RouterID[1], RouterID[2], RouterID[3]);
	thisAreaID.Format("Area ID: %u.%u.%u.%u\r\n", AreaID[0], AreaID[1], AreaID[2], AreaID[3]);

	return "OSPF\r\n" + thisVersion + thisType + thisRouterID + thisAreaID;
}

void Head_DNS::PackageAnalysis(u_char *pkt_data)
{
	memcpy(TransactionID, pkt_data, 2);
	Question = pkt_data[0] << 8 || pkt_data[1];

	next = NULL;
	packet_kind = 10;
}

CString Head_DNS::PrintData()
{
	CString thisTransactionID;
	CString thisQuestion;

	thisTransactionID.Format("Transaction ID: 0x%02x%02x\r\n", TransactionID[0], TransactionID[1]);
	thisQuestion.Format("Question: %d\r\n", Question);

	return "DNS\r\n" + thisTransactionID + thisQuestion;
}

void Head_HTTP::PackageAnalysis(u_char *pkt_data)
{
	next = NULL;
	packet_kind = 11;
}

CString Head_HTTP::PrintData()
{
	return "HTTP\r\n";
}

void Head_FTP::PackageAnalysis(u_char *pkt_data)
{
	next = NULL;
	packet_kind = 12;
}

CString Head_FTP::PrintData()
{
	return "FTP\r\n";
}
