#pragma once

#include <string>

#include <CustomControls/ItemListControl.h>
#include <Utilities/Path.h>

#include "AgendaModel/Agenda.h"

class Settings;

typedef std::map<std::string, Agenda::Time> SumList;

class AgendaTotalsItem
{
public:
                                  AgendaTotalsItem(const std::string & task, const Agenda::Time & time)
                                    : m_Task(task), m_Time(time) {}

  void                            Write(CListCtrl & aControl, int iItemIndex);

private:
  std::string m_Task;
  Agenda::Time m_Time;
};


class AgendaTotalsList:
  public ListControl
{
public:
  AgendaTotalsList();
  ~AgendaTotalsList() override;

  void FillFrom(const Agenda::Agenda::ItemTimeMap & map);
  void SelectItem(int iIndex, bool bSelect);

protected:
  void ClearItems();

private:
  SumList m_SumList;
  std::vector<AgendaTotalsItem *>  m_Items;
};

