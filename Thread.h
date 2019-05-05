#if !defined(AFX_THREAD_H__40FDE7DB_B292_4BB9_BF5A_29222ACAEC99__INCLUDED_)
#define AFX_THREAD_H__40FDE7DB_B292_4BB9_BF5A_29222ACAEC99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_TCATCH	WM_USER + 100
#define WM_TEXIT	WM_USER + 101

// оператор потока захвата
UINT ThreadProc(LPVOID param);

#include <pcap.h>

class ThreadParam{
public:
	pcap_t *m_dev; // контролируемое сетевое устройство
	pcap_dumper_t *m_dumper; // файл для сохранения информации дампа

	ThreadParam(pcap_t *dev, pcap_dumper_t *dumper)
	{
		m_dev = dev;
		m_dumper = dumper;
	}
};

#endif // !defined(AFX_THREAD_H__40FDE7DB_B292_4BB9_BF5A_29222ACAEC99__INCLUDED_)
