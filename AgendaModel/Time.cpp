
#include "Time.h"

#include <assert.h>  

#include <Utilities/Inifile.h>
#include <Utilities/strutils.h>


namespace Agenda
{

//***************************************************************************
// class Agenda::Time
//***************************************************************************

//---------------------------------------------------------------------------
Time::Time()
: mHour   (0),
  mMinutes(0)
{
}


Time::Time(const Minutes & minutes)
: mMinutes(minutes % 60),
  mHour(minutes / 60)
{
}

Time::Time(const SYSTEMTIME & aTime)
: mHour   (aTime.wHour),
  mMinutes(aTime.wMinute)
{
}

Time Time::Now()
{
  SYSTEMTIME    date;
  GetLocalTime(&date);
  return Time(date);
}


std::tstring Time::String() const
{
  TCHAR time[6];
  _stprintf_s(time, _T("%02d:%02d"), mHour, mMinutes);
  time[5] = _T('\0');
  return std::tstring(time);
}

Time & Time::Hour(unsigned int anHour)
{
  if (anHour >= 24)
    throw std::runtime_error("Hour >= 24");

  mHour = anHour;

  return *this;
}


Time & Time::Minute(unsigned int aMinutes)
{
  if (aMinutes >= 60)
    throw std::runtime_error("Minutes >= 60");

  mMinutes = aMinutes;
  return *this;
}


unsigned int Time::GetHour() const
{
  return mHour;
}


unsigned int Time::GetMinute() const
{
  return mMinutes;
}

unsigned int Time::InMinutes() const
{
  return mMinutes + mHour * 60;
}


Time & Time::operator+=(const Time & aTime)
{
  return operator+=(aTime.InMinutes());
}


Time & Time:: operator+=(int aMinutes)
{
  unsigned int minutes(InMinutes() + aMinutes);
  mMinutes = minutes % 60;
  mHour = (minutes / 60) % 24;
  return *this;
}

Time & Time::operator-=(const Time & aTime)
{
  return operator-=(aTime.InMinutes());
}

Time & Time::operator-=(int aMinutes)
{
  int minutes(static_cast<int>(InMinutes()) - aMinutes);
  mMinutes = minutes % 60;
  mHour = (minutes / 60) % 24;
  return *this;
}


bool operator==(const Time & aTime1, const Time & aTime2)
{
  return (aTime1.GetHour()    == aTime2.GetHour() &&
          aTime1.GetMinute()  == aTime2.GetMinute());
}


//--------------------------------------------------------------------------
bool operator< (const Time & aTime1, const Time & aTime2)
{
  if (aTime1.GetHour() < aTime2.GetHour())
    return true;
  else if (aTime1.GetHour() == aTime2.GetHour())
    if (aTime1.GetMinute() < aTime2.GetMinute())
      return true;

  return false;
}


//--------------------------------------------------------------------------
int operator -(const Time & aTime1, const Time & aTime2)
{
  int t1 = aTime1.GetHour() * 60 + aTime1.GetMinute();
  int t2 = aTime2.GetHour() * 60 + aTime2.GetMinute();

  return t1 - t2;
}


bool ToTime(const std::tstring &  aTime, Time & time,
            const std::tstring &  aSeparator)
{
  std::vector<std::tstring> tokens;
  Str::StrTok(aTime, aSeparator, tokens);

  if (tokens.size() != 2)
    return false;

  time.Hour(Str::ToInt(tokens[0].c_str()));
  time.Minute(Str::ToInt(tokens[1].c_str()));
  return true;
}

Time ToTime(const std::tstring & aTime, const std::tstring & aSeparator)
{
  std::vector<std::tstring> tokens;
  Str::StrTok(aTime, aSeparator, tokens);

  Time time;
  if (tokens.size() > 0)
  {
    time.Hour(Str::ToInt(tokens[0].c_str()));
  }

  if (tokens.size() > 1)
  {
    time.Minute(Str::ToInt(tokens[1].c_str()));
  }

  return time;
}


} // namespace Agenda


//------------------------------------------------------------------------------
std::tistream & operator>>(std::tistream & aStream, Agenda::Time & aTime)
{
  std::tstring token;
  aStream >> token;
  aTime = Agenda::ToTime(token);

  return aStream;
}


//------------------------------------------------------------------------------
std::tostream & operator<<(std::tostream & aStream, const Agenda::Time & aTime)
{
  aStream << aTime.String();
  return aStream;
}
