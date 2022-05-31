// TotalsPage.cpp : implementation file
//

#include "stdafx.h"
#include "Agenda.h"
#include "TotalsPage.h"
#include "afxdialogex.h"

#include "Settings.h"


// TotalsPage dialog

IMPLEMENT_DYNAMIC(TotalsPage, CDialog)

TotalsPage::TotalsPage(Settings & settings, CWnd* pParent /*=NULL*/)
: CDialog(IDD_TOTALS_PAGE, pParent),
  m_Settings(settings)
{

}

TotalsPage::~TotalsPage()
{
}

BOOL TotalsPage::OnInitDialog()
{
  if (CDialog::OnInitDialog() == FALSE)
    return FALSE;

  m_StartDate.SetTime(m_Settings.GetDefaultStartDate().ToSystemTime());
  m_EndDate.SetTime(m_Settings.GetDefaultEndDate().ToSystemTime());

  m_Totals.Initialize();
  m_Totals.SetSelectionMark(0);
  m_Totals.ShowWindow(SW_SHOW);

  return 0;
}

void TotalsPage::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_STARTDATE, m_StartDate);
  DDX_Control(pDX, IDC_ENDDATE, m_EndDate);
  DDX_Control(pDX, IDC_LIST1, m_Totals);
}


BEGIN_MESSAGE_MAP(TotalsPage, CDialog)
//  ON_BN_CLICKED(IDC_BUTTON1, &TotalsPage::OnBnClickedButton1)
ON_BN_CLICKED(IDC_CALCULATE_BUTTON, &TotalsPage::OnBnClickedCalculateButton)
END_MESSAGE_MAP()


// TotalsPage message handlers


void TotalsPage::OnBnClickedCalculateButton()
{
  SYSTEMTIME stime;

  memset(&stime, 0, sizeof(stime));
  m_StartDate.GetTime(&stime);
  m_Settings.SetDefaultStartDate(Agenda::Date(stime));

  memset(&stime, 0, sizeof(stime));
  m_EndDate.GetTime(&stime);
  m_Settings.SetDefaultEndDate(Agenda::Date(stime));

  Agenda::Agenda::ItemTimeMap timemap;

  Path path(m_Settings.GetDataPath());
  path += _T("*.age");
  WIN32_FIND_DATA finddata;
  memset(&finddata, 0, sizeof(WIN32_FIND_DATA));
  HANDLE hFile = FindFirstFile(path.AsString().c_str(), &finddata);
  if (hFile !=  INVALID_HANDLE_VALUE) {
    if (_tcsicmp(finddata.cFileName, (m_Settings.GetDefaultStartDate().String() + _T(".age")).c_str()) >= 0
        && _tcsicmp(finddata.cFileName, (m_Settings.GetDefaultEndDate().String() + _T(".age")).c_str()) <= 0) {
      Agenda::Agenda agenda;
      m_Settings.LoadAgenda(agenda, m_Settings.GetDataPath() + finddata.cFileName);
      agenda.GetTimeItemMap(timemap);
    }
    while (FindNextFile(hFile, &finddata) != FALSE) {
      if (_tcsicmp(finddata.cFileName, (m_Settings.GetDefaultStartDate().String() + _T(".age")).c_str()) >= 0
          && _tcsicmp(finddata.cFileName, (m_Settings.GetDefaultEndDate().String() + _T(".age")).c_str()) <= 0) {
        Agenda::Agenda agenda;
        m_Settings.LoadAgenda(agenda, m_Settings.GetDataPath() + finddata.cFileName);
        agenda.GetTimeItemMap(timemap);
      }
    }

    FindClose(hFile);
  }

  m_Totals.FillFrom(timemap);
}
