#include "stdafx.h"
#include "TableView.h"
#include "Application.h"
#include "Thread.h"
#include "Info.h"
#include "Head.h"

extern int count;
extern int packet_kind;
extern unsigned char SourceIP[4];
extern unsigned char DestinationIP[4];
extern unsigned char SourceIPv6[16];
extern unsigned char DestinationIPv6[16];
extern int IPv6;	
extern int remain_len;
//extern CString filter;
CString filter = "sourceip:192.168.0.1:destinationip:192.168.0.103:protocol:tcp";
// protocol:tcp
// sourceip:192.168.0.1
// sourceip:192.168.0.1:destinationip:192.168.0.103
// sourceip:192.168.0.1:destinationip:192.168.0.103:protocol:tcp

IMPLEMENT_DYNCREATE(CTableView, CListView)

CTableView::CTableView()
{
	App.tableView = this;
}

CTableView::~CTableView() {}

BEGIN_MESSAGE_MAP(CTableView, CListView)
	//{{AFX_MSG_MAP(CTableView)
	ON_MESSAGE(WM_TCATCH, OnTCatch)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTableView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}
// когда поток данных перехватил пакет, pkt_data является захваченным пакетом
void CTableView::OnTCatch(struct pcap_pkthdr *header, u_char *pkt_data)
{
	struct tm *ltime; // структура времени
	char timestr[16]; // хранение времени
	CString line_num,m_len,m_smac,m_dmac; // преобразование выходного формата
	CString m_kind; // тип записи
	CString m_cntl;	// если это кадр 802.3
	CString m_packetkind; // тип пакета
	CString sourceIp; // ip источника
	CString DestinationIp; // ip назачения
	Info temp_info;	// добавление захваченных пакетов
	u_char SourceMAC[6]; // источник MAC
	u_char DestinationMAC[6]; // назначение MAC
	u_char Kind[2];	// сохранение типа кадра Ethernet
	u_char Cntl[1];	// если это 802.3, то должно быть 03
	remain_len = header->len;	// инициализация
	temp_info.header = header; // пакет хранения
	temp_info.pkt_data = pkt_data;

	// время получения заголовка и информация о длине пакета
	ltime = localtime(&header->ts.tv_sec);
	strftime(timestr, sizeof(timestr), "%H:%M:%S", ltime);
	line_num.Format("%d", count++); // запись номера пакета
	m_len.Format("%d",header->len);

	// детали пакета
	memcpy(DestinationMAC, pkt_data, 6);
	memcpy(SourceMAC, pkt_data + 6, 6);
	memcpy(Kind, pkt_data + 12, 2);
	memcpy(Cntl, pkt_data + 16, 1);
	m_dmac.Format("%02X-%02X-%02X-%02X-%02X-%02X",DestinationMAC[0],DestinationMAC[1],DestinationMAC[2],DestinationMAC[3],DestinationMAC[4],DestinationMAC[5]);
	m_smac.Format("%02X-%02X-%02X-%02X-%02X-%02X",SourceMAC[0],SourceMAC[1],SourceMAC[2],SourceMAC[3],SourceMAC[4],SourceMAC[5]);
	m_kind.Format("%02X%02X",Kind[0], Kind[1]);
	m_cntl.Format("%02X", Cntl[0]);

	// анализ пакета	
	if(m_kind.Compare("0800") == 0 || m_kind.Compare("0806") == 0 || m_kind.Compare("86DD") == 0) {
		temp_info.m_Head = new Head_Ethernet();
		temp_info.m_Head->PackageAnalysis(pkt_data);
	}
	else {
		temp_info.m_Head = new Head_802_3();
		temp_info.m_Head->PackageAnalysis(pkt_data);
	}
	
	m_info.push_back(temp_info);

	if(IPv6 == 0) {
		sourceIp.Format("%u.%u.%u.%u",SourceIP[0], SourceIP[1], SourceIP[2], SourceIP[3]);
		DestinationIp.Format("%u.%u.%u.%u",DestinationIP[0], DestinationIP[1], DestinationIP[2], DestinationIP[3]);
	}
	else {
		sourceIp.Format("%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X", SourceIPv6[0], SourceIPv6[1], SourceIPv6[2], SourceIPv6[3], SourceIPv6[4], SourceIPv6[5], SourceIPv6[6], SourceIPv6[7],SourceIPv6[8], SourceIPv6[9], SourceIPv6[10],SourceIPv6[11], SourceIPv6[12], SourceIPv6[13], SourceIPv6[14], SourceIPv6[15]);
		DestinationIp.Format("%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X", DestinationIPv6[0], DestinationIPv6[1], DestinationIPv6[2], DestinationIPv6[3], DestinationIPv6[4], DestinationIPv6[5],DestinationIPv6[6], DestinationIPv6[7], DestinationIPv6[8], DestinationIPv6[9], DestinationIPv6[10], DestinationIPv6[11], DestinationIPv6[12], DestinationIPv6[13], DestinationIPv6[14], DestinationIPv6[15]);
	}
	switch(packet_kind){
	case 0:m_packetkind = "Ethernet"; break;
	case 1:m_packetkind = "802.3"; break;
	case 2:m_packetkind = "ARP"; break;
	case 3:m_packetkind = "IP"; break;
	case 4:m_packetkind = "ICMPv6"; break;
	case 5:m_packetkind = "UDP"; break;
	case 6:m_packetkind = "IGMPv3"; break;
	case 7:m_packetkind = "TCP"; break;
	case 8:m_packetkind = "IPv6"; break;
	case 9:m_packetkind = "OSPF"; break;
	case 10:m_packetkind = "DNS"; break;
	case 11:m_packetkind = "HTTP"; break;
	case 12:m_packetkind = "FTP"; break;
	}
	// показать в списке
	CListCtrl& ctr = this->GetListCtrl();

	int n = filter.Find(':');
	//::AfxMessageBox(n);
	CString str;
    str.Format("%d",n);
	//::AfxMessageBox(str); -1 если нет :

	CString str1 = "";
	if (n == 8) {
		str1 = filter.Mid(9);
		int kk = str1.Find(':');
		CString sss;
        sss.Format("%d", kk);
		CString sip = str1.Left(kk);
		//::AfxMessageBox(sip);

		if (filter.Left(1) == "S" || filter.Left(1) == "s") {
			int h = filter.Find(':', 9);
			if (h == -1) {
				if (sourceIp == str1) {
					int row = ctr.InsertItem(ctr.GetItemCount(), line_num); // получение номера строки для вывода
					ctr.SetItemText(row, 1, timestr);
					ctr.SetItemText(row, 2, sourceIp);
					ctr.SetItemText(row, 3, DestinationIp);
					ctr.SetItemText(row, 4, m_packetkind);
					ctr.SetItemText(row, 5, m_len);
				}
			}
			else { // str1 = 192.168.0.103:protocol:tcp
				CString str2;
				str2 = str1.Mid(kk+1); // protocol:tcp
				//::AfxMessageBox(str2);
				if (str2.Left(1) == "P" || str2.Left(1) == "p") { 
					str2 = str2.Mid(9);
					str2.MakeUpper();
					if (sourceIp == sip && m_packetkind == str2) {
						int row = ctr.InsertItem(ctr.GetItemCount(), line_num);
						ctr.SetItemText(row, 1, timestr);
						ctr.SetItemText(row, 2, sourceIp);
						ctr.SetItemText(row, 3, DestinationIp);
						ctr.SetItemText(row, 4, m_packetkind);
						ctr.SetItemText(row, 5, m_len);
					}
				}
				if (str2.Left(1) == "D" || str2.Left(1) == "d") { 
					str2 = str2.Mid(14);
					//::AfxMessageBox(str2);
					int h = str2.Find(':', 1);
					if (h == -1) {
						if (sourceIp == sip && DestinationIp == str2) {
							int row = ctr.InsertItem(ctr.GetItemCount(), line_num); // получение номера строки для вывода
							ctr.SetItemText(row, 1, timestr);
							ctr.SetItemText(row, 2, sourceIp);
							ctr.SetItemText(row, 3, DestinationIp);
							ctr.SetItemText(row, 4, m_packetkind);
							ctr.SetItemText(row, 5, m_len);
						}
					}
					else {
						int kk = str2.Find(':');
						CString sss;
						sss.Format("%d", kk);
						CString dip = str2.Left(kk);
						//::AfxMessageBox(dip);
						str2 = str2.Mid(h+1);
						//::AfxMessageBox(str2);
						str2 = str2.Mid(9);
						//::AfxMessageBox(str2);
						str2.MakeUpper();
						//::AfxMessageBox(sip);
						//::AfxMessageBox(dip);
						//::AfxMessageBox(str2);
						if (sourceIp == sip && DestinationIp == dip && m_packetkind == str2) {
						//if (sourceIp == sip && DestinationIp == dip) {
							int row = ctr.InsertItem(ctr.GetItemCount(), line_num);
							ctr.SetItemText(row, 1, timestr);
							ctr.SetItemText(row, 2, sourceIp);
							ctr.SetItemText(row, 3, DestinationIp);
							ctr.SetItemText(row, 4, m_packetkind);
							ctr.SetItemText(row, 5, m_len);
						}
					}

					/*if (sourceIp == sip && DestinationIp == str2) {
						int row = ctr.InsertItem(ctr.GetItemCount(), line_num);
						ctr.SetItemText(row, 1, timestr);
						ctr.SetItemText(row, 2, sourceIp);
						ctr.SetItemText(row, 3, DestinationIp);
						ctr.SetItemText(row, 4, m_packetkind);
						ctr.SetItemText(row, 5, m_len);
					}*/
				}
			}
		}
	}
	if (n == 13) {
		str1 = filter.Mid(14);
		int kk = str1.Find(':');
		CString sss;
        sss.Format("%d", kk);
		CString dip = str1.Left(kk);
		//::AfxMessageBox(dip);

		if (filter.Left(1) == "D" || filter.Left(1) == "d") {
			int h = filter.Find(':', 14);
			if (h == -1) {
				if (DestinationIp == str1) {
					int row = ctr.InsertItem(ctr.GetItemCount(), line_num); // получение номера строки для вывода
					ctr.SetItemText(row, 1, timestr);
					ctr.SetItemText(row, 2, sourceIp);
					ctr.SetItemText(row, 3, DestinationIp);
					ctr.SetItemText(row, 4, m_packetkind);
					ctr.SetItemText(row, 5, m_len);
				}
			}
			else { // str1 = 192.168.0.103:protocol:tcp
				CString str2;
				str2 = str1.Mid(kk+1); // protocol:tcp
				//::AfxMessageBox(str2);
				if (str2.Left(1) == "P" || str2.Left(1) == "p") {
					str2 = str2.Mid(9);
					str2.MakeUpper();
					//::AfxMessageBox(dip);
					//::AfxMessageBox(str2);
					if (DestinationIp == dip && m_packetkind == str2) {
						int row = ctr.InsertItem(ctr.GetItemCount(), line_num);
						ctr.SetItemText(row, 1, timestr);
						ctr.SetItemText(row, 2, sourceIp);
						ctr.SetItemText(row, 3, DestinationIp);
						ctr.SetItemText(row, 4, m_packetkind);
						ctr.SetItemText(row, 5, m_len);
					}
				}
				if (str2.Left(1) == "S" || str2.Left(1) == "s") { 
					str2 = str2.Mid(9);
					if (sourceIp == str2 && DestinationIp == dip) {
						int row = ctr.InsertItem(ctr.GetItemCount(), line_num);
						ctr.SetItemText(row, 1, timestr);
						ctr.SetItemText(row, 2, sourceIp);
						ctr.SetItemText(row, 3, DestinationIp);
						ctr.SetItemText(row, 4, m_packetkind);
						ctr.SetItemText(row, 5, m_len);
					}
				}
			}

		}
	}
	/*
	if (n == 8) {
		str1 = filter.Mid(9);
		if (filter.Left(1) == "S" || filter.Left(1) == "s") {
			if (sourceIp == str1) {
				int row = ctr.InsertItem(ctr.GetItemCount(), line_num); // получение номера строки для вывода
				ctr.SetItemText(row, 1, timestr);
				ctr.SetItemText(row, 2, sourceIp);
				ctr.SetItemText(row, 3, DestinationIp);
				ctr.SetItemText(row, 4, m_packetkind);
				ctr.SetItemText(row, 5, m_len);
			}
		}
	}
	if (n == 13) {
		str1 = filter.Mid(14);
		//::AfxMessageBox(str1);
		if (DestinationIp == str1) {
			int row = ctr.InsertItem(ctr.GetItemCount(), line_num); // получение номера строки для вывода
			ctr.SetItemText(row, 1, timestr);
			ctr.SetItemText(row, 2, sourceIp);
			ctr.SetItemText(row, 3, DestinationIp);
			ctr.SetItemText(row, 4, m_packetkind);
			ctr.SetItemText(row, 5, m_len);
		}
	} */

	/*if (n == 8) {
		str1 = filter.Mid(9);
		if (filter.Left(1) == "P" || filter.Left(1) == "p") {
			str1.MakeUpper();
			int h = 0;
			h = filter.Find(':', 9);
			if (h != 0) {
				CString str2;
				str2.Format("%d",h);
				::AfxMessageBox(h);
			}*/


	if (n == 8) {
		str1 = filter.Mid(9);
		if (filter.Left(1) == "P" || filter.Left(1) == "p") {
			str1.MakeUpper();
			if (m_packetkind == str1) {
				int row = ctr.InsertItem(ctr.GetItemCount(), line_num); // получение номера строки для вывода
				ctr.SetItemText(row, 1, timestr);
				ctr.SetItemText(row, 2, sourceIp);
				ctr.SetItemText(row, 3, DestinationIp);
				ctr.SetItemText(row, 4, m_packetkind);
				ctr.SetItemText(row, 5, m_len);
			}
		}
	}
  	
	if (filter == "") {
		int row = ctr.InsertItem(ctr.GetItemCount(), line_num); // получение номера строки для вывода
		ctr.SetItemText(row, 1, timestr);
		ctr.SetItemText(row, 2, sourceIp);
		ctr.SetItemText(row, 3, DestinationIp);
		ctr.SetItemText(row, 4, m_packetkind);
		ctr.SetItemText(row, 5, m_len);
	}

	// После того, как пакет захвачен, документ настроек был изменен. При выходе будет предложено сохранить файл дампа
	if(App.dumperInfo)
		this->GetDocument()->SetModifiedFlag();
	else // В настоящее время этот пакет в файле pcap, не нужно устанавливать измененный флаг.
		;
}

