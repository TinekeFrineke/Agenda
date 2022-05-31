#pragma once

#include <CustomControls/TabPage.h>
#include "AgendaFileListBox.h"
#include "OldAgendaList.h"

// OldFilesPage dialog

class OldFilesPage : public CDialog, public TabPage
{
	DECLARE_DYNAMIC(OldFilesPage)
public:
	OldFilesPage(const Settings & settings, CWnd* pParent = NULL);   // standard constructor
	virtual ~OldFilesPage();

  virtual CDialog *       GetDialog() override { return this; }
	virtual BOOL            OnInitDialog() override;
  virtual void            OnCancel() override {}
  virtual void            OnOK() override {}

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OLDFILES_DIALOG };
#endif

  afx_msg void OnLbnSelchangeOldfilesList();
  afx_msg void OnLbnDblclkOldfilesList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  void                  UpdateToSelection();

  const Settings &      m_Settings;
  AgendaFileListBox     m_FileList;

private:
  OldAgendaList         m_OldAgenda;
};
