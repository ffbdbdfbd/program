#if !defined(AFX_APPLICATIONDOC_H__6FF6570C_C264_4347_B057_065BF82A0DC0__INCLUDED_)
#define AFX_APPLICATIONDOC_H__6FF6570C_C264_4347_B057_065BF82A0DC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CApplicationDoc : public CDocument
{
protected:
	CApplicationDoc();
	DECLARE_DYNCREATE(CApplicationDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApplicationDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CApplicationDoc();
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CApplicationDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_APPLICATIONDOC_H__6FF6570C_C264_4347_B057_065BF82A0DC0__INCLUDED_)
