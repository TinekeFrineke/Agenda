// CombinedPage.cpp : implementation file
//

#include "stdafx.h"

#include "Agenda.h"
#include "CombinedPage.h"

#include <tchar.h>

#include <AgendaModel/AgendaUtilities.h>
#include <CustomControls/ItemListControl.h>
#include <Utilities/Date.h>
#include <Utilities/DateUtils.h>

#include "afxdialogex.h"
#include "AgendaSaver.h"
#include "EditItemDialog.h"
#include "FileLoader.h"
#include "Settings.h"
#include "SettingUtils.h"

BEGIN_MESSAGE_MAP(CombinedPage, CDialogEx)
    ON_BN_CLICKED(IDC_WORK, &CombinedPage::OnBnClickedWork)
    ON_BN_CLICKED(IDC_PLAY, &CombinedPage::OnBnClickedPlay)
    ON_BN_CLICKED(IDC_ADD, &CombinedPage::OnBnClickedAdd)
    ON_WM_TIMER()
    //  ON_WM_CREATE()
    ON_EN_CHANGE(IDC_HOUR, &CombinedPage::OnEnChangeHour)
    ON_EN_CHANGE(IDC_MINUTE, &CombinedPage::OnEnChangeMinute)
    ON_NOTIFY(UDN_DELTAPOS, IDC_HOURSPIN, &CombinedPage::OnDeltaposHourspin)
    ON_NOTIFY(UDN_DELTAPOS, IDC_MINUTESPIN, &CombinedPage::OnDeltaposMinutespin)
    ON_NOTIFY(NM_DBLCLK, IDC_ACTIVITYLIST, &CombinedPage::OnNMDblclkActivitylist)
    ON_WM_CREATE()
    ON_WM_SHOWWINDOW()
    ON_EN_SETFOCUS(IDC_HOUR, &CombinedPage::OnEnSetfocusHour)
    ON_BN_CLICKED(IDC_NOW, &CombinedPage::OnBnClickedNow)
    ON_EN_SETFOCUS(IDC_MINUTE, &CombinedPage::OnEnSetfocusMinute)
END_MESSAGE_MAP()

// CombinedPage dialog

IMPLEMENT_DYNAMIC(CombinedPage, CDialogEx)

CombinedPage::CombinedPage(Agenda::Agenda & agenda,
                           Settings& settings,
                           CWnd* pParent /*=NULL*/)
:   CDialogEx (IDD_COMBINED_PAGE, pParent),
    m_Today   (agenda),
    m_Settings(settings),
    m_Items   (agenda),
    m_TimerID (300000)
{
    if (!m_Settings.HasDefaultActivity(_T("Play")))
        m_Settings.AddDefaultActivity(_T("Play"), false);

    Utils::Date newdate(Utils::Date::Today());
    Agenda::Date Today(Utils::Date::ToSystemTime(newdate));
    std::vector<Agenda::Date> week(GetWeek(Today));

    auto toIgnore(SettingUtils::ActvitiesToIgnore(settings));

    FileLoader loader(m_Settings);
    for (const auto& date : week)
    {
        if (date == Today)
            continue;

        Agenda::Agenda oldagenda;
        loader.Load(date, oldagenda);
        m_WeekTotals += Agenda::GetWorkedTime(oldagenda, toIgnore);
    }
}

CombinedPage::~CombinedPage()
{
  //KillTimer(m_TimerID);
}

void CombinedPage::UpdateTime()
{
  UpdateTime(Agenda::Time::Now());
}

void CombinedPage::UpdateTime(const Agenda::Time & time)
{
    TCHAR text[1024];
    _stprintf_s(text, _T("%0d"), time.GetHour());
    m_Hour.SetWindowText(text);
    _stprintf_s(text, _T("%0d"), time.GetMinute());
    m_Minutes.SetWindowText(text);
}

BOOL CombinedPage::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_Timer = SetTimer(m_TimerID, 1000, NULL);
  UpdateTime();

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

void CombinedPage::RetrieveTime(Agenda::Time & time) const
{
  TCHAR text[1024];
  m_Hour.GetWindowText(text, 1024);
  int hours(_ttoi(text));
  time.Hour(hours);
  m_Minutes.GetWindowText(text, 1024);
  int minutes(_ttoi(text));
  time.Minute(minutes);
}

