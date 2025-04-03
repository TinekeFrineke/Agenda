
#include "TotalTime.h"

#include "MyTime.h"

//#include <assert.h>  
//
//#include <Utilities/Inifile.h>
//#include <Utilities/strutils.h>


namespace Agenda
{

TotalTime::TotalTime()
    : mMinutes(0)
{
}

unsigned int TotalTime::GetHour() const
{
    return mMinutes / 60;
}

unsigned int TotalTime::GetMinute() const
{
    return mMinutes % 60;
}

std::string TotalTime::String() const
{
    std::string Text(std::to_string(GetHour()));
    Text += ":";
    char min[10];
    sprintf_s(min, "%02u", GetMinute());
    Text += min;
    return Text;
}

TotalTime& TotalTime::operator+=(const Time& aTime)
{
    mMinutes += aTime.GetHour() * 60 + aTime.GetMinute();
    return *this;
}

TotalTime& TotalTime::operator+=(int aMinutes)
{
    mMinutes += aMinutes;
    return *this;
}

} // namespace Agenda
