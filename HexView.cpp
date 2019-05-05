#include "stdafx.h"
#include "HexView.h"
#include "Application.h"

IMPLEMENT_DYNCREATE(CHexView, CEditView)

CHexView::CHexView()
{
	// 16-������ ���������
	App.hexView = dynamic_cast<Hex*>(this);
}

CHexView::~CHexView() {}

BEGIN_MESSAGE_MAP(CHexView, CEditView)
	//{{AFX_MSG_MAP(CHexView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CHexView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

// ��������� 16-������� �������������
void CHexView::TextToHex(char *data, int length)
{
	CString result; // ���������
	CString temp; // �������� ������������ �����������
	int lineHead; // ��������

	for(lineHead = 0; lineHead < length; lineHead += 16)
	{
		// �������� �����
		temp.Format("%04X: ", lineHead);
		result += temp;

		CString hex;
		CString ascii;
		int position; // �������
		int lineLength = length - lineHead >= 16 ? 16 : length - lineHead; // ���������� ������ ��� ������ � ���� ������

		for(position = 0; position < lineLength; ++position)
		{
			unsigned char c = (unsigned char)data[lineHead + position];

			// 16-������ ������
			if(position != 8)
				temp.Format(" %02X", c);
			else
				temp.Format("-%02X", c);
			hex += temp;

			// ��� ASCII ������?
			ascii += isprint(c) ? c : '.';
		}

		result += hex;
		if(position < 16)
		{
			CString sPad(' ', 3 * (16 - position));
			result += sPad;
		}
		result += "  ";
		result += ascii;
		result += "\r\n";
	}

	// �������� ���������
	this->SetWindowText(result);
}

// �������������
void CHexView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	static BOOL inited = FALSE;
	if(inited) return;
	inited = TRUE; // ������ ����� OnInitialUpdate()

	CEdit& control = this->GetEditCtrl();
	control.SetReadOnly();// ������ ��� ������

	// ���������� �����
	this->m_Font.CreatePointFont(120, "Courier New");
	this->SetFont(&this->m_Font, false);
}

