#pragma once
#include "afxwin.h"

namespace Agenda { class Item; }

// EditItemDialog dialog

class EditItemDialog : public CDialogEx
{
	DECLARE_DYNAMIC(EditItemDialog)

public:
	EditItemDialog(Agenda::Item & item, CWnd* pParent = NULL);   // standard constructor
	virtual ~EditItemDialog();

	virtual BOOL            OnInitDialog() override;
  virtual void            OnOK() override;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_ITEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  Agenda::Item &  m_Item;
  CEdit           m_EditBegin;
  CEdit           m_EditDescription;
  CEdit           m_EditEnd;
};
