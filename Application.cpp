#include "Application.h"
#include "ApplicationDoc.h"
#include "Main.h"
#include "TableView.h"

BEGIN_MESSAGE_MAP(CApplication, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, OnCaptureFilter)
	//ON_COMMAND(ID_APP_ABOUT, OnOK)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	//ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

//CString filter = "";
extern CString filter;

// ������������� ���� � �����
CApplication::CApplication()
{
	char buf[1024];
	GetCurrentDirectory(sizeof(buf),buf);
	this->dumpFilePath.Format("%s\\tmp.pcap", buf);
	this->catching = FALSE;
}

CApplication App;
int count = 1;
unsigned char SourceIP[4];
unsigned char DestinationIP[4];
unsigned char SourceIPv6[16];
unsigned char DestinationIPv6[16];
int IPv6 = 0;	// ���� ���, ipv6 ����� 0, ���� �������� ����� 1
int packet_kind = 0; // 0: Ethernet 1: 802.3 2: ARP 3: IP 4: ICMP 5: UDP 6: IGMP 7: TCP
int remain_len;	// ���������� ����� ������, ������������ ��� ������������� http � ������� ����� 80

BOOL CApplication::InitInstance()
{
	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;  // ����� ������
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME, // ������������� ����
		RUNTIME_CLASS(CApplicationDoc), // ��������
		RUNTIME_CLASS(CMain), // ����� ����
		RUNTIME_CLASS(CTableView)); // ��������
	AddDocTemplate(pDocTemplate); // �������� ������

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo; // ����� ������
	ParseCommandLine(cmdInfo); // ������ ��������� ������

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo)) // ������ ������ �� ����������
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW); // �������� ����
	m_pMainWnd->UpdateWindow(); // ��������� ���� ��������� ���������

	return TRUE;
}

// ������ ������ ��� ������� �������
void CApplication::startCatch()
{
	this->catching = TRUE;	// ����
	this->tableView->clearList();	// ������� ������
	::AfxBeginThread(ThreadProc, new ThreadParam(captureInstance, dumperInfo)); // ������ ������ �������
}

class CCaptureFilter : public CDialog
{
public:
	CCaptureFilter();
	CString GetEditStr();
	void SetEditStr(CString  csEdit);
	virtual void DoDataExchange(CDataExchange* pDX);
// Dialog Data
	//{{AFX_DATA(CCaptureFilter)
	enum { IDD = IDD_FILTER};
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaptureFilter)
	protected:
	CString m_edit;    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

void CCaptureFilter::SetEditStr(CString  csEdit)
{
	m_edit = csEdit;
}
CString CCaptureFilter::GetEditStr()
{
	return m_edit;
}

CCaptureFilter::CCaptureFilter() : CDialog(CCaptureFilter::IDD)
{
	//{{AFX_DATA_INIT(CCaptureFilter)
	//filter = _T("");
	//filter2 = _T("");
	//}}AFX_DATA_INIT
} 

void CCaptureFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaptureFilter)
	DDX_Text(pDX, IDC_FILTER, m_edit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCaptureFilter, CDialog)
	//{{AFX_MSG_MAP(CCaptureFilter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP() 

// App command to run the dialog
void CApplication::OnCaptureFilter()
{
	CCaptureFilter FilterDlg;
	//FilterDlg.SetEditStr(filter);
	//FilterDlg.SetEditStr("sourceip:192.168.0.1");
	//FilterDlg.SetEditStr("destinationip:192.168.43.166");
	//FilterDlg.SetEditStr("protocol:tcp");
	//FilterDlg.DoModal();
	//::AfxMessageBox(FilterDlg.GetEditStr());
	FilterDlg.SetEditStr(filter);

	int nRet = -1;
	nRet = FilterDlg.DoModal();

	switch (nRet)
	{
		case -1: 
			AfxMessageBox("Dialog box could not be created!");
			break;
		case IDOK:
			filter = FilterDlg.GetEditStr();
			break;
		case IDCANCEL:
			filter = "";
			break;
		default:
			AfxMessageBox("Error");
			break;
	};

	//filter = FilterDlg.GetEditStr();

	/*if(IDOK == FilterDlg.DoModal()) {
		CWnd* pWnd = AfxGetMainWnd();
		::AfxMessageBox("1");
		CButton *m_ctlCheck = (CButton*)pWnd-> GetDlgItem(IDC_CHECK1);
		::AfxMessageBox("2");
		int ChkBox; //= m_ctlCheck->GetCheck();
		::AfxMessageBox("3");
		CString str;*/

	/*if(ChkBox == BST_UNCHECKED)
		str.Format(_T("Un Checked"));
	else if(ChkBox == BST_CHECKED)
		str.Format(_T("Checked")); */

	//::AfxMessageBox("3"); 
	//}
	//filter = FilterDlg.GetEditStr();

	/*CButton *m_ctlCheck = (CButton*) GetDlgItem(IDC_CHECK1);
    int ChkBox = m_ctlCheck->GetCheck();
	CString str;

	if(ChkBox == BST_UNCHECKED)
		str.Format(_T("Un Checked"));
	else if(ChkBox == BST_CHECKED)
		str.Format(_T("Checked"));

	::AfxMessageBox(str); */
}
/*
void CApplication::OnOK() 
{
	CCaptureFilter FilterDlg;
	FilterDlg.SetEditStr("192.168.0.1");
	//FilterDlg.DoModal();
    //::AfxMessageBox("sds");
	if(IDOK == FilterDlg.DoModal()) {
		CWnd* pWnd = AfxGetMainWnd();
		::AfxMessageBox("1");
		CButton *m_ctlCheck = (CButton*)pWnd-> GetDlgItem(IDC_CHECK1);
		::AfxMessageBox("2");
		int ChkBox;// = m_ctlCheck->GetCheck();
		::AfxMessageBox("3");
		CString str;

	if(ChkBox == BST_UNCHECKED)
		str.Format(_T("Un Checked"));
	else if(ChkBox == BST_CHECKED)
		str.Format(_T("Checked"));

	::AfxMessageBox("3"); 
	}
	filter = FilterDlg.GetEditStr();
} */

BOOL CCaptureFilter::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//this->filter = "include:remote:tcp:80";
	/*CButton *m_ctlCheck = (CButton*) GetDlgItem(IDC_CHECK1);
    int ChkBox = m_ctlCheck->GetCheck();
	CString str;

	if(ChkBox == BST_UNCHECKED)
		str.Format(_T("Un Checked"));
	else if(ChkBox == BST_CHECKED)
		str.Format(_T("Checked"));

	::AfxMessageBox(str);*/

	this->UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
} 
