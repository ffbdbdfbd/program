#if !defined(AFX_HEAD_H__A94CE23F_F01E_48CA_8FBA_997EA55E23CD__INCLUDED_)
#define AFX_HEAD_H__A94CE23F_F01E_48CA_8FBA_997EA55E23CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <pcap.h>

// главный класс
class HeadMain{
public:
	// протокол более высокого уровня
	HeadMain* next;

	// получение информации из PDU
	virtual void PackageAnalysis(u_char *pkt_data) = 0;

	// вывод на экран
	virtual CString PrintData() = 0;
};

class Head_Ethernet : public HeadMain{
public:
	u_char SourceMAC[6];
	u_char DestinationMAC[6];
	u_char Type[2];   //0800:IP  0806:ARP
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_802_3 : public HeadMain{
public:
	u_char SourceMAC[6];
	u_char DestinationMAC[6];
	u_char Type[2];   //0800:IP  0806:ARP
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_ARP : public HeadMain{
public:
	u_char Opcode[2];  //require or answer
	u_char SourceMAC[6];
	u_char DestinationMAC[6];
	u_char Source_IP[4];
	u_char Destination_IP[4];
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_IP : public HeadMain{
public:
	int Version;
	int Length;
	u_char sign[2];
	u_char TTL;
	int Protocol;
	u_char Source_IP[4];
	u_char Destination_IP[4];
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_ICMP : public HeadMain{
public:
	int Type;
	int Code;
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_UDP : public HeadMain{
public:
	u_char SourcePort[2];
	u_char DestinationPort[2];
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_IGMP : public HeadMain{
public:
	int Version;
	u_char Type;
	u_char MulticastAddress[4];
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_TCP : public HeadMain{
public:
	u_char SourcePort[2];
	u_char DestinationPort[2];
	u_char SYN[4];
	u_char ACK[4];
	u_char WindowSizeValue[2];
	int Lenght;
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_IPv6 : public HeadMain{
public:
	int Version;
	int Protocol;
	int extern_len;
	int sign;
	u_char S_IPv6[16];
	u_char D_IPv6[16];
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_OSPF : public HeadMain{
public:
	int Version;
	int Type;
	u_char RouterID[4];
	u_char AreaID[4];
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_DNS : public HeadMain{
public:
	u_char TransactionID[2];
	int Question;
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_HTTP : public HeadMain{
public:
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

class Head_FTP : public HeadMain{
public:
	void PackageAnalysis(u_char *pkt_data);
	CString PrintData();
};

#endif // !defined(AFX_HEAD_H__A94CE23F_F01E_48CA_8FBA_997EA55E23CD__INCLUDED_)
