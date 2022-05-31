#pragma once

#include <windows.h>

#include <iosfwd>
#include <list>
#include <map>
#include <string>

#include <Utilities/strutils.h>

namespace Agenda
{

class Minutes
{
public:
  Minutes(int number) : m_Number(number) {}

  operator int() const { return m_Number; }

private:
  int                     m_Number;
};

// Time is streamed in / out with hh:mm
class Time
{
public:
  explicit                Time(const Minutes & minutes);
                          Time(const SYSTEMTIME & aTime);
                          Time();

  Time &                  Hour(unsigned int anHour);
  Time &                  Minute(unsigned int aMinute);

  unsigned int            GetHour() const;
  unsigned int            GetMinute() const;

  unsigned int            InMinutes() const;

  std::tstring            String() const;

  Time &                  operator+=(const Time & aTime);
  Time &                  operator+=(int aMinutes);
  Time &                  operator-=(const Time & aTime);
  Time &                  operator-=(int aMinutes);

  static Time             Now();

private:
  unsigned int            mHour;
  unsigned int            mMinutes;
};



bool              ToTime(const std::tstring &  aTime,
                         Time & time,
                         const std::tstring &  aSeparator = _T(":"));
Time              ToTime(const std::tstring &  aTime,
                         const std::tstring &  aSeparator = _T(":"));

bool          operator==(const Time & aTime1, const Time & aTime2);
bool          operator< (const Time & aTime1, const Time & aTime2);
int           operator -(const Time & aTime1, const Time & aTime2);


} // namespace Agenda


std::tistream & operator>>(std::tistream & aStream, Agenda::Time & anItem);
std::tostream & operator<<(std::tostream & aStream, const Agenda::Time & anItem);
