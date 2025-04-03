// AgendaPage.cpp : implementation file
//

#include "stdafx.h"

#include "Agenda.h"
#include "WorkAndPlayPage.h"

#include <AgendaModel/AgendaUtilities.h>
#include <CustomControls/ItemListControl.h>
#include <Utilities/Date.h>

#include "afxdialogex.h"
#include "AgendaSaver.h"
#include "EditItemDialog.h"
#include "FileLoader.h"
#include "Settings.h"
#include "SettingUtils.h"


BEGIN_MESSAGE_MAP(WorkAndPlayPage, CDialog)
    ON_BN_CLICKED(IDC_WORK, &WorkAndPlayPage::OnBnClickedWork)
    ON_BN_CLICKED(IDC_PLAY, &WorkAndPlayPage::OnBnClickedPlay)
    ON_WM_TIMER()
    //  ON_WM_CREATE()
    ON_EN_CHANGE(IDC_HOUR, &WorkAndPlayPage::OnEnChangeHour)
    ON_EN_CHANGE(IDC_MINUTE, &WorkAndPlayPage::OnEnChangeMinute)
    ON_NOTIFY(UDN_DELTAPOS, IDC_HOURSPIN, &WorkAndPlayPage::OnDeltaposHourspin)
    ON_NOTIFY(UDN_DELTAPOS, IDC_MINUTESPIN, &WorkAndPlayPage::OnDeltaposMinutespin)
    ON_NOTIFY(NM_DBLCLK, IDC_ACTIVITYLIST, &WorkAndPlayPage::OnNMDblclkActivitylist)
    ON_WM_CREATE()
    ON_WM_SHOWWINDOW()
    ON_EN_SETFOCUS(IDC_HOUR, &WorkAndPlayPage::OnEnSetfocusHour)
    ON_BN_CLICKED(IDC_NOW, &WorkAndPlayPage::OnBnClickedNow)
    ON_EN_SETFOCUS(IDC_MINUTE, &WorkAndPlayPage::OnEnSetfocusMinute)
END_MESSAGE_MAP()

// AgendaPage dialog

IMPLEMENT_DYNAMIC(WorkAndPlayPage, CDialog)

WorkAndPlayPage::WorkAndPlayPage(Agenda::Agenda& agenda,
                                 Settings& settings,
                                 CWnd* pParent /*=NULL*/)
    : CDialog(IDD_WORK_AND_PLAY_PAGE, pParent),
    m_Today(agenda),
    m_Settings(settings),
    m_Items(agenda),
    m_TimerID(300000)
{
    if (!m_Settings.HasDefaultActivity(_T("Play")))
        m_Settings.AddDefaultActivity(_T("Play"), false);

    Utils::Date today(Utils::Today());
    std::vector<Utils::Date> week(GetWeek(today));

    auto toIgnore(SettingUtils::ActvitiesToIgnore(settings));

    FileLoader loader(m_Settings);
    for (const auto& date : week)
    {
        if (date == today)
            continue;

        Agenda::Agenda oldagenda;
        loader.Load(date, oldagenda);
        m_WeekTotals += Agenda::GetWorkedTime(oldagenda, toIgnore);
    }
}

WorkAndPlayPage::~WorkAndPlayPage()
{
    //KillTimer(m_TimerID);
}

void WorkAndPlayPage::UpdateTime()
{
    UpdateTime(Agenda::Time::Now());
}

void WorkAndPlayPage::UpdateTime(const Agenda::Time& time)
{
    TCHAR text[1024];
    _stprintf_s(text, _T("%0d"), time.GetHour());
    m_Hour.SetWindowText(text);
    _stprintf_s(text, _T("%0d"), time.GetMinute());
    m_Minutes.SetWindowText(text);
}

