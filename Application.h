#if !defined(AFX_APPLICATION_H__6F3BD220_C90F_4D95_896A_BB01F11B7493__INCLUDED_)
#define AFX_APPLICATION_H__6F3BD220_C90F_4D95_896A_BB01F11B7493__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Hex.h"
#include "PacketView.h"
#include "TableView.h"
#include "Thread.h"
#include "resource.h"
#include <pcap.h>

class CApplication : public CWinApp
{
public:
	CApplication();

	CPacketView *packetView;	// представление заголовков пакета
	Hex *hexView;			    // шестнадцатеричное предстваление
	CTableView *tableView;   	// очистка списка
	HWND controlHWND;			// дескриптор сообщения, получающего выход потока
	HWND displayHWND;			// дескриптор сообщения, которое перехватило пакет

	pcap_t *captureInstance;	// устройство слушается в pcap_loop сейчас
	pcap_dumper_t *dumperInfo;	// хранение информации дампа
	CString dumpFilePath;	    // путь к файлу tmp.pcap
	BOOL catching;		     	// флаг захвата покета для кнопок
	void startCatch();			// запуск захвата

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApplication)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CApplication)
	afx_msg void OnCaptureFilter();
	//afx_msg void OnOK();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

extern CApplication App;

#endif // !defined(AFX_APPLICATION_H__6F3BD220_C90F_4D95_896A_BB01F11B7493__INCLUDED_)
