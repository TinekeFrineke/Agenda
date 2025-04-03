#pragma once


namespace Utils {
class Date;
}


namespace Agenda
{

class Agenda;
class Date;


class IAgendaSaver
{
public:
    virtual bool Save(const Utils::Date& date, const Agenda& agenda) const = 0;
};

} // namespace Agenda