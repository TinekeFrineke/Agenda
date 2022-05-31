// EditAgendaDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Agenda.h"
#include "EditAgendaDialog.h"
#include "afxdialogex.h"

#include "EditItemDialog.h"
#include "Settings.h"


// EditAgendaDialog dialog

IMPLEMENT_DYNAMIC(EditAgendaDialog, CDialog)

EditAgendaDialog::EditAgendaDialog(const Agenda::Agenda & agenda, CWnd* pParent /*=NULL*/)
: CDialog   (IDD_EDIT_AGENDA_DIALOG, pParent),
  m_Agenda(agenda),
  m_ItemList(m_Agenda)
{
}

EditAgendaDialog::~EditAgendaDialog()
{
}

void EditAgendaDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_ITEM_LIST, m_ItemList);
}


BEGIN_MESSAGE_MAP(EditAgendaDialog, CDialog)
  ON_BN_CLICKED(IDOK, &EditAgendaDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &EditAgendaDialog::OnBnClickedCancel)
  ON_NOTIFY(NM_DBLCLK, IDC_ITEM_LIST, &EditAgendaDialog::OnNMDblclkItemList)
END_MESSAGE_MAP()


// EditAgendaDialog message handlers


BOOL EditAgendaDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_ItemList.Initialize();
  m_ItemList.SetSelectionMark(0);
  m_ItemList.ShowWindow(SW_SHOW);
  m_ItemList.View(m_Agenda);
  return 0;
}

void EditAgendaDialog::OnBnClickedOk()
{
  CDialog::OnOK();
}


void EditAgendaDialog::OnBnClickedCancel()
{
  CDialog::OnCancel();
}


void EditAgendaDialog::OnNMDblclkItemList(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
  if (pNMItemActivate->iItem == -1) {
    *pResult = 0;
    return;
  }

  AgendaItemListItem * pItem(reinterpret_cast<AgendaItemListItem *>(m_ItemList.GetItemData(pNMItemActivate->iItem)));
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

  m_ItemList.View(m_Agenda);

  // TODO: Add your control notification handler code here
  *pResult = 0;
}
