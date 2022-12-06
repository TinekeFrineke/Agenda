#pragma once

#include <list>

namespace Agenda
{

class Agenda;
class Date;


class IAgendaSaver
{
public:
    virtual bool Save(const Date& date, const Agenda& agenda) const = 0;
};

} // namespace Agenda