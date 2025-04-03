// OldFilesPage.cpp : implementation file
//

#include "stdafx.h"
#include "Agenda.h"
#include "OldFilesPage.h"
#include "afxdialogex.h"

#include "EditAgendaDialog.h"
#include "Settings.h"

// OldFilesPage dialog

IMPLEMENT_DYNAMIC(OldFilesPage, CDialog)

OldFilesPage::OldFilesPage(const Settings & settings, CWnd* pParent /*=NULL*/)
: CDialog     (IDD_OLDFILES_DIALOG, pParent),
  m_Settings  (settings),
  m_FileList  (settings),
  m_OldAgenda (settings)
{
}

OldFilesPage::~OldFilesPage()
{
}

BOOL OldFilesPage::OnInitDialog()
{
  if (CDialog::OnInitDialog() == FALSE)
    return FALSE;

  m_FileList.Fill(m_Settings.GetDataPath());
  m_OldAgenda.Initialize();
  m_OldAgenda.SetSelectionMark(0);
  m_OldAgenda.ShowWindow(SW_SHOW);

  return 0;
}

void OldFilesPage::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_OLDFILES_LIST, m_FileList);
  DDX_Control(pDX, IDC_OLDAGENDA_LIST, m_OldAgenda);
}

void OldFilesPage::UpdateToSelection()
{
  TCHAR buffer[25];
  int sel = m_FileList.GetCurSel();
  if (sel == LB_ERR)
    return;

  m_FileList.GetText(m_FileList.GetCurSel(), buffer);
  m_OldAgenda.FillFrom(buffer);
}


BEGIN_MESSAGE_MAP(OldFilesPage, CDialog)
  ON_LBN_SELCHANGE(IDC_OLDFILES_LIST, &OldFilesPage::OnLbnSelchangeOldfilesList)
  ON_LBN_DBLCLK(IDC_OLDFILES_LIST, &OldFilesPage::OnLbnDblclkOldfilesList)
END_MESSAGE_MAP()


// OldFilesPage message handlers


void OldFilesPage::OnLbnSelchangeOldfilesList()
{
  UpdateToSelection();
}


void OldFilesPage::OnLbnDblclkOldfilesList()
{
  char buffer[25];
  int sel = m_FileList.GetCurSel();
  if (sel == LB_ERR)
    return;

  m_FileList.GetText(m_FileList.GetCurSel(), buffer);

  Agenda::Agenda agenda;
  m_Settings.LoadAgenda(agenda, Path(buffer));

  EditAgendaDialog dialog(agenda, this);
  INT_PTR nResponse = dialog.DoModal();
  switch (nResponse) {
    case IDOK:
      m_Settings.SaveAgenda(dialog.GetAgenda(), Path(buffer));
      UpdateToSelection();
    break;
    case IDCANCEL:
    break;
    default:
    break;
  }
}
