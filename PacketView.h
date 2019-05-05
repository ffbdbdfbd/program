#if !defined(AFX_PACKETVIEW_H__100AE7CF_89B2_4776_AE90_B0490E99F9B4__INCLUDED_)
#define AFX_PACKETVIEW_H__100AE7CF_89B2_4776_AE90_B0490E99F9B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

class CPacketView : public CEditView
{
public:
	// отобразить строку в область
	void ShowPackInfo(const CString& info);

protected:
	CPacketView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPacketView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPacketView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPacketView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPacketView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PACKETVIEW_H__100AE7CF_89B2_4776_AE90_B0490E99F9B4__INCLUDED_)
