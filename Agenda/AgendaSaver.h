#pragma once


#include <AgendaModel/IAgendaSaver.h>

class Path;
class Settings;

class AgendaSaver
    : public Agenda::IAgendaSaver
{
public:
    AgendaSaver(const Settings& settings);

    virtual bool Save(const Agenda::Date& date,
                      const Agenda::Agenda& agenda) const override;

private:
    bool Save(const Agenda::Agenda& agenda, const Path& path) const;

    const Settings& m_Settings;
};

