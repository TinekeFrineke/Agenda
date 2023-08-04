#pragma once

#include <CustomControls/ItemListControl.h>

#include "../AgendaModel/Agenda.h"

class AgendaItemListItem
{
public:
    AgendaItemListItem(Agenda::Agenda & agenda, unsigned int anItem)
    : m_Agenda(agenda), m_Item(anItem) {}

	void                            Write(CListCtrl & aControl, int iItemIndex);

	Agenda::Item &                  GetItem();

private:
	Agenda::Agenda &                m_Agenda;
	const unsigned int              m_Item;
};


class AgendaItemList : public ListControl/* , public Observer*/
{
// Construction
public:
    AgendaItemList(Agenda::Agenda & agenda);
	virtual ~AgendaItemList();

	void                  Initialize() override;
	void                          View(Agenda::Agenda & agenda);

  void SetSize();
  AgendaItemListItem *              GetItemAt(int iIndex);
  AgendaItemListItem *              GetSelectedItem();
  void                              SelectItem(const Agenda::Item & anItem);
  void                              SelectItem(int iIndex, bool bSelect);

protected:
	//{{AFX_MSG(ItemList)
	//afx_msg void                      OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

  Agenda::Agenda &                  GetAgenda() { return m_Agenda; }
  void                              ClearItems();

private:
  std::vector<AgendaItemListItem *> m_Items;
  Agenda::Agenda &                  m_Agenda;
};


//class AgendaItemList : public ItemList
//{
//};