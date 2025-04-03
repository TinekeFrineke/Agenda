#pragma once


namespace Utils {
class Date;
}


namespace Agenda
{

class Agenda;

class IAgendaLoader
{
public:
    virtual bool Load(const Utils::Date& date, Agenda& agenda) const = 0;
};

class WeekLoader
    : public IAgendaLoader
{
public:
};

} // namespace Agenda