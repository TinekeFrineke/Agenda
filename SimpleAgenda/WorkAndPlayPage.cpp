// AgendaPage.cpp : implementation file
//

#include "stdafx.h"

#include "WorkAndPlay.h"
#include "WorkAndPlayPage.h"

#include <tchar.h>

#include <CustomControls/ItemListControl.h>
#include <Utilities/Date.h>
#include <Utilities/DateUtils.h>

#include "AgendaModel/AgendaUtilities.h"

#include "afxdialogex.h"
#include "EditItemDialog.h"
#include "FileLoader.h"
#include "Settings.h"
#include "SettingUtils.h"

BEGIN_MESSAGE_MAP(AgendaPage, CDialog)
    ON_BN_CLICKED(IDC_WORK, &AgendaPage::OnBnClickedWork)
    ON_BN_CLICKED(IDC_PLAY, &AgendaPage::OnBnClickedPlay)
    ON_WM_TIMER()
    //  ON_WM_CREATE()
    ON_EN_CHANGE(IDC_HOUR, &AgendaPage::OnEnChangeHour)
    ON_EN_CHANGE(IDC_MINUTE, &AgendaPage::OnEnChangeMinute)
    ON_NOTIFY(UDN_DELTAPOS, IDC_HOURSPIN, &AgendaPage::OnDeltaposHourspin)
    ON_NOTIFY(UDN_DELTAPOS, IDC_MINUTESPIN, &AgendaPage::OnDeltaposMinutespin)
    ON_NOTIFY(NM_DBLCLK, IDC_ACTIVITYLIST, &AgendaPage::OnNMDblclkActivitylist)
    ON_WM_CREATE()
    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// AgendaPage dialog

IMPLEMENT_DYNAMIC(AgendaPage, CDialog)

AgendaPage::AgendaPage(Agenda::Agenda & agenda,
                       Settings& settings,
                       CWnd* pParent /*=NULL*/)
:   CDialog   (IDD_AGENDA_PAGE, pParent),
    m_Today   (agenda),
    m_Settings(settings),
    m_Items   (agenda),
    m_TimerID (300000)
{
    Utils::Date newdate(Utils::Date::Today());
    Agenda::Date Today(Utils::Date::ToSystemTime(newdate));
    std::vector<Agenda::Date> week(GetWeek(Today));

    auto toIgnore(SettingUtils::ActvitiesToIgnore(settings));

    FileLoader loader(m_Settings);
    for (const auto& date : week)
    {
        Agenda::Agenda oldagenda;
        loader.LoadAgenda(date, oldagenda);
        m_WeekTotals += Agenda::GetWorkedTime(oldagenda, toIgnore);
    }
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
    DDX_Control(pDX, IDC_ACTIVITYLIST, m_Items);
    DDX_Control(pDX, IDC_HOUR, m_Hour);
    DDX_Control(pDX, IDC_MINUTE, m_Minutes);
    DDX_Control(pDX, IDC_TOTAAL, m_Totaal);
    DDX_Control(pDX, IDC_WORK, m_WorkButton);
    DDX_Control(pDX, IDC_PLAY, m_PlayButton);
    DDX_Control(pDX, IDC_WEEK, m_Week);
}



// AgendaPage message handlers



void AgendaPage::OnBnClickedWork()
{
    AddItem(_T("Work"));
}

void AgendaPage::OnBnClickedPlay()
{
    AddItem(_T("Play"));
}

void AgendaPage::AddItem(const std::tstring& Item)
{
    Agenda::Time time;

    try {
        RetrieveTimer(time);
    }
    catch (std::runtime_error& error) {
        MessageBox(Str::ToTString(error.what()).c_str(), _T("ERROR READING TIME"), MB_OK);
        return;
    }

    SYSTEMTIME stime;
    GetLocalTime(&stime);
    stime.wHour = static_cast<unsigned short>(time.GetHour());
    stime.wMinute = static_cast<unsigned short>(time.GetMinute());

    m_Today.Add(Agenda::Item(stime, Item), true);

    UpdateView();
}

std::vector<Agenda::Date> AgendaPage::GetWeek(const Agenda::Date& today)
{
    std::vector<Agenda::Date> dates;

    SYSTEMTIME stime(today.ToSystemTime());
    SYSTEMTIME sunday(Date::GetSundayBefore(stime));
    Utils::Date startDate(Utils::Date::FromSystemTime(sunday));

    for (size_t i = 0; i < 6; ++i)
    {
        startDate.AddDays(1);
        dates.push_back(Agenda::Date(Utils::Date::ToSystemTime(startDate)));
    }

    return dates;
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
  Agenda::Time totalTime = Agenda::GetWorkedTime(m_Today, SettingUtils::ActvitiesToIgnore(m_Settings));

  //typedef std::map<std::tstring, Agenda::Time, Str::ci_less> TotalsMap;
  //TotalsMap totalsMap;
  //Agenda::GetTotals(m_Today, totalsMap);

  //Agenda::Time totalTime;
  //for (TotalsMap::const_iterator iter = totalsMap.begin(); iter != totalsMap.end(); ++iter)
  //  if (!m_Settings.HasDefaultActivity(iter->first)
  //      || m_Settings.ShouldAddDefeaultActivity(iter->first))
  //    totalTime += iter->second;

  m_Totaal.SetWindowText(totalTime.String().c_str());

  Agenda::Time weekTotals(totalTime + m_WeekTotals);
  m_Week.SetWindowText(weekTotals.String().c_str());
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

int AgendaPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here

    return 0;
}
