#pragma once

#include <CustomControls/ItemListControl.h>
#include <Utilities/PathUtils.h>

#include "../AgendaModel/Agenda.h"

class Settings;

typedef std::map<std::tstring, Agenda::Time> SumList;

class OldAgendaItem
{
public:
                                  OldAgendaItem(const std::tstring & task, const Agenda::Time & time)
                                    : m_Task(task), m_Time(time) {}

  void                            Write(CListCtrl & aControl, int iItemIndex);

  //Agenda::Item &                  GetItem();

private:
  std::tstring                    m_Task;
  Agenda::Time                    m_Time;
};


class OldAgendaList:
  public ListControl
{
public:
  OldAgendaList(const Settings & settings);
  virtual                       ~OldAgendaList();

  void                          FillFrom(const std::tstring & filename);
  void                          SelectItem(int iIndex, bool bSelect);

protected:
  void                          ClearItems();

private:
  Path                          m_Path;
  Agenda::Agenda                m_Agenda;
  SumList                       m_SumList;
  std::vector<OldAgendaItem *>  m_Items;
};

