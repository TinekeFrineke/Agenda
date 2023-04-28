#pragma once

#include <windows.h>

#include <iosfwd>
#include <list>
#include <map>
#include <string>

#include <Utilities/strutils.h>

namespace Agenda
{

class Time;

class TotalTime
{
public:
    TotalTime();

    unsigned int            GetHour() const;
    unsigned int            GetMinute() const;

    std::tstring            String() const;

    TotalTime& operator+=(const Time& aTime);
    TotalTime& operator+=(int aMinutes);

private:
    unsigned int            mMinutes;
};

} // namespace Agenda