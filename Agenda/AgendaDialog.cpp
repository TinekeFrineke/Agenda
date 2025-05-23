
// AgendaDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AgendaDialog.h"

#include "Agenda.h"
#include "afxdialogex.h"

#include "AgendaPage.h"
#include "CombinedPage.h"
#include "OldFilesPage.h"
#include "Settings.h"
#include "TotalsPage.h"
#include "WorkAndPlayPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// AgendaDialog dialog



AgendaDialog::AgendaDialog(Agenda::Agenda & agenda, Settings & settings, CWnd* pParent /*=NULL*/)
: CDialogEx(IDD_AGENDA_DIALOG, pParent),
  m_Agenda  (agenda),
  m_Settings(settings)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void AgendaDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_AGENDA_TAB, m_TabControl);
}

BEGIN_MESSAGE_MAP(AgendaDialog, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_NOTIFY(TCN_SELCHANGE, IDC_AGENDA_TAB, &AgendaDialog::OnTcnSelchangeAgendaTab)
END_MESSAGE_MAP()


// AgendaDialog message handlers

BOOL AgendaDialog::OnInitDialog()
{
	switch (m_Settings.GetType())
	{
	case Settings::Type::WorkAndPlay:
	{
		m_TabControl.AddPage(new WorkAndPlayPage(m_Agenda, m_Settings, this), IDD_WORK_AND_PLAY_PAGE, "Vandaag");
		m_TabControl.AddPage(new OldFilesPage(m_Settings, this), IDD_OLDFILES_DIALOG, "Oud");
		m_TabControl.AddPage(new TotalsPage(m_Settings, this), IDD_TOTALS_PAGE, "Totalen");
		break;
	}
	case Settings::Type::OldAgenda:
	{
		m_TabControl.AddPage(new AgendaPage(m_Agenda, m_Settings, this), IDD_AGENDA_PAGE, _T("Vandaag"));
		m_TabControl.AddPage(new OldFilesPage(m_Settings, this), IDD_OLDFILES_DIALOG, _T("Oud"));
		m_TabControl.AddPage(new TotalsPage(m_Settings, this), IDD_TOTALS_PAGE, _T("Totalen"));
		break;
	}
	case Settings::Type::Combined:
	{
		m_TabControl.AddPage(new CombinedPage(m_Agenda, m_Settings, this), IDD_COMBINED_PAGE, _T("Combined"));
		m_TabControl.AddPage(new OldFilesPage(m_Settings, this), IDD_OLDFILES_DIALOG, _T("Oud"));
		m_TabControl.AddPage(new TotalsPage(m_Settings, this), IDD_TOTALS_PAGE, _T("Totalen"));
		break;
	}
	case Settings::Type::Both:
	{
		m_TabControl.AddPage(new WorkAndPlayPage(m_Agenda, m_Settings, this), IDD_WORK_AND_PLAY_PAGE, _T("WorkAndPlay"));
		m_TabControl.AddPage(new AgendaPage(m_Agenda, m_Settings, this), IDD_AGENDA_PAGE, _T("Agenda"));
		m_TabControl.AddPage(new OldFilesPage(m_Settings, this), IDD_OLDFILES_DIALOG, _T("Oud"));
		m_TabControl.AddPage(new TotalsPage(m_Settings, this), IDD_TOTALS_PAGE, _T("Totalen"));
		break;
	}

	default:
		throw std::runtime_error("Unknown agenda type");
	}

	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_TabControl.Initialize();
	m_TabControl.SelectPage(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void AgendaDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void AgendaDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR AgendaDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void AgendaDialog::OnTcnSelchangeAgendaTab(NMHDR * /*pNMHDR*/, LRESULT *pResult)
{
  // TODO: Add your control notification handler code here
  *pResult = 0;
}

// This method blocks <enter> and <escape> from closing the application
BOOL AgendaDialog::PreTranslateMessage(MSG* pMsg)
{
  if ((pMsg->message == WM_KEYDOWN))
  {
    if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
    {
      return TRUE;
    }
  }
  return CDialogEx::PreTranslateMessage(pMsg);
}