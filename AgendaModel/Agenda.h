#pragma once


#include <windows.h>

#include <iosfwd>
#include <list>
#include <map>
#include <string>

#include "MyTime.h"

namespace Agenda
{


class Date
{
public:
    Date();
    Date(const SYSTEMTIME& aDate);

    Date& Year(unsigned int aYear);
    Date& Month(unsigned int aMonth);
    Date& Day(unsigned int aDay);

    std::string String() const;
    SYSTEMTIME ToSystemTime() const { return mDate; }

private:
    SYSTEMTIME mDate;
};

bool ToDate(const std::string& token, Date& date);


class Item
{
public:
    Item();
    explicit Item(const std::string& aTaskName);
    Item(const SYSTEMTIME& aBeginTime, const std::string& aTaskName);
    //                      Implement copy constructor: must add item to item list
    //                      Don't implement assignment; memberwise assignment is OK.
    Item(const Item& anItem);

    ~Item();

    void Begin(const Time& aBeginTime);
    void End(const Time& anEndTime);
    void SetTaskName(const std::string& aTaskName);

    Time GetBegin() const;
    Time GetEnd() const;
    std::string GetTaskName() const;

private:
    std::string mTaskName;
    Time mBeginTime;
    Time mEndTime;
};

bool operator==(const Item& anItem1, const Item& anItem2);

class Agenda
{
public:
    using ItemList = std::list<Item>;
    using ItemTimeMap = std::map<std::string, Time>;

    Agenda();

    void Clear();

    ItemList::iterator Begin();
    ItemList::iterator End();

    ItemList::const_iterator  Begin() const;
    ItemList::const_iterator  End() const;

    size_t Size() const;

    Item& operator[](unsigned int anIndex);
    const Item& operator[](unsigned int anIndex) const;

    ItemTimeMap::const_iterator MapBegin() const;
    ItemTimeMap::const_iterator MapEnd() const;

    void Add(const Item& anItem, bool adaptendtime);
    void Remove(const Item& anItem);

    void LoadItems();
    void UpdateTotals();
    bool Empty() const;
    // Does not clear the map, allowing for a summig of agenda's!
    void GetTimeItemMap(ItemTimeMap& map) const;

private:
    ItemList     mItemList;
    ItemTimeMap  mTotals;
};


} // namespace Agenda

inline bool operator==(const Agenda::Date& lhs, const Agenda::Date& rhs)
{
    return lhs.String() == rhs.String();
}

std::istream& operator>>(std::istream& aStream, Agenda::Date& anItem);
std::ostream& operator<<(std::ostream& aStream, const Agenda::Date& anItem);

std::istream& operator>>(std::istream& aStream, Agenda::Item& anItem);
std::ostream& operator<<(std::ostream& aStream, const Agenda::Item& anItem);


std::istream& operator>>(std::istream& aStream, Agenda::Agenda& anAgenda);
std::ostream& operator<<(std::ostream& aStream, const Agenda::Agenda& anAgenda);
