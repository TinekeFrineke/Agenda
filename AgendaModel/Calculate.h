
#pragma once

#include <map>
#include <string>

#include "agenda.h"

namespace Agenda
{

void GrandTotal(const std::tstring &                          aTimeFrom,
                const std::tstring &                          aTimeTo,
                std::map<std::tstring, Time, Str::ci_less> &  aTotalMap);
// By default, timeto is today.
void GrandTotal(const std::tstring &                          aTimeFrom,
                std::map<std::tstring, Time, Str::ci_less> &  aTotalMap);

} // namespace Agenda
