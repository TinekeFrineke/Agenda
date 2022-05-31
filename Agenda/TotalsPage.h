#pragma once

#include <CustomControls/TabPage.h>
#include "afxdtctl.h"
#include "AgendaItemList.h"

#include "AgendaTotalsList.h"

// TotalsPage dialog
class Settings;

class TotalsPage : public CDialog, public TabPage
{
	DECLARE_DYNAMIC(TotalsPage)

public:
	TotalsPage(Settings & settings, CWnd* pParent = NULL);   // standard constructor
	virtual ~TotalsPage();

  virtual CDialog *       GetDialog() override { return this; }
	virtual BOOL            OnInitDialog() override;

  virtual void      OnCancel() override {}
  virtual void      OnOK() override {}

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOTALS_PAGE };
#endif

  afx_msg void OnBnClickedCalculateButton();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  Settings &            m_Settings;
  CDateTimeCtrl         m_StartDate;
  CDateTimeCtrl         m_EndDate;
  AgendaTotalsList      m_Totals;
};
