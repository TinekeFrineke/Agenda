#pragma once

#include <AgendaModel/IAgendaLoader.h>

class Path;
class Settings;

namespace Agenda
{

class Agenda;
class Date;

namespace SettingUtils
{

class AgendaLoaderFromFile : public IAgendaLoader
{
public:
    AgendaLoaderFromFile(const Settings& settings);

    bool LoadAgenda(const Date& date, Agenda& agenda) const override;

private:
    void LoadAgenda(Agenda& agenda, const Path& path) const;

    const Settings& m_Settings;
};

}

}
