// AgendaPage.cpp : implementation file
//

#include "stdafx.h"

#include "Agenda.h"
#include "AgendaPage.h"

#include <tchar.h>

#include <CustomControls/ItemListControl.h>

#include "../AgendaModel/AgendaUtilities.h"

#include "afxdialogex.h"
#include "EditItemDialog.h"
#include "Settings.h"


// AgendaPage dialog

IMPLEMENT_DYNAMIC(AgendaPage, CDialog)

AgendaPage::AgendaPage(Agenda::Agenda & agenda, Settings & settings, CWnd* pParent /*=NULL*/)
: CDialog   (IDD_AGENDA_PAGE, pParent),
  m_Today   (agenda),
  m_Settings(settings),
  m_Items   (agenda),
  m_TimerID (300000)
{
}

AgendaPage::~AgendaPage()
{
  //KillTimer(m_TimerID);
}

void AgendaPage::UpdateTimer()
{
  UpdateTimer(Agenda::Time::Now());
}

void AgendaPage::UpdateTimer(const Agenda::Time & time)
{
  TCHAR text[1024];
  _stprintf_s(text, _T("%0d"), time.GetHour());
  m_Hour.SetWindowText(text);
  _stprintf_s(text, _T("%0d"), time.GetMinute());
  m_Minutes.SetWindowText(text);
}

BOOL AgendaPage::OnInitDialog()
{
	CDialog::OnInitDialog();
  m_Timer = SetTimer(m_TimerID, 1000, NULL);
  UpdateTimer();

  // Fill the activities list:
  for (Agenda::Agenda::ItemList::iterator iter = m_Today.Begin(); iter != m_Today.End(); ++iter)
    if (m_Description.FindString(0, iter->GetTaskName().c_str()) == CB_ERR)
      m_Description.AddString(iter->GetTaskName().c_str());

  for (size_t i = 0; i < m_Settings.GetDefaultActivities().size(); ++i)
    if (m_Description.FindString(0, m_Settings.GetDefaultActivities()[i].m_Description.c_str()) == CB_ERR)
      m_Description.AddString(m_Settings.GetDefaultActivities()[i].m_Description.c_str());

  // Read yesterday's items and add them:
  Path agendapath(m_Settings.GetDataPath() + _T("*.age"));
  std::tstring todayfile(Agenda::Date().String() + _T(".age"));
  std::tstring yesterdayfile;

  WIN32_FIND_DATA finddata;
  HANDLE hFile = FindFirstFile(agendapath.AsString().c_str(), &finddata);
  if (hFile !=  INVALID_HANDLE_VALUE) {
    std::tstring latestfile(finddata.cFileName);
    if (latestfile != std::tstring(Agenda::Date().String()) + _T(".age")) {
      if (yesterdayfile.empty() || latestfile > yesterdayfile)
        yesterdayfile = latestfile;
    }

    while (FindNextFile(hFile, &finddata) != FALSE) {
      latestfile = finddata.cFileName;
      if (latestfile != std::tstring(Agenda::Date().String()) + _T(".age")) {
        if (yesterdayfile.empty() || latestfile > yesterdayfile)
          yesterdayfile = latestfile;
      }
    }
  }

  Agenda::Agenda yesterday;

  if (!yesterdayfile.empty()) {
    Path yesterdaypath(m_Settings.GetDataPath() + yesterdayfile);
    // Create the current agenda item
    m_Settings.LoadAgenda(yesterday, yesterdaypath);

    for (Agenda::Agenda::ItemList::iterator iter = yesterday.Begin(); iter != yesterday.End(); ++iter)
      if (m_Description.FindString(0, iter->GetTaskName().c_str()) == CB_ERR)
        m_Description.AddString(iter->GetTaskName().c_str());
  }

  if (!m_Today.Empty())
    m_Description.SelectString(0, m_Today.Begin()->GetTaskName().c_str());
  else if (!yesterday.Empty())
    m_Description.SelectString(0, yesterday.Begin()->GetTaskName().c_str());

  m_Items.Initialize();
  m_Items.SetSelectionMark(0);
  m_Items.ShowWindow(SW_SHOW);
  
  UpdateView();

  return TRUE;
}

void AgendaPage::RetrieveTimer(Agenda::Time & time) const
{
  // TODO: Add your control notification handler code here
  TCHAR text[1024];
  m_Hour.GetWindowText(text, 1024);
  int hours(_ttoi(text));
  time.Hour(hours);
  m_Minutes.GetWindowText(text, 1024);
  int minutes(_ttoi(text));
  time.Minute(minutes);
}

