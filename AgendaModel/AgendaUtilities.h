#pragma once

#include <list>
#include <map>
#include <string>

#include <Utilities/strutils.h>

#include "agenda.h"

namespace Agenda
{

class Agenda;

void GetTotals(const Agenda & agenda, std::map<std::string, Time, Str::ci_less> & aTotalMap);
void GetTotals(std::list<const Agenda*> agendas, std::map<std::string, Time, Str::ci_less>& aTotalMap);
Time GetWorkedTime(const Agenda& agenda, const std::vector<std::string>& activitiesToCount);

} // namespace Agenda