// CTableView наследуется от CListView
void CTableView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	static BOOL inited = FALSE;
	if(inited) return;
	inited = TRUE; // первый вызов OnInitialUpdate
	
	App.displayHWND = this->GetSafeHwnd(); // дескриптор окна
	CListCtrl& list = GetListCtrl(); // получение ссылки на CListCtrl базового CListView
	LONG lStyle;
	lStyle = GetWindowLong(list.m_hWnd, GWL_STYLE); // полуить текущий стиль
	lStyle &= ~LVS_TYPEMASK; // определить текущий стиль окна элемента управления 
	lStyle |= LVS_REPORT; // выравнивание первого столбца по левому краю
	SetWindowLong(list.m_hWnd, GWL_STYLE, lStyle); // задать новый стиль на основе старого, изменив только вид списка
	DWORD dwStyle = list.GetExtendedStyle(); // извлекает текущие раширенные стили управления list
	dwStyle |= LVS_EX_FULLROWSELECT; // выделение всех пунктов выбранного элемента
	dwStyle |= LVS_EX_GRIDLINES; // отображает линии сетки вокруг элементов
	list.SetExtendedStyle(dwStyle);
	list.SetBkColor(RGB(200, 200, 200));
	list.SetTextBkColor(RGB(200, 200, 200));
	list.SetTextColor(RGB(0, 0, 0));
	
	list.InsertColumn( 0, "No.", LVCFMT_CENTER, 50);
	list.InsertColumn( 1, "Time", LVCFMT_CENTER, 80); 
	list.InsertColumn( 2, "Source", LVCFMT_CENTER, 240);
	list.InsertColumn( 3, "Destination", LVCFMT_CENTER, 240);
	list.InsertColumn( 4, "Protocol", LVCFMT_CENTER, 80);
	list.InsertColumn( 5, "Length", LVCFMT_CENTER, 80);
}

// нажатие на строку в списке
void CTableView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	NMLISTVIEW* pList = (NMLISTVIEW*)pNMHDR;
	CString m_message; // инициализация пуста
	HeadMain* head_temp;
    int iItem = pList->iItem;
    if(iItem != -1)
    {
        App.hexView->TextToHex((char*)m_info[iItem].pkt_data, m_info[iItem].header->len);
		head_temp = m_info[iItem].m_Head;
		while(head_temp != NULL)
		{
			m_message = m_message+head_temp->PrintData()+"\r\n";
			head_temp = head_temp->next;
		}
		App.packetView->ShowPackInfo(m_message);
    }
    *pResult = 0;
}

// очищение списка
void CTableView::clearList()
{
	count = 0;
	this->m_info.clear();
	this->GetListCtrl().DeleteAllItems();
}
