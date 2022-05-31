#pragma once
#include "AgendaItemList.h"

class Settings;

namespace Agenda { class Agenda; }

// EditAgendaDialog dialog

class EditAgendaDialog : public CDialog
{
	DECLARE_DYNAMIC(EditAgendaDialog)

public:
	EditAgendaDialog(const Agenda::Agenda & agenda, CWnd* pParent = NULL);   // standard constructor
	virtual ~EditAgendaDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_AGENDA_DIALOG };
#endif

	virtual BOOL            OnInitDialog() override;
  afx_msg void            OnBnClickedOk();
  afx_msg void            OnBnClickedCancel();
  afx_msg void            OnNMDblclkItemList(NMHDR *pNMHDR, LRESULT *pResult);

  const Agenda::Agenda &  GetAgenda() const { return m_Agenda; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  Agenda::Agenda          m_Agenda;
  AgendaItemList          m_ItemList;
public:
};
