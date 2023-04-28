
#include "TotalTime.h"

#include "Time.h"

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

std::tstring TotalTime::String() const
{
    std::tstring Text(Str::ToTString(std::to_string(GetHour())));
    Text += _T(":");
    TCHAR min[10];
    _stprintf_s(min, _T("%02u"), GetMinute());
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
