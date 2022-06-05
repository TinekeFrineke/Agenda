#pragma once


#include <windows.h>

#include <iosfwd>
#include <list>
#include <map>
#include <string>

#include <Utilities/strutils.h>

#include "Time.h"

namespace Agenda
{


class Date
{
public:
                          Date();
                          Date(const SYSTEMTIME & aDate);

  Date &                  Year(unsigned int aYear);
  Date &                  Month(unsigned int aMonth);
  Date &                  Day(unsigned int aDay);

  std::tstring            String() const;
  SYSTEMTIME              ToSystemTime() const { return mDate; }

private:
  SYSTEMTIME              mDate;
};

bool ToDate(const std::tstring & token, Date & date);


class Item
{
public:
                          Item();
                          Item(const std::tstring & aTaskName);
                          Item(const SYSTEMTIME &   aBeginTime,
                               const std::tstring & aTaskName);
  //                      Implement copy constructor: must add item to item list
  //                      Don't implement assignment; memberwise assignment is OK.
                          Item(const Item & anItem);

                          ~Item();

  void                    Begin(const Time & aBeginTime);
  void                    End(const Time & anEndTime);
  void                    SetTaskName(const std::tstring & aTaskName);

  Time                    GetBegin() const;
  Time                    GetEnd() const;
  std::tstring            GetTaskName() const;

private:
  std::tstring            mTaskName;
  Time                    mBeginTime;
  Time                    mEndTime;
};

bool              operator==(const Item & anItem1, const Item & anItem2);

class Agenda
{
public:
  typedef std::list<Item> ItemList;
  typedef std::map<std::tstring, Time> ItemTimeMap;

                            Agenda();

  void                      Clear();

  ItemList::iterator        Begin();
  ItemList::iterator        End();

  ItemList::const_iterator  Begin() const;
  ItemList::const_iterator  End() const;

  unsigned int              Size() const;

  Item &                    operator[](unsigned int anIndex);
  const Item &              operator[](unsigned int anIndex) const;

  ItemTimeMap::const_iterator
                            MapBegin() const;
  ItemTimeMap::const_iterator
                            MapEnd() const;

  void                      Add(const Item & anItem, bool adaptendtime);
  void                      Remove(const Item & anItem);

  void                      LoadItems();
  void                      UpdateTotals();
  bool                      Empty() const;
  // Does not clear the map, allowing for a summig of agenda's!
  void                      GetTimeItemMap(ItemTimeMap & map) const;
  
private:
  ItemList                  mItemList;
  ItemTimeMap               mTotals;
};


} // namespace Agenda


std::tistream & operator>>(std::tistream & aStream, Agenda::Date & anItem);
std::tostream & operator<<(std::tostream & aStream, const Agenda::Date & anItem);

std::tistream & operator>>(std::tistream & aStream, Agenda::Item & anItem);
std::tostream & operator<<(std::tostream & aStream, const Agenda::Item & anItem);


std::tistream & operator>>(std::tistream & aStream, Agenda::Agenda & anAgenda);
std::tostream & operator<<(std::tostream & aStream, const Agenda::Agenda & anAgenda);
