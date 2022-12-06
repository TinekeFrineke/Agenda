#pragma once

#include <vector>

#include <AgendaModel/IAgendaLoader.h>
#include <Utilities/strutils.h>

class Path;
class Settings;

namespace Agenda
{

class Agenda;
class Date;

}

namespace SettingUtils
{

class AgendaLoaderFromFile : public Agenda::IAgendaLoader
{
public:
    AgendaLoaderFromFile(const Settings& settings);

    bool Load(const Agenda::Date& date, Agenda::Agenda& agenda) const override;

private:
    void Load(Agenda::Agenda& agenda, const Path& path) const;

    const Settings& m_Settings;
};

std::vector<std::tstring> ActvitiesToIgnore(const Settings& settings);

}
