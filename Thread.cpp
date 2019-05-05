#include "stdafx.h"
#include "Thread.h"
#include "Application.h"
#include <pcap.h>

// pcap_loop копирует пакет и захватывает сообщение
static void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	struct pcap_pkthdr *new_header = new struct pcap_pkthdr;
	u_char *new_data = new u_char[header->len];

	if(param) // если параметр не NULL, то он хранится в файле
		pcap_dump(param, header, pkt_data);

	// копируем данные, отправляем сообщение
	*new_header = *header;
	memcpy(new_data, pkt_data, header->len);
	::PostMessage(App.displayHWND, WM_TCATCH, (WPARAM)new_header, (LPARAM)new_data);
}

// захват реализации потока
UINT ThreadProc(LPVOID param)
{
	ThreadParam *p = (ThreadParam*)param;

	// цикл захватывает пакет, pcap_breakloop прерывает цмкл
	int loopreturn = pcap_loop(p->m_dev, 0, packet_handler, (unsigned char*)p->m_dumper);
	delete p;

	// перед выходом вывести сообщение WM_TEXIT
	::PostMessage(App.controlHWND, WM_TEXIT, loopreturn, NULL);

	return 0;
}

