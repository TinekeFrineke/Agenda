// EditItemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Agenda.h"
#include "EditItemDialog.h"
#include "afxdialogex.h"

#include "../AgendaModel/Agenda.h"
#include "../AgendaModel/MyTime.h"


// EditItemDialog dialog

IMPLEMENT_DYNAMIC(EditItemDialog, CDialogEx)

EditItemDialog::EditItemDialog(Agenda::Item & item, CWnd* pParent /*=NULL*/)
: CDialogEx(IDD_EDIT_ITEM, pParent),
  m_Item(item)
{

}

EditItemDialog::~EditItemDialog()
{
}

BOOL EditItemDialog::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  m_EditBegin.SetWindowText(m_Item.GetBegin().String().c_str());
  m_EditDescription.SetWindowText(m_Item.GetTaskName().c_str());
  m_EditEnd.SetWindowText(m_Item.GetEnd().String().c_str());
  return 0;
}

void EditItemDialog::OnOK()
{
  Agenda::Time start;
  CString startdate;
  m_EditBegin.GetWindowText(startdate);
  if (!Agenda::ToTime(startdate.GetBuffer(), start)) {
    ::MessageBox(NULL, _T("Invalid start date format (should be \"hh:mm\""), _T("ERROR"), MB_OK);
    return;
  }

  Agenda::Time end;
  CString enddate;
  m_EditEnd.GetWindowText(enddate);
  if (!Agenda::ToTime(enddate.GetBuffer(), end)) {
    ::MessageBox(NULL, _T("Invalid end date format (should be \"hh:mm\""), _T("ERROR"), MB_OK);
    return;
  }

  CString activity;
  m_EditDescription.GetWindowText(activity);
  if (activity.IsEmpty()) {
    ::MessageBox(NULL, _T("Description is empty"), _T("ERROR"), MB_OK);
    return;
  }

  m_Item.Begin(start);
  m_Item.End(end);
  m_Item.SetTaskName(activity.GetBuffer());

  CDialogEx::OnOK();

}

void EditItemDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT_BEGIN, m_EditBegin);
  DDX_Control(pDX, IDC_EDIT_BESCHRIJVING, m_EditDescription);
  DDX_Control(pDX, IDC_EDIT_END, m_EditEnd);
}


BEGIN_MESSAGE_MAP(EditItemDialog, CDialogEx)
END_MESSAGE_MAP()


// EditItemDialog message handlers
