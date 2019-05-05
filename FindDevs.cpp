#define _CTR_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "FindDevs.h"
#include "Application.h"
#include <pcap.h>
#include <winsock.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <C:\Users\HOME\Desktop\algorithm>
#include <vector>
#include <C:\Users\HOME\Downloads\iptypes.h>
#include <C:\Users\HOME\Downloads\iphlpapi.h>
#include <C:\Users\HOME\Downloads\ipexport.h>
#pragma comment(lib, "IPHLPAPI.lib")

using namespace std;

// выбор устройства
CFindDevs::CFindDevs(CWnd* pParent)
	: CDialog(CFindDevs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindDevs)
	//}}AFX_DATA_INIT
}

// обмен данными
void CFindDevs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDevs)
	//DDX_Control(pDX, IDC_DEVS, m_devsName);
	DDX_Control(pDX, IDC_LIST1, m_devsName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFindDevs, CDialog)
	//{{AFX_MSG_MAP(CFindDevs)
	ON_WM_CREATE()
    //ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

vector<pcap_if_t *> reverse_a (const vector<pcap_if_t *> &v1) {
vector<pcap_if_t *> v2;
size_t i = v1.size();
while (i > 0)
v2.push_back (v1[--i]);
return v2;
}

// получение списка устройств
void CFindDevs::InitWinPcap()
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1){
		::AfxMessageBox("Failed to get device list");
		return;
	}

	/*for (d = alldevs; d; d = d->next){
		this->m_devsArray.push_back(d);
		CString line_num;
		int count = 0;
		line_num.Format("%d", count++);
		int row = m_devsName.InsertItem(m_devsName.GetItemCount(), line_num); // получение номера строки для вывода
		//m_devsName.SetItemText(row, 1, d->description);
	} */

	#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
    #define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

	PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        printf("Error allocating memory needed to call GetAdaptersinfo\n");
		::AfxMessageBox("1Error opening device");
        //return 1;
    }
	
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            printf("Error allocating memory needed to call GetAdaptersinfo\n");
			::AfxMessageBox("2Error opening device");
            //return 1;
        }
    }

	int k = -1;
	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		for (d = alldevs; d; d = d->next){
			this->m_devsArray.push_back(d);
			CString line_num;
			int count = 0;
			line_num.Format("%d", count++);
			int row = m_devsName.InsertItem(m_devsName.GetItemCount(), line_num); // получение номера строки для вывода
			m_devsName.SetItemText(row, 0, pAdapter->Description);
			m_devsName.SetItemText(row, 2, pAdapter->AdapterName);
			m_devsName.SetItemText(row, 3, pAdapter->IpAddressList.IpAddress.String);
			pAdapter = pAdapter->Next;
			k++;
		}
	} 

	v2 = reverse_a (m_devsArray);
	CString c1, c2, c3, c4;
;
	for (d = alldevs; d; d = d->next){
		this->m_devsArray.push_back(d);
		CString line_num;
		int count = 0;
		line_num.Format("%d", count++);
		m_devsName.SetItemText(k, 1, d->description);
		if (k==1) c1 =  d->description;
		if (k==3) c2 =  d->description;
        if (k==0) c3 =  d->description;
		if (k==5) c4 =  d->description;
		k--;
	}
	m_devsName.SetItemText(1, 1, c2);
	m_devsName.SetItemText(3, 1, c1);
	m_devsName.SetItemText(0, 1, c4);
	m_devsName.SetItemText(5, 1, c3);

	
    if (pAdapterInfo)
        FREE(pAdapterInfo); 
	this->UpdateData(false);
}



int CFindDevs::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    CRect rect;
	GetClientRect(rect);
    
	m_devsName.Create(WS_VISIBLE|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|WS_VSCROLL, CRect(10,10,970,128), this, 50);

	DWORD dwStyle = m_devsName.GetExtendedStyle(); // извлекает текущие раширенные стили управления list
	dwStyle |= LVS_EX_FULLROWSELECT; // выделение всех пунктов выбранного элемента
	dwStyle |= LVS_EX_GRIDLINES; // отображает линии сетки вокруг элементов
	dwStyle |= WS_VSCROLL;
	m_devsName.SetExtendedStyle(dwStyle);

	m_devsName.SetBkColor(RGB(215, 215, 215));
	m_devsName.SetTextBkColor(RGB(215, 215, 215));
    m_devsName.InsertColumn(0, "Device", LVCFMT_LEFT, 250);	
	m_devsName.InsertColumn(1, "Description", LVCFMT_LEFT, 360);
	m_devsName.InsertColumn(2, "Name", LVCFMT_LEFT, 250);
	m_devsName.InsertColumn(3, "IP", LVCFMT_CENTER, 100);	

	return 0;

}

// освобождение списка устройств
CFindDevs::~CFindDevs()
{
	// если произошел сбой, то вызов pcap_freealldevs
	if(!this->m_devsArray.empty())
		pcap_freealldevs(*(this->m_devsArray.begin()));
}

// инициализация
BOOL CFindDevs::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->InitWinPcap();
	return TRUE;
}

// кнопка "ОК"
void CFindDevs::OnOK() 
{
	int nItem;
	POSITION pos = m_devsName.GetFirstSelectedItemPosition();
    if (pos == NULL) {
		::AfxMessageBox("Сетевой интерфейс не выбран");
		return;
    }
    else{
		while (pos) {
			nItem = m_devsName.GetNextSelectedItem(pos);
        }
    }
	char errbuf[PCAP_ERRBUF_SIZE];

	pcap_if_t *d = this->v2[nItem];
	if ((App.captureInstance = pcap_open(d->name, // name of the device
		65536, // portion of the packet to capture , 65535 guarantees that the whole packet will be captured on all the link layers
		PCAP_OPENFLAG_PROMISCUOUS, // promiscuous mode 
		1000, // read timeout
		NULL, // authentication on the remote machine 
		errbuf) // error buffer
		) == NULL)
	{
		::AfxMessageBox("Error opening device");
		App.captureInstance = NULL;
		return;
	} 

	CDialog::OnOK();
} 