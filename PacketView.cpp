#include "stdafx.h"
#include "PacketView.h"
#include "Application.h"

IMPLEMENT_DYNCREATE(CPacketView, CEditView)

CPacketView::CPacketView()
{
	// отображение деталей каждого уровня PDU захваченного пакета
	App.packetView = this;
}

CPacketView::~CPacketView() {}

BEGIN_MESSAGE_MAP(CPacketView, CEditView)
	//{{AFX_MSG_MAP(CPacketView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPacketView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

// инициализация
void CPacketView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	static BOOL inited = FALSE;
	if(inited) return;
	inited = TRUE; 	// первый вызов OnInitialUpdate
	//CEdit& m_edi = GetEditCtrl();
	//m_edi.SetColor(RGB(255, 255, 255));

	CEdit& control = this->GetEditCtrl();
	control.SetReadOnly();// только для чтения
}

// отобразить строку
void CPacketView::ShowPackInfo(const CString& info)
{
	this->SetWindowText(info);
}
