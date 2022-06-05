#pragma once

#include <Utilities/PathUtils.h>

#include "../AgendaModel/Agenda.h"

class Inifile;
class Path;

class Settings
{
public:
    enum class Type { WorkAndPlay, OldAgenda };
    struct Activity
    {
        Activity(const std::tstring & description, bool addtime)
            : m_Description(description), m_AddTimeToTotal(addtime) {}
        std::tstring          m_Description;
        bool                  m_AddTimeToTotal;
    };

    Settings(const Path & path);

    void AddDefaultActivity(const std::tstring& rName, bool KeepScore);

    void                    FillFrom(const Inifile & inifile);
    void                    WriteTo(Inifile & inifile);

    bool                    HasDefaultActivity(const std::tstring & description) const;
    bool                    ShouldAddDefeaultActivity(const std::tstring & description) const;

    const std::vector<Activity> &
                            GetDefaultActivities() const  { return m_DefaultActivities; }
    const Agenda::Date &    GetDefaultStartDate() const   { return m_DefaultStartDate; }
    const Agenda::Date &    GetDefaultEndDate() const     { return m_DefaultEndDate; }
    const Path &            GetDataPath() const           { return m_DataPath; }

    void                    SetDefaultStartDate(const Agenda::Date & date)   { m_DefaultStartDate = date; }
    void                    SetDefaultEndDate(const Agenda::Date & date)     { m_DefaultEndDate = date; }

    void                    LoadAgenda(Agenda::Agenda & agenda, const Agenda::Date & date) const;
    void                    LoadAgenda(Agenda::Agenda & agenda, const Path & path) const;
    void                    SaveAgenda(const Agenda::Agenda & agenda, const Agenda::Date & date) const;
    void                    SaveAgenda(const Agenda::Agenda & agenda, const Path & path) const;

    Type GetType() const { return m_AgendaType; }

private:
    Path                    m_DataPath;

    Type m_AgendaType;
    Agenda::Date            m_DefaultStartDate;
    Agenda::Date            m_DefaultEndDate;
    std::vector<Activity>   m_DefaultActivities;
};
