#pragma once

#include <list>
#include <map>
#include <string>

#include "agenda.h"

namespace Agenda
{

class Agenda;

void GetTotals(const Agenda & agenda, std::map<std::tstring, Time, Str::ci_less> & aTotalMap);
void GetTotals(std::list<const Agenda*> agendas, std::map<std::tstring, Time, Str::ci_less>& aTotalMap);
void GetWeekTotals(const Date& dateInWeek, std::map<std::tstring, Time, Str::ci_less>& aTotalMap);

} // namespace Agenda