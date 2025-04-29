
#include "agenda.h"

#include <assert.h>  
#include <numeric>
#include <optional>

#include <utilities/Inifile.h>
#include <utilities/strutils.h>


namespace Agenda
{

//******************************************************************************
// class Agenda::Agenda
//******************************************************************************

//------------------------------------------------------------------------------
Agenda::Agenda()
{
}


void Agenda::Clear()
{
    mItemList.clear();
}


//------------------------------------------------------------------------------
Agenda::ItemList::iterator Agenda::Begin()
{
    return mItemList.begin();
}


//------------------------------------------------------------------------------
Agenda::ItemList::iterator Agenda::End()
{
    return mItemList.end();
}


//------------------------------------------------------------------------------
Agenda::ItemList::const_iterator Agenda::Begin() const
{
    return mItemList.begin();
}


//------------------------------------------------------------------------------
Agenda::ItemList::const_iterator Agenda::End() const
{
    return mItemList.end();
}


//---------------------------------------------------------------------------
Item& Agenda::operator[](unsigned int anIndex)
{
    Agenda::ItemList::iterator iter = Begin();
    unsigned int counter = 0;
    while (counter < anIndex && iter != End())
    {
        iter++;
        counter++;
    }

    if (iter == End())
        throw std::runtime_error("Index out of range!");

    return *iter;
}


//---------------------------------------------------------------------------
const Item& Agenda::operator[](unsigned int anIndex) const
{
    Agenda::ItemList::const_iterator citer = Begin();
    unsigned int counter = 0;
    while (counter < anIndex && citer != End())
    {
        citer++;
        counter++;
    }

    if (citer == End())
        throw std::runtime_error("Index out of range!");

    return *citer;
}

//------------------------------------------------------------------------------
void Agenda::Add(const Item& anItem, bool adaptendtime)
{
    if (adaptendtime) {
        ItemList::reverse_iterator riter = mItemList.rbegin();
        if (riter != mItemList.rend())
            riter->End(anItem.GetBegin());
    }

    mItemList.push_back(anItem);

    UpdateTotals();
}


//--------------------------------------------------------------------------
void Agenda::UpdateTotals()
{
    mTotals.clear();
    for (ItemList::const_iterator citer = mItemList.begin();
         citer != mItemList.end();
         citer++)
    {
        mTotals[citer->GetTaskName()] += (citer->GetEnd() - citer->GetBegin());
    }
}

bool Agenda::Empty() const
{
    return mItemList.empty();
}

void Agenda::GetTimeItemMap(ItemTimeMap& map) const
{
    for (ItemList::const_iterator citer = mItemList.begin();
         citer != mItemList.end();
         citer++)
    {
        map[citer->GetTaskName()] += (citer->GetEnd() - citer->GetBegin());
    }
}

size_t Agenda::Size() const
{
    return mItemList.size();
}

//------------------------------------------------------------------------------
void Agenda::Remove(const Item& anItem)
{
    for (ItemList::iterator iter = mItemList.begin();
         iter != mItemList.end();
         iter++)
    {
        if (*iter == anItem)
        {
            mItemList.erase(iter);
            iter = mItemList.end();
        }
    }

    UpdateTotals();
}


//------------------------------------------------------------------------------
void Agenda::LoadItems()
{
    std::string filename = Date().String();
    std::ifstream instream(filename, std::ios::in);
    if (instream.bad())
        return;

    Item item;

    instream >> item;
    while (!instream.fail())
    {
        Add(item, false);
        instream >> item;
    }
}


//---------------------------------------------------------------------------
Agenda::ItemTimeMap::const_iterator Agenda::MapBegin() const
{
    return mTotals.begin();
}


//---------------------------------------------------------------------------
Agenda::ItemTimeMap::const_iterator Agenda::MapEnd() const
{
    return mTotals.end();
}


//******************************************************************************
// class Agenda::Date
//******************************************************************************

//------------------------------------------------------------------------------
Date::Date()
{
    GetLocalTime(&mDate);
}


//------------------------------------------------------------------------------
Date::Date(const SYSTEMTIME& aDate)
{
    mDate = aDate;
}


//------------------------------------------------------------------------------
std::string Date::String() const
{
    char date[9];
    sprintf_s(date, "%04d%02d%02d", mDate.wYear, mDate.wMonth, mDate.wDay);
    date[8] = '\0';
    return date;
}


//------------------------------------------------------------------------------
Date& Date::Year(unsigned int aYear)
{
    mDate.wYear = static_cast<WORD>(aYear);
    return *this;
}


//------------------------------------------------------------------------------
Date& Date::Month(unsigned int aMonth)
{
    assert(aMonth < 13);
    mDate.wMonth = static_cast<WORD>(aMonth);
    return *this;
}


//------------------------------------------------------------------------------
Date& Date::Day(unsigned int aDay)
{
    assert(aDay < 32);
    mDate.wDay = static_cast<WORD>(aDay);

    return *this;
}


//******************************************************************************
// class Agenda::Item
//******************************************************************************

//------------------------------------------------------------------------------
Item::Item()
{
}


//------------------------------------------------------------------------------
Item::Item(const std::string& aTaskName)
    : mTaskName(aTaskName)
{
}


//------------------------------------------------------------------------------
Item::Item(const SYSTEMTIME& aBeginTime, const std::string& aTaskName)
    : mBeginTime(aBeginTime),
    mEndTime(aBeginTime),
    mTaskName(aTaskName)
{
}


//------------------------------------------------------------------------------
Item::Item(const Item& anItem)
    : mBeginTime(anItem.mBeginTime),
    mEndTime(anItem.mEndTime),
    mTaskName(anItem.mTaskName)
{
}


//------------------------------------------------------------------------------
Item::~Item()
{
}


//------------------------------------------------------------------------------
void Item::Begin(const Time& aBeginTime)
{
    mBeginTime = aBeginTime;
}


//------------------------------------------------------------------------------
void Item::End(const Time& anEndTime)
{
    mEndTime = anEndTime;
}


//------------------------------------------------------------------------------
Time Item::GetBegin() const
{
    return mBeginTime;
}


//------------------------------------------------------------------------------
Time Item::GetEnd() const
{
    return mEndTime;
}


//------------------------------------------------------------------------------
std::string Item::GetTaskName() const
{
    return mTaskName;
}


//------------------------------------------------------------------------------
void Item::SetTaskName(const std::string& aTaskName)
{
    mTaskName = aTaskName;
}


//------------------------------------------------------------------------------
bool operator==(const Item& anItem1, const Item& anItem2)
{
    return (anItem1.GetBegin() == anItem2.GetBegin() &&
            anItem1.GetTaskName() == anItem2.GetTaskName() &&
            anItem1.GetEnd() == anItem2.GetEnd());
}


//------------------------------------------------------------------------------
bool ToDate(const std::string& token, Date& date)
{
    std::vector<std::string> tokens;
    if (token.length() < 8)
        return false;

    try {
        date.Year(Str::ToInt(token.substr(0, 4).c_str()))
            .Month(Str::ToInt(token.substr(4, 2).c_str()))
            .Day(Str::ToInt(token.substr(6, 2).c_str()));
    }
    catch (...) {
        return false;
    }

    return true;
}


} // namespace Agenda


