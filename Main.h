#if !defined(AFX_MAIN_H__6151A7B4_3D82_4F81_B4BD_9E2BA7D82F79__INCLUDED_)
#define AFX_MAIN_H__6151A7B4_3D82_4F81_B4BD_9E2BA7D82F79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMain : public CFrameWnd // класс рамки окна
{
protected:
	CSplitterWnd m_wndSplitter; // для создания формы с разделителями
public:
	void OnTExit(int exitNum); // обработка события выхода из потока захвата 
	
protected:
	CMain();
	DECLARE_DYNCREATE(CMain)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMain)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMain();

protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMain)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelectStart();
	afx_msg void OnEndCatch();
	afx_msg void OnUpdateSelectStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEndCatch(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_MAIN_H__6151A7B4_3D82_4F81_B4BD_9E2BA7D82F79__INCLUDED_)
