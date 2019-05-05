#include "stdafx.h"
#include "Main.h"
#include "Application.h"
#include "TableView.h"
#include "HexView.h"
#include "PacketView.h"
#include "FindDevs.h"

IMPLEMENT_DYNCREATE(CMain, CFrameWnd) // динамическое создание объекта (имя класса, имя класса родителя)

BEGIN_MESSAGE_MAP(CMain, CFrameWnd) // таблица откликов на сообщения
	//{{AFX_MSG_MAP(CMain)
	ON_WM_CREATE() // событие создания окна
	ON_COMMAND(ID_SELECTSTART, OnSelectStart)
	ON_COMMAND(ID_ENDCATCH, OnEndCatch)
	ON_UPDATE_COMMAND_UI(ID_SELECTSTART, OnUpdateSelectStart) // обновление элементов интерфейса
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_ENDCATCH, OnUpdateEndCatch)
	ON_MESSAGE(WM_TEXIT, OnTExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator панель состояния
}; 

CMain::CMain() {}

CMain::~CMain() {}

int CMain::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1; // окно не создано
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY); // флаги закрепления панели инструментов
	EnableDocking(CBRS_ALIGN_ANY); // функция окна для управления панелью
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMain::PreCreateWindow(CREATESTRUCT& cs) // вызывается перед созданием окна
{
	if(!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.cx = 810;
	cs.cy = 500;
	return TRUE;
}

// Создать 3 строки и 1 столбец с разделением формы
BOOL CMain::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	App.controlHWND = this->GetSafeHwnd(); //возвращает дескриптор окна для окна

	CRect rc;

    GetClientRect(&rc); // координаты клиентской области

    // окно 3 строки и 1 столбец
    if (!m_wndSplitter.CreateStatic(this, 3, 1, WS_CHILD | WS_VISIBLE))
        return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CTableView), CSize(rc.Width(), rc.Height()/3), pContext))
        return FALSE;
	if (!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CPacketView), CSize(rc.Width(), rc.Height()/4), pContext))
        return FALSE;
    if (!m_wndSplitter.CreateView(2, 0, RUNTIME_CLASS(CHexView), CSize(rc.Width(), rc.Height()/2), pContext))
        return FALSE;

    return TRUE;
}

// нажать Start и выбрать устройство для захвата пакетов
void CMain::OnSelectStart() 
{
	CFindDevs selector;

	if(IDOK == selector.DoModal() && App.captureInstance)
	{
		// открыть tmp.pcap для получения данных пакета
		App.dumperInfo = pcap_dump_open(App.captureInstance, App.dumpFilePath);
		App.startCatch(); // запуск потока для захвата пакетов
	}
}

// освобождение ресурсов, если нажать кнопку закрытия программы
void CMain::OnTExit(int exitNum)
{
	// отключение устройства и файла дампа
	pcap_close(App.captureInstance);
	if(App.dumperInfo)
		pcap_dump_close(App.dumperInfo);

	// флаг
	App.catching = FALSE;
}

// кнопка завершить прослушивание
void CMain::OnEndCatch() 
{
	pcap_breakloop(App.captureInstance);
}

// кнопка "Start"
void CMain::OnUpdateSelectStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!App.catching);
}

// кнопка "End"
void CMain::OnUpdateEndCatch(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(App.catching);
}

// кнопка "Сохранить"
void CMain::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!App.catching);	
}

// кнопка "Сохранить как"
void CMain::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!App.catching);	
}

//  кнопка "Открыть файл"
void CMain::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!App.catching);	
}