void AgendaPage::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO1, m_Description);
  DDX_Control(pDX, IDC_ACTIVITYLIST, m_Items);
  DDX_Control(pDX, IDC_HOUR, m_Hour);
  DDX_Control(pDX, IDC_MINUTE, m_Minutes);
  DDX_Control(pDX, IDC_TOTAAL, m_Totaal);
}


BEGIN_MESSAGE_MAP(AgendaPage, CDialog)
  ON_BN_CLICKED(IDC_ADD, &AgendaPage::OnBnClickedAdd)
  ON_WM_TIMER()
//  ON_WM_CREATE()
  ON_EN_CHANGE(IDC_HOUR, &AgendaPage::OnEnChangeHour)
  ON_EN_CHANGE(IDC_MINUTE, &AgendaPage::OnEnChangeMinute)
  ON_NOTIFY(UDN_DELTAPOS, IDC_HOURSPIN, &AgendaPage::OnDeltaposHourspin)
  ON_NOTIFY(UDN_DELTAPOS, IDC_MINUTESPIN, &AgendaPage::OnDeltaposMinutespin)
  ON_NOTIFY(NM_DBLCLK, IDC_ACTIVITYLIST, &AgendaPage::OnNMDblclkActivitylist)
END_MESSAGE_MAP()


// AgendaPage message handlers


void AgendaPage::OnBnClickedAdd()
{
  // TODO: Add your control notification handler code here
  TCHAR text[1024];
  m_Description.GetWindowText(text, 1024);

  Agenda::Time time;

  try {
    RetrieveTimer(time);
  }
  catch(std::runtime_error& error) {
    MessageBox(Str::ToTString(error.what()).c_str(), _T("ERROR READING TIME"), MB_OK);
    return;
  }

  SYSTEMTIME stime;
  GetLocalTime(&stime);
  stime.wHour = static_cast<unsigned short>(time.GetHour());
  stime.wMinute = static_cast<unsigned short>(time.GetMinute());

  m_Today.Add(Agenda::Item(stime, text), true);
  if (m_Description.FindString(0, text) == CB_ERR)
    m_Description.InsertString(0, text);

  m_Description.SelectString(0, text);
  
  UpdateView();
}


void AgendaPage::OnTimer(UINT_PTR nIDEvent)
{
  UpdateTimer();
  __super::OnTimer(nIDEvent);
  m_Timer = SetTimer(m_TimerID, 1000, NULL);
}


void AgendaPage::OnEnChangeHour()
{
  // When the user edits the time, stretch the update of the time fields
  KillTimer(m_TimerID);
  m_Timer = SetTimer(m_TimerID, 60000, NULL);
}


void AgendaPage::OnEnChangeMinute()
{
  // When the user edits the time, stretch the update of the time fields
  KillTimer(m_TimerID);
  m_Timer = SetTimer(m_TimerID, 60000, NULL);
}


void AgendaPage::OnDeltaposHourspin(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

  Agenda::Time time;
  RetrieveTimer(time);
  time -= pNMUpDown->iDelta * 60;
  UpdateTimer(time);

  *pResult = 0;
}


void AgendaPage::OnDeltaposMinutespin(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

  Agenda::Time time;
  RetrieveTimer(time);
  time -= pNMUpDown->iDelta;
  UpdateTimer(time);

  *pResult = 0;
}

void AgendaPage::UpdateView()
{
  m_Items.View(m_Today);
  typedef std::map<std::tstring, Agenda::Time, Str::ci_less> TotalsMap;
  TotalsMap totalsMap;
  Agenda::GetTotals(m_Today, totalsMap);

  Agenda::Time totalTime;
  for (TotalsMap::const_iterator iter = totalsMap.begin(); iter != totalsMap.end(); ++iter)
    if (!m_Settings.HasDefaultActivity(iter->first)
        || m_Settings.ShouldAddDefeaultActivity(iter->first))
      totalTime += iter->second;

  m_Totaal.SetWindowText(totalTime.String().c_str());
}


void AgendaPage::OnNMDblclkActivitylist(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
  if (pNMItemActivate->iItem == -1) {
    *pResult = 0;
    return;
  }

  AgendaItemListItem * pItem(reinterpret_cast<AgendaItemListItem *>(m_Items.GetItemData(pNMItemActivate->iItem)));
  Agenda::Item & item(pItem->GetItem());

  EditItemDialog dialog(item, this);
  INT_PTR nResponse = dialog.DoModal();
  switch (nResponse) {
    case IDOK:
    break;
    case IDCANCEL:
    break;
    default:
		  TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		  TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
    break;
	}

  UpdateView();

  // TODO: Add your control notification handler code here
  *pResult = 0;
}
