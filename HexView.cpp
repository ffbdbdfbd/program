#include "stdafx.h"
#include "HexView.h"
#include "Application.h"

IMPLEMENT_DYNCREATE(CHexView, CEditView)

CHexView::CHexView()
{
	// 16-ричный указатель
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

// Реализаци 16-ричного предстваления
void CHexView::TextToHex(char *data, int length)
{
	CString result; // результат
	CString temp; // хранение прмежуточных результатов
	int lineHead; // смещение

	for(lineHead = 0; lineHead < length; lineHead += 16)
	{
		// смещение линии
		temp.Format("%04X: ", lineHead);
		result += temp;

		CString hex;
		CString ascii;
		int position; // позиция
		int lineLength = length - lineHead >= 16 ? 16 : length - lineHead; // количество байтов для печати в этой строке

		for(position = 0; position < lineLength; ++position)
		{
			unsigned char c = (unsigned char)data[lineHead + position];

			// 16-ричный формат
			if(position != 8)
				temp.Format(" %02X", c);
			else
				temp.Format("-%02X", c);
			hex += temp;

			// это ASCII символ?
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

	// показать результат
	this->SetWindowText(result);
}

// инициализация
void CHexView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	static BOOL inited = FALSE;
	if(inited) return;
	inited = TRUE; // первый вызов OnInitialUpdate()

	CEdit& control = this->GetEditCtrl();
	control.SetReadOnly();// только для чтения

	// установить шрифт
	this->m_Font.CreatePointFont(120, "Courier New");
	this->SetFont(&this->m_Font, false);
}

