
#include "stdafx.h"

#include "OldAgendaList.h"
#include "Settings.h"


void OldAgendaItem::Write(CListCtrl & aControl, int iItemIndex)
{
  LV_ITEM lvi;
  memset(&lvi, 0, sizeof(LV_ITEM));

	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = iItemIndex;
	lvi.iSubItem = 0;
  //lvi.lParam = (LPARAM)m_Item;

	int index = aControl.InsertItem(&lvi);

  aControl.SetItemText(index, 0, m_Task.c_str());
  aControl.SetItemText(index, 1, m_Time.String().c_str());
  aControl.SetItemData(index, (DWORD_PTR)this);
  //delete name;
}

//Agenda::Item & OldAgendaItem::GetItem()
//{
//}


OldAgendaList::OldAgendaList(const Settings & settings)
: m_Path(settings.GetDataPath())
{
  SetNumberOfColumns(2);
  SetColumnInfo     (0, ColInfo(250,  LVCFMT_LEFT,  _T("Task name")));
  SetColumnInfo     (1, ColInfo(80,   LVCFMT_LEFT,  _T("Total time")));
}


OldAgendaList::~OldAgendaList()
{
  ClearItems();
}

void OldAgendaList::FillFrom(const std::tstring & filename)
{
  DeleteAllItems();
  ClearItems();

  Path path(m_Path);
  path += filename;
  m_Agenda.Clear();
  std::tifstream stream(path.AsString().c_str());
  stream >> m_Agenda;

  m_SumList.clear();
  for (Agenda::Agenda::ItemList::iterator iter = m_Agenda.Begin(); iter != m_Agenda.End(); ++iter)
    m_SumList[iter->GetTaskName()] += Agenda::Time(Agenda::Minutes(iter->GetEnd() - iter->GetBegin()));

  for (SumList::iterator iter = m_SumList.begin(); iter != m_SumList.end(); ++iter)
    m_Items.push_back(new OldAgendaItem(iter->first, iter->second));

  for (size_t i = 0; i < m_Items.size(); ++i)
    m_Items[i]->Write(*this, (int)i);

  if (!m_Items.empty())
  {
    LVITEM item;
    item.iItem = 0;
    item.mask = LVIF_STATE;
    SelectItem(0, true);
  }

  for (int i = 0; i < 3; ++i)
    SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
}

void OldAgendaList::SelectItem(int iIndex, bool bSelect)
{
  if (iIndex >= static_cast<int>(m_Items.size()))
    return;

  LVITEM item;
  item.iItem = iIndex;
  item.mask = LVIF_STATE;
  item.state = bSelect ? 1/*LIS_FOCUSED*/ : 0;
  SetItem(&item);
}

void OldAgendaList::ClearItems()
{
  for (size_t i = 0; i < m_Items.size(); ++i)
    delete m_Items[i];

  m_Items.clear();
}
