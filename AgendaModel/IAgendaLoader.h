#pragma once

#include <list>

namespace Agenda
{

class Agenda;


class IAgendaLoader
{
public:
    virtual void Load(std::list<Agenda*>& agendas) = 0;
};

class WeekLoader
    : public IAgendaLoader
{
public:
};

} // namespace Agenda