BOOL WorkAndPlayPage::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_Timer = SetTimer(m_TimerID, 1000, NULL);
    UpdateTime();

    // Read yesterday's items and add them:
    Path agendapath(m_Settings.GetDataPath() + "*.age");
    std::string todayfile(Agenda::Date().String() + ".age");
    std::string yesterdayfile;

    WIN32_FIND_DATA finddata;
    HANDLE hFile = FindFirstFile(agendapath.AsString().c_str(), &finddata);
    if (hFile != INVALID_HANDLE_VALUE) {
        std::string latestfile(finddata.cFileName);
        if (latestfile != std::string(Agenda::Date().String()) + ".age") {
            if (yesterdayfile.empty() || latestfile > yesterdayfile)
                yesterdayfile = latestfile;
        }

        while (FindNextFile(hFile, &finddata) != FALSE) {
            latestfile = finddata.cFileName;
            if (latestfile != std::string(Agenda::Date().String()) + ".age") {
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

void WorkAndPlayPage::RetrieveTime(Agenda::Time& time) const
{
    // TODO: Add your control notification handler code here
    char text[1024];
    m_Hour.GetWindowText(text, 1024);
    int hours(atoi(text));
    time.Hour(hours);
    m_Minutes.GetWindowText(text, 1024);
    int minutes(atoi(text));
    time.Minute(minutes);
}

void WorkAndPlayPage::DoDataExchange(CDataExchange* pDX)
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



void WorkAndPlayPage::OnBnClickedWork()
{
    AddItem("Work");
    WriteAgenda();
}

void WorkAndPlayPage::OnBnClickedPlay()
{
    AddItem("Play");
    WriteAgenda();
}

void WorkAndPlayPage::AddItem(const std::string& Item)
{
    Agenda::Time time;

    try {
        RetrieveTime(time);
    }
    catch (std::runtime_error& error) {
        MessageBox(error.what(), "ERROR READING TIME", MB_OK);
        return;
    }

    SYSTEMTIME stime;
    GetLocalTime(&stime);
    stime.wHour = static_cast<unsigned short>(time.GetHour());
    stime.wMinute = static_cast<unsigned short>(time.GetMinute());

    m_Today.Add(Agenda::Item(stime, Item), true);

    UpdateView();
}

void WorkAndPlayPage::WriteAgenda()
{
    AgendaSaver Saver(m_Settings);
    Utils::Date today(Utils::Today());

    if (!Saver.Save(today, m_Today))
    {
        MessageBox("Could not save agenda", "ERROR WRITING AGENDA", MB_OK);
    }
}

void WorkAndPlayPage::StallAutomaticUpdate()
{
    // When the user edits the time, stretch the update of the time fields
    KillTimer(m_TimerID);
    m_Timer = SetTimer(m_TimerID, 60000, NULL);
}

std::vector<Utils::Date> WorkAndPlayPage::GetWeek(const Utils::Date& today)
{
    std::vector<Utils::Date> dates;

    const Utils::Date sunday(Utils::GetSundayBefore(today));
    Utils::Date startDate(sunday);

    for (size_t i = 0; i < 6; ++i)
    {
        startDate.AddDays(1);
        dates.push_back(startDate);
    }

    return dates;
}


void WorkAndPlayPage::OnTimer(UINT_PTR nIDEvent)
{
    UpdateTime();
    __super::OnTimer(nIDEvent);
    m_Timer = SetTimer(m_TimerID, 1000, NULL);
}


void WorkAndPlayPage::OnEnChangeHour()
{
    // When the user edits the time, stretch the update of the time fields
    StallAutomaticUpdate();
}


void WorkAndPlayPage::OnEnChangeMinute()
{
    // When the user edits the time, stretch the update of the time fields
    StallAutomaticUpdate();
}


void WorkAndPlayPage::OnDeltaposHourspin(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    Agenda::Time time;
    RetrieveTime(time);
    time -= pNMUpDown->iDelta * 60;
    UpdateTime(time);

    *pResult = 0;
}


void WorkAndPlayPage::OnDeltaposMinutespin(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    Agenda::Time time;
    RetrieveTime(time);
    time -= pNMUpDown->iDelta;
    UpdateTime(time);

    *pResult = 0;
}

void WorkAndPlayPage::UpdateView()
{
    m_Items.View(m_Today);
    Agenda::Time totalTime = Agenda::GetWorkedTime(m_Today, SettingUtils::ActvitiesToIgnore(m_Settings));

    m_Totaal.SetWindowText(totalTime.String().c_str());

    Agenda::TotalTime weekTotals(m_WeekTotals);
    weekTotals += totalTime;
    m_Week.SetWindowText(weekTotals.String().c_str());
}


void WorkAndPlayPage::OnNMDblclkActivitylist(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    if (pNMItemActivate->iItem == -1) {
        *pResult = 0;
        return;
    }

    AgendaItemListItem* pItem(reinterpret_cast<AgendaItemListItem*>(m_Items.GetItemData(pNMItemActivate->iItem)));
    Agenda::Item& item(pItem->GetItem());

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

int WorkAndPlayPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}


void WorkAndPlayPage::OnEnSetfocusHour()
{
    StallAutomaticUpdate();
}


void WorkAndPlayPage::OnBnClickedNow()
{
    UpdateTime();
}


void WorkAndPlayPage::OnEnSetfocusMinute()
{
    StallAutomaticUpdate();
}
