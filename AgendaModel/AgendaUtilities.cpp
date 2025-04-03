

#include "AgendaUtilities.h"

#include "Utilities/Date.h"

namespace Agenda
{
void GetTotals(const Agenda & agenda, std::map<std::string, Time, Str::ci_less>& aTotalMap)
{
  for (Agenda::Agenda::ItemList::const_iterator iter = agenda.Begin();
        iter != agenda.End();
        iter++)
  {
    aTotalMap[iter->GetTaskName()] += (iter->GetEnd() - iter->GetBegin());
  }
}

void GetTotals(std::list<const Agenda*> agendas, std::map<std::string, Time, Str::ci_less>& aTotalMap)
{
    for (const auto* agenda : agendas)
    {
        GetTotals(*agenda, aTotalMap);
    }
}

Time GetWorkedTime(const Agenda& agenda, const std::vector<std::string>& activitiesToIgnore)
{
    typedef std::map<std::string, Time, Str::ci_less> TotalsMap;
    TotalsMap totalsMap;
    GetTotals(agenda, totalsMap);

    Time totalTime;
    for (TotalsMap::const_iterator iter = totalsMap.begin(); iter != totalsMap.end(); ++iter)
        if (std::find(activitiesToIgnore.begin(), activitiesToIgnore.end(), iter->first) == activitiesToIgnore.end())
            totalTime += iter->second;

    return totalTime;
}

} // namespace Agenda