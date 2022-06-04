#pragma once

#include <CustomControls/TabPage.h>

#include "../AgendaModel/Agenda.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "AgendaItemList.h"

// AgendaPage dialog

// The page upxdates the time from time to time:
// Normally every second, but if the user has typed anything anywhere, or if he has
// selected an item from the combobox, a one minute delay takes effect.

class Settings;

class AgendaPage : public CDialog, public TabPage
{
	DECLARE_DYNAMIC(AgendaPage)

public:
	AgendaPage(Agenda::Agenda & agenda,
			   Settings & settings,
			   CWnd* pParent = NULL);   // standard constructor
	virtual ~AgendaPage();

	virtual CDialog *       GetDialog() override { return this; }
	virtual BOOL            OnInitDialog() override;
	virtual void            OnCancel() override {}
	virtual void            OnOK() override {}

	void                    UpdateTimer();
	void                    UpdateTimer(const Agenda::Time & time);
	void                    RetrieveTimer(Agenda::Time & time) const;

  // Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGENDA_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedWork();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeHour();
	afx_msg void OnEnChangeMinute();
	afx_msg void OnDeltaposHourspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposMinutespin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkActivitylist(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void UpdateView();
	void AddItem(const std::tstring& Item);

	static std::vector<Agenda::Date> GetWeek(const Agenda::Date& today);

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

	Agenda::Time m_WeekTotals;

	CButton m_WorkButton;
	CButton m_PlayButton;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
