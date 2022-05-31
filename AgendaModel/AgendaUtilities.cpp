

#include "AgendaUtilities.h"

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

} // namespace Agenda