std::istream& operator>>(std::istream& aStream, Agenda::Date& aDate)
{
    std::string token;
    aStream >> token;
    if (!Agenda::ToDate(token, aDate))
        aDate = Agenda::Date();

    return aStream;
}

std::ostream& operator<<(std::ostream& aStream, const Agenda::Date& aDate)
{
    aStream << aDate.String();
    return aStream;
}

namespace {
std::optional<Agenda::Item> ToItem(const std::string& line) {
    std::vector<std::string> tokens;
    if (Str::StrTok(line, " ", tokens) < 3)
        return {};

    Agenda::Item item;
    item.Begin(Agenda::ToTime(tokens.front()));
    item.End(Agenda::ToTime(tokens.back()));
    tokens.erase(tokens.begin());
    tokens.erase(tokens.end() - 1);

    auto task = std::accumulate(tokens.begin(), tokens.end(), std::string(" "));
    Str::Trim(task);
    item.SetTaskName(task);
    return item;
}
}

//------------------------------------------------------------------------------
std::istream& operator>>(std::istream& aStream, Agenda::Item& anItem)
{
    std::string line;
    std::getline(aStream, line);

    auto item = ToItem(line);
    if (item)
        anItem = *item;

    return aStream;
}


//---------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& aStream, const Agenda::Item& anItem)
{
    aStream << anItem.GetBegin()
        << " "
        << anItem.GetTaskName()
        << " "
        << anItem.GetEnd()
        << std::endl;

    return aStream;
}


//------------------------------------------------------------------------------
std::istream& operator>>(std::istream& aStream, Agenda::Agenda& anAgenda)
{
    // Read items
    Agenda::Item item;
    aStream >> item;
    while (!aStream.fail())
    {
        anAgenda.Add(item, false);
        aStream >> item;
    }

    return aStream;
}


//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& aStream,
                         const Agenda::Agenda& anAgenda)
{
    for (Agenda::Agenda::ItemList::const_iterator citer = anAgenda.Begin();
         citer != anAgenda.End();
         citer++)
    {
        aStream << *citer;
    }

    return aStream;
}

