#if !defined(AFX_FINDDEVS_H__755F40B6_DD35_401F_AEA5_0374BAEDFC44__INCLUDED_)
#define AFX_FINDDEVS_H__755F40B6_DD35_401F_AEA5_0374BAEDFC44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <pcap.h>
#include "resource.h"

using namespace std;

class CFindDevs : public CDialog
{
protected:
	vector<pcap_if_t *> m_devsArray, v2; // список устройств
	void InitWinPcap(); // получение списка устройств
public:
	~CFindDevs();

// Construction
public:
	CFindDevs(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindDevs)
	//enum { IDD = IDD_SELECT  };
	//CComboBox	m_devsName;
	enum { IDD = IDD_SELECT  };
	CListCtrl m_devsName;;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindDevs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindDevs)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_FINDDEVS_H__755F40B6_DD35_401F_AEA5_0374BAEDFC44__INCLUDED_)
