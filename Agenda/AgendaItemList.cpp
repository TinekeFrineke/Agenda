
#include "stdafx.h"

#include <algorithm>

#include "AgendaItemList.h"


void AgendaItemListItem::Write(CListCtrl & aControl, int iItemIndex)
{
    LV_ITEM lvi;
    memset(&lvi, 0, sizeof(LV_ITEM));

    lvi.mask = LVIF_TEXT | LVIF_PARAM;
    lvi.iItem = iItemIndex;
    lvi.iSubItem = 0;
    lvi.lParam = (LPARAM)m_Item;

    int index = aControl.InsertItem(&lvi);

    aControl.SetItemText(index, 0, m_Agenda[m_Item].GetBegin().String().c_str());
    aControl.SetItemText(index, 1, m_Agenda[m_Item].GetTaskName().c_str());
    aControl.SetItemText(index, 2, m_Agenda[m_Item].GetEnd().String().c_str());
    aControl.SetItemData(index, (DWORD_PTR)this);
}

Agenda::Item & AgendaItemListItem::GetItem()
{
  return m_Agenda[m_Item];
}


AgendaItemList::AgendaItemList(Agenda::Agenda & agenda)
    : m_Agenda(agenda)
{
}

AgendaItemList::~AgendaItemList()
{
  ClearItems();
}

void AgendaItemList::Initialize()
{
    SetSize();
    ListControl::Initialize();
}

void AgendaItemList::View(Agenda::Agenda & agenda)
{
  DeleteAllItems();
  ClearItems();

  for (unsigned int i = 0; i < agenda.Size(); ++i)
    m_Items.push_back(new AgendaItemListItem(agenda, i));

  for (size_t i = 0; i < m_Items.size(); ++i)
    m_Items[i]->Write(*this, (int)i);

  if (!m_Items.empty())
  {
    LVITEM item;
    item.iItem = 0;
    item.mask = LVIF_STATE;
    SelectItem(0, true);
  }

  SetSize();
}

void AgendaItemList::SetSize()
{
    CFont* font = GetFont();
    CClientDC deviceContext(this);
    deviceContext.SelectObject(HFONT(*font));

    RECT rectangle({ 0, 0, 0, 0 });
    GetClientRect(&rectangle);
    deviceContext.Rectangle(&rectangle);

    CSize timeSize = deviceContext.GetTextExtent("99:99");
    CSize startSize = deviceContext.GetTextExtent("Start");
    const auto maxTimeWidth(std::max(timeSize.cx, startSize.cx));
    const auto descriptionSize = rectangle.right - rectangle.left - 20 - maxTimeWidth * 2;

    SetNumberOfColumns(3);
    SetColumnInfo(0, ColInfo(maxTimeWidth, LVCFMT_LEFT, "Start"));
    SetColumnInfo(1, ColInfo(descriptionSize, LVCFMT_LEFT, "Description"));
    SetColumnInfo(2, ColInfo(maxTimeWidth, LVCFMT_LEFT, "End"));
    SetColumnWidth(0, LVSCW_AUTOSIZE);
    //SetColumnWidth(1, LVSCW_AUTOSIZE);
    SetColumnWidth(2, LVSCW_AUTOSIZE);
}

AgendaItemListItem * AgendaItemList::GetItemAt(int iIndex)
{
  if (iIndex < 0 || iIndex >= static_cast<int>(m_Items.size()))
    return NULL;

  return m_Items[iIndex];
}

AgendaItemListItem * AgendaItemList::GetSelectedItem()
{
  POSITION pos = GetFirstSelectedItemPosition();
  if (pos == NULL)
    return NULL;

  int nItem = GetNextSelectedItem(pos);

  if (nItem >= 0 && nItem < int(m_Items.size()))
    return (AgendaItemListItem *)GetItemData(nItem);
  
  return NULL;
}

void AgendaItemList::SelectItem(const Agenda::Item & anItem)
{
  for (size_t i = 0; i < m_Items.size(); ++i)
    SelectItem(static_cast<int>(i), m_Items[i]->GetItem() == anItem);
}

void AgendaItemList::SelectItem(int iIndex, bool bSelect)
{
  if (iIndex >= static_cast<int>(m_Items.size()))
    return;

  LVITEM item;
  item.iItem = iIndex;
  item.mask = LVIF_STATE;
  item.state = bSelect ? 1/*LIS_FOCUSED*/ : 0;
  SetItem(&item);
}

void AgendaItemList::ClearItems()
{
  for (size_t i = 0; i < m_Items.size(); ++i)
    delete m_Items[i];

  m_Items.clear();
}

BEGIN_MESSAGE_MAP(AgendaItemList, CListCtrl)
	//{{AFX_MSG_MAP(AgendaItemList)
	//ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

