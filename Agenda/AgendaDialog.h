
// AgendaDialog.h : header file
//

#pragma once
#include "afxcmn.h"

#include <CustomControls/TabControl.h>

#include "../AgendaModel/Agenda.h"


class Settings;

// AgendaDialog dialog
class AgendaDialog : public CDialogEx
{
// Construction
public:
	AgendaDialog(Agenda::Agenda & agenda, Settings & settings, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGENDA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL            OnInitDialog() override;
  //virtual void            OnCancel() override {}
  //virtual void            OnOK() override {}
  virtual BOOL PreTranslateMessage(MSG* pMsg) override;

  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnTcnSelchangeAgendaTab(NMHDR *pNMHDR, LRESULT *pResult);

private:
  TabControl        m_TabControl;
  Agenda::Agenda &  m_Agenda;
  Settings &        m_Settings;
};
