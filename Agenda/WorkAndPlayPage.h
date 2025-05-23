#pragma once

#include <AgendaModel/Agenda.h>
#include <AgendaModel/TotalTime.h>
#include <CustomControls/TabPage.h>

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "AgendaItemList.h"

namespace Utils {
class Date;
}

// AgendaPage dialog

// The page upxdates the time from time to time:
// Normally every second, but if the user has typed anything anywhere, or if he has
// selected an item from the combobox, a one minute delay takes effect.

class Settings;

class WorkAndPlayPage : public CDialog, public TabPage
{
	DECLARE_DYNAMIC(WorkAndPlayPage)

public:
	WorkAndPlayPage(Agenda::Agenda & agenda,
				    Settings & settings,
				    CWnd* pParent = NULL);   // standard constructor
	virtual ~WorkAndPlayPage();

	virtual CDialog *       GetDialog() override { return this; }
	virtual BOOL            OnInitDialog() override;
	virtual void            OnCancel() override {}
	virtual void            OnOK() override {}

	void                    UpdateTime();
	void                    UpdateTime(const Agenda::Time & time);
	void                    RetrieveTime(Agenda::Time & time) const;

  // Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORK_AND_PLAY_PAGE };
#endif

	afx_msg void OnBnClickedWork();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeHour();
	afx_msg void OnEnChangeMinute();
	afx_msg void OnDeltaposHourspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposMinutespin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkActivitylist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnSetfocusHour();
	afx_msg void OnBnClickedNow();
	afx_msg void OnEnSetfocusMinute();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void UpdateView();
	void AddItem(const std::string& Item);
	void WriteAgenda();
	// Makes sure the timer is not automatically updated in the next minute
	void StallAutomaticUpdate();

	static std::vector<Utils::Date> GetWeek(const Utils::Date& today);

	Agenda::Agenda &    m_Today;
	Settings &          m_Settings;

	CComboBox           m_Description;
	AgendaItemList      m_Items;
	CEdit               m_Hour;
	CEdit               m_Minutes;

	const UINT_PTR      m_TimerID;
	UINT_PTR            m_Timer;
	CEdit               m_Totaal;
	CEdit               m_Week;

	Agenda::TotalTime m_WeekTotals;

	CButton m_WorkButton;
	CButton m_PlayButton;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
