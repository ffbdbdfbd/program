#include "stdafx.h"
#include "ApplicationDoc.h"
#include "Application.h"
#include "Thread.h"

IMPLEMENT_DYNCREATE(CApplicationDoc, CDocument)

BEGIN_MESSAGE_MAP(CApplicationDoc, CDocument)
	//{{AFX_MSG_MAP(CApplicationDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CApplicationDoc::CApplicationDoc() {}

CApplicationDoc::~CApplicationDoc() {}

BOOL CApplicationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

// копирует содержимое файла src в файл dest
void copyFile(CFile *src, CFile *dest)
{
	char buf[1024];
	int n;

	while((n = src->Read(buf, sizeof(buf))) > 0)
		dest->Write(buf, n);
}

// открытие и сохранение файла
void CApplicationDoc::Serialize(CArchive& ar)
{
	CFile *f = ar.GetFile();

	// если tmp.pcap выдает ошибку
	if(App.dumpFilePath.Compare(f->GetFilePath()) == 0)
	{
		CString s;
		s.Format("%s Файл уже используется для записи", App.dumpFilePath);
		::AfxMessageBox(s);
		return;
	}

	// если файл сохранен, то копирует содержимое tmp.pcap в f
	if (ar.IsStoring())
	{
		CFile src(App.dumpFilePath, CFile::modeRead);
		copyFile(&src, f);
		src.Close();
	}

	// если файл уже открыт, то копировать содержимое из файла в tmp.pcap
	else
	{
		CFile dest(App.dumpFilePath, CFile::modeWrite | CFile::modeCreate);
		copyFile(f, &dest);
		dest.Close();

		// открыть tmp.pcap, слушать, в событии TExit закрыть файл tmp.pcap
		FILE *fp = fopen(App.dumpFilePath, "rb");
		char errbuf[PCAP_ERRBUF_SIZE];
		App.captureInstance = pcap_fopen_offline(fp, errbuf);
		App.dumperInfo = NULL;
		App.startCatch(); // начать захватывать пакет с флагом ловить
	}
}
