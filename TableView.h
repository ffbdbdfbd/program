#if !defined(AFX_TABLEVIEW_H__8639B617_C95A_40FF_B913_01963AF74123__INCLUDED_)
#define AFX_TABLEVIEW_H__8639B617_C95A_40FF_B913_01963AF74123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <pcap.h>
#include <vector>
#include "Info.h"

using namespace std;

class CTableView : public CListView
{
public:
	void clearList(); // очитска списка
	//CString filter;
protected:
	void OnTCatch(struct pcap_pkthdr *header, u_char *pkt_data); // обработка события, которое поймал пакет
	vector<Info> m_info; // хранение
protected:
	CTableView(); 
	DECLARE_DYNCREATE(CTableView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTableView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTableView)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TABLEVIEW_H__8639B617_C95A_40FF_B913_01963AF74123__INCLUDED_)
