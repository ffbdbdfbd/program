#if !defined(AFX_HEXVIEW_H__3CE45B5E_47D3_4467_AC38_1C5EAD1CC739__INCLUDED_)
#define AFX_HEXVIEW_H__3CE45B5E_47D3_4467_AC38_1C5EAD1CC739__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Hex.h"

class CHexView : public CEditView, public Hex
{
protected:
	CFont m_Font; // хранение шрифта
protected:
	CHexView(); 
	DECLARE_DYNCREATE(CHexView)

// Attributes
public:

// Operations
public:
	// реализация 16-ричного представления это виртуальная функция
	void TextToHex(char *data, int length);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC); 
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHexView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHexView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_HEXVIEW_H__3CE45B5E_47D3_4467_AC38_1C5EAD1CC739__INCLUDED_)
