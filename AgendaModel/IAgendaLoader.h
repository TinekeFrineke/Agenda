#pragma once

#include <list>

namespace Agenda
{

class Agenda;
class Date;


class IAgendaLoader
{
public:
    virtual bool LoadAgenda(const Date& date, Agenda& agenda) const = 0;
};

class WeekLoader
    : public IAgendaLoader
{
public:
};

} // namespace Agenda