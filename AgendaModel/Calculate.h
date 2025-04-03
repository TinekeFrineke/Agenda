
#pragma once

#include <map>
#include <string>

#include <Utilities/StrUtils.h>

#include "agenda.h"

namespace Agenda
{

void GrandTotal(const std::string &                          aTimeFrom,
                const std::string &                          aTimeTo,
                std::map<std::string, Time, Str::ci_less> &  aTotalMap);
// By default, timeto is today.
void GrandTotal(const std::string &                          aTimeFrom,
                std::map<std::string, Time, Str::ci_less> &  aTotalMap);

} // namespace Agenda
