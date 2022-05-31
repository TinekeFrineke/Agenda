#pragma once

#include <CustomControls/ItemListControl.h>
#include <Utilities/PathUtils.h>

#include "../AgendaModel/Agenda.h"

class Settings;

typedef std::map<std::tstring, Agenda::Time> SumList;

class AgendaTotalsItem
{
public:
                                  AgendaTotalsItem(const std::tstring & task, const Agenda::Time & time)
                                    : m_Task(task), m_Time(time) {}

  void                            Write(CListCtrl & aControl, int iItemIndex);

private:
  std::tstring                    m_Task;
  Agenda::Time                    m_Time;
};


class AgendaTotalsList:
  public ListControl
{
public:
  AgendaTotalsList();
  virtual                       ~AgendaTotalsList();

  void                          FillFrom(const Agenda::Agenda::ItemTimeMap & map);
  void                          SelectItem(int iIndex, bool bSelect);

protected:
  void                          ClearItems();

private:
  SumList                       m_SumList;
  std::vector<AgendaTotalsItem *>  m_Items;
};

