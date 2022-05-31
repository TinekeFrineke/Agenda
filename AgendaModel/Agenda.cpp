
#include "agenda.h"

#include <assert.h>  

#include <Utilities/Inifile.h>
#include <Utilities/strutils.h>


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
Item & Agenda::operator[](unsigned int anIndex)
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
const Item & Agenda::operator[](unsigned int anIndex) const
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
void Agenda::Add(const Item & anItem, bool adaptendtime)
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

void Agenda::GetTimeItemMap(ItemTimeMap & map) const
{
  for (ItemList::const_iterator citer = mItemList.begin();
       citer != mItemList.end();
       citer++)
  {
    map[citer->GetTaskName()] += (citer->GetEnd() - citer->GetBegin());
  }
}

unsigned int Agenda::Size() const
{
  return mItemList.size();
}

//------------------------------------------------------------------------------
void Agenda::Remove(const Item & anItem)
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
  std::tstring filename = Date().String();
  std::tifstream instream(Str::ToString(filename).c_str(), std::ios::in);
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
Date::Date(const SYSTEMTIME & aDate)
{
  mDate = aDate;
}


//------------------------------------------------------------------------------
std::tstring Date::String() const
{
  TCHAR date[9];
  _stprintf_s(date, _T("%04d%02d%02d"), mDate.wYear, mDate.wMonth, mDate.wDay);
  date[8] = _T('\0');
  return std::tstring(date);
}


//------------------------------------------------------------------------------
Date & Date::Year(unsigned int aYear)
{
  mDate.wYear = static_cast<WORD>(aYear);
  return *this;
}


//------------------------------------------------------------------------------
Date & Date::Month(unsigned int aMonth)
{
  assert(aMonth < 13);
  mDate.wMonth = static_cast<WORD>(aMonth);
  return *this;
}


//------------------------------------------------------------------------------
Date & Date::Day(unsigned int aDay)
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
Item::Item(const std::tstring & aTaskName)
: mTaskName   (aTaskName)
{
}


//------------------------------------------------------------------------------
Item::Item(const SYSTEMTIME & aBeginTime, const std::tstring & aTaskName)
: mBeginTime  (aBeginTime),
  mEndTime    (aBeginTime),
  mTaskName   (aTaskName)
{
}


//------------------------------------------------------------------------------
Item::Item(const Item & anItem)
: mBeginTime(anItem.mBeginTime),
  mEndTime  (anItem.mEndTime),
  mTaskName (anItem.mTaskName)
{
}


//------------------------------------------------------------------------------
Item::~Item()
{
}


//------------------------------------------------------------------------------
void Item::Begin(const Time & aBeginTime)
{
  mBeginTime = aBeginTime;
}


//------------------------------------------------------------------------------
void Item::End(const Time & anEndTime)
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
std::tstring Item::GetTaskName() const
{
  return mTaskName;
}


//------------------------------------------------------------------------------
void Item::SetTaskName(const std::tstring &  aTaskName)
{
  mTaskName = aTaskName;
}


//------------------------------------------------------------------------------
bool operator==(const Item & anItem1, const Item & anItem2)
{
  return (anItem1.GetBegin()    == anItem2.GetBegin()     &&
          anItem1.GetTaskName() == anItem2.GetTaskName()  &&
          anItem1.GetEnd()      == anItem2.GetEnd());
}


//------------------------------------------------------------------------------
bool ToDate(const std::tstring & token, Date & date)
{
  std::vector<std::string> tokens;
  if (token.length() < 8)
    return false;

  try {
    date.Year (Str::ToInt(token.substr(0, 4).c_str()))
        .Month(Str::ToInt(token.substr(4, 2).c_str()))
        .Day  (Str::ToInt(token.substr(6, 2).c_str()));
  }
  catch(...) {
    return false;
  }

  return true;
}


} // namespace Agenda


std::tistream & operator >> (std::tistream & aStream, Agenda::Date & aDate)
{
  std::tstring token;
  aStream >> token;
  if (!Agenda::ToDate(token, aDate))
    aDate = Agenda::Date();

  return aStream;
}

std::tostream & operator<<(std::tostream & aStream, const Agenda::Date & aDate)
{
  aStream << aDate.String();
  return aStream;
}

//------------------------------------------------------------------------------
std::tistream & operator>>(std::tistream & aStream, Agenda::Item & anItem)
{
  Agenda::Item item;

  std::tstring line;

  aStream >> line;
  if (aStream.eof())
    return aStream;

  item.Begin(Agenda::ToTime(line));
  aStream >> line;
  item.SetTaskName(Str::HttpDecode(line.c_str()));
  aStream >> line;
  item.End(Agenda::ToTime(line));

  if (!aStream.fail())
    anItem = item;

  return aStream;
}


//---------------------------------------------------------------------------
std::tostream & operator<<(std::tostream & aStream, const Agenda::Item & anItem)
{
  aStream << anItem.GetBegin()
          << _T(" ")
          << Str::HttpEncode(anItem.GetTaskName().c_str())
          << _T(" ")
          << anItem.GetEnd()
          << std::endl;

  return aStream;
}


//------------------------------------------------------------------------------
std::tistream & operator>>(std::tistream &   aStream,
                           Agenda::Agenda & anAgenda)
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
std::tostream & operator<<(std::tostream &         aStream,
                           const Agenda::Agenda & anAgenda)
{
  for (Agenda::Agenda::ItemList::const_iterator citer = anAgenda.Begin();
       citer != anAgenda.End();
       citer++)
  {
    aStream << *citer;
  }

  return aStream;
}

