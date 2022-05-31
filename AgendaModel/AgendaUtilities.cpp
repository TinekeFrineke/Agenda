

#include "AgendaUtilities.h"

#include "Utilities/DateUtils.h"
#include "Utilities/Date.h"

namespace Agenda
{
void GetTotals(const Agenda & agenda, std::map<std::tstring, Time, Str::ci_less>& aTotalMap)
{
  for (Agenda::Agenda::ItemList::const_iterator iter = agenda.Begin();
        iter != agenda.End();
        iter++)
  {
    aTotalMap[iter->GetTaskName()] += (iter->GetEnd() - iter->GetBegin());
  }
}

void GetTotals(std::list<const Agenda*> agendas, std::map<std::tstring, Time, Str::ci_less>& aTotalMap)
{
    for (const auto* agenda : agendas)
    {
        GetTotals(*agenda, aTotalMap);
    }
}

//void GetWeekTotals(const Date& dateInWeek, std::map<std::tstring, Time, Str::ci_less>& aTotalMap)
//{
//    SYSTEMTIME stime = ::Date::GetSundayBefore(dateInWeek.ToSystemTime());
//    Utils::Date date(Utils::Date::FromSystemTime(stime));
//
//    for (size_t i = 0; i < 5; ++i)
//    {
//        date.AddDays(1);
//    }
//}

} // namespace Agenda