#if !defined(AFX_INFO_H__CC17FB65_912B_474B_8D83_DB7AF9CD3AAB__INCLUDED_)
#define AFX_INFO_H__CC17FB65_912B_474B_8D83_DB7AF9CD3AAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Head.h"

// ���������� ���������� � ���������� ������
class Info{
public:
	struct pcap_pkthdr *header;	// ������ ����������
	u_char *pkt_data; // ��������� ������ �������
	HeadMain* m_Head; // PDU ��������� ������ ������� ������
};

#endif // !defined(AFX_INFO_H__CC17FB65_912B_474B_8D83_DB7AF9CD3AAB__INCLUDED_)