void CombinedPage::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ITEM_NAMES, m_Description);
    DDX_Control(pDX, IDC_ACTIVITYLIST, m_Items);
    DDX_Control(pDX, IDC_HOUR, m_Hour);
    DDX_Control(pDX, IDC_MINUTE, m_Minutes);
    DDX_Control(pDX, IDC_TOTAAL, m_Totaal);
    DDX_Control(pDX, IDC_WORK, m_WorkButton);
    DDX_Control(pDX, IDC_PLAY, m_PlayButton);
    DDX_Control(pDX, IDC_WEEK, m_Week);
    DDX_Control(pDX, IDC_NOW, m_Now);
}



// CombinedPage message handlers
void CombinedPage::OnBnClickedAdd()
{
    TCHAR text[1024];
    m_Description.GetWindowText(text, 1024);

    AddItem(text);
}

void CombinedPage::OnBnClickedWork()
{
    AddItem(_T("Work"));
    WriteAgenda();
}

void CombinedPage::OnBnClickedPlay()
{
    AddItem(_T("Play"));
    WriteAgenda();
}

void CombinedPage::AddItem(const std::tstring& item)
{
    Agenda::Time time;

    try {
        RetrieveTime(time);
    }
    catch (std::runtime_error& error) {
        MessageBox(Str::ToTString(error.what()).c_str(), _T("ERROR READING TIME"), MB_OK);
        return;
    }

    SYSTEMTIME stime;
    GetLocalTime(&stime);
    stime.wHour = static_cast<unsigned short>(time.GetHour());
    stime.wMinute = static_cast<unsigned short>(time.GetMinute());

    m_Today.Add(Agenda::Item(stime, item), true);
    if (m_Description.FindString(0, item.c_str()) == CB_ERR)
      m_Description.InsertString(0, item.c_str());

    if (item != L"Work" && item != L"Play")
        m_Description.SelectString(0, item.c_str());
  
    UpdateView();
}

void CombinedPage::WriteAgenda()
{
    AgendaSaver Saver(m_Settings);
    Utils::Date newdate(Utils::Date::Today());
    Agenda::Date Today(Utils::Date::ToSystemTime(newdate));

    if (!Saver.Save(Today, m_Today))
    {
        MessageBox(_T("Could not save agenda"), _T("ERROR WRITING AGENDA"), MB_OK);
    }
}

void CombinedPage::StallAutomaticUpdate()
{
    // When the user edits the time, stretch the update of the time fields
    KillTimer(m_TimerID);
    m_Timer = SetTimer(m_TimerID, 60000, NULL);
}

std::vector<Agenda::Date> CombinedPage::GetWeek(const Agenda::Date& today)
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


void CombinedPage::OnTimer(UINT_PTR nIDEvent)
{
  UpdateTime();
  __super::OnTimer(nIDEvent);
  m_Timer = SetTimer(m_TimerID, 1000, NULL);
}


void CombinedPage::OnEnChangeHour()
{
  // When the user edits the time, stretch the update of the time fields
  StallAutomaticUpdate();
}


void CombinedPage::OnEnChangeMinute()
{
  // When the user edits the time, stretch the update of the time fields
  StallAutomaticUpdate();
}


void CombinedPage::OnDeltaposHourspin(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

  Agenda::Time time;
  RetrieveTime(time);
  time -= pNMUpDown->iDelta * 60;
  UpdateTime(time);

  *pResult = 0;
}


void CombinedPage::OnDeltaposMinutespin(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

  Agenda::Time time;
  RetrieveTime(time);
  time -= pNMUpDown->iDelta;
  UpdateTime(time);

  *pResult = 0;
}

void CombinedPage::UpdateView()
{
  m_Items.View(m_Today);
  Agenda::Time totalTime = Agenda::GetWorkedTime(m_Today, SettingUtils::ActvitiesToIgnore(m_Settings));

  m_Totaal.SetWindowText(totalTime.String().c_str());

  Agenda::TotalTime weekTotals(m_WeekTotals);
  weekTotals += totalTime;
  m_Week.SetWindowText(weekTotals.String().c_str());
}


void CombinedPage::OnNMDblclkActivitylist(NMHDR *pNMHDR, LRESULT *pResult)
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
  WriteAgenda();

    *pResult = 0;
}

int CombinedPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}


void CombinedPage::OnEnSetfocusHour()
{
    StallAutomaticUpdate();
}


void CombinedPage::OnBnClickedNow()
{
    UpdateTime();
}


void CombinedPage::OnEnSetfocusMinute()
{
    StallAutomaticUpdate();
}
