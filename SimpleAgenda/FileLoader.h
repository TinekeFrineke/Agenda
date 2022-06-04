#pragma once


#include <AgendaModel/IAgendaLoader.h>

class Path;
class Settings;

class FileLoader
    : public Agenda::IAgendaLoader
{
public:
    FileLoader(const Settings& settings);

    virtual bool LoadAgenda(const Agenda::Date& date,
                            Agenda::Agenda& agenda) const override;

private:
    bool LoadAgenda(Agenda::Agenda& agenda, const Path& path) const;

    const Settings& m_Settings;
};

