#if !defined(AFX_INFO_H__CC17FB65_912B_474B_8D83_DB7AF9CD3AAB__INCLUDED_)
#define AFX_INFO_H__CC17FB65_912B_474B_8D83_DB7AF9CD3AAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Head.h"

// сохранение информации о захваченно пакете
class Info{
public:
	struct pcap_pkthdr *header;	// захват информации
	u_char *pkt_data; // заголовок пакета захвата
	HeadMain* m_Head; // PDU протокола самого низкого уровня
};

#endif // !defined(AFX_INFO_H__CC17FB65_912B_474B_8D83_DB7AF9CD3AAB__INCLUDED_)
