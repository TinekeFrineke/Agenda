#pragma once

#include <vector>

#include <Utilities/Path.h>

#include "../AgendaModel/Agenda.h"


class Inifile;


class Settings
{
public:
    enum class Type { WorkAndPlay, OldAgenda, Combined,  Both };
    struct Activity
    {
        Activity(const std::string & description, bool addtime)
            : m_Description(description), m_AddTimeToTotal(addtime) {}
        std::string m_Description;
        bool m_AddTimeToTotal;
    };

    Settings(const Path & path);

    void AddDefaultActivity(const std::string& rName, bool KeepScore);

    void FillFrom(const Inifile& inifile);
    void WriteTo(Inifile& inifile);

    bool HasDefaultActivity(const std::string & description) const;
    bool ShouldAddDefeaultActivity(const std::string & description) const;

    const std::vector<Activity> & GetDefaultActivities() const  { return m_DefaultActivities; }
    const Agenda::Date & GetDefaultStartDate() const   { return m_DefaultStartDate; }
    const Agenda::Date & GetDefaultEndDate() const     { return m_DefaultEndDate; }
    const Path &         GetDataPath() const           { return m_DataPath; }

    void SetDefaultStartDate(const Agenda::Date & date)   { m_DefaultStartDate = date; }
    void SetDefaultEndDate(const Agenda::Date & date)     { m_DefaultEndDate = date; }

    void LoadAgenda(Agenda::Agenda & agenda, const Agenda::Date & date) const;
    void LoadAgenda(Agenda::Agenda & agenda, const Path & path) const;
    void SaveAgenda(const Agenda::Agenda & agenda, const Agenda::Date & date) const;
    void SaveAgenda(const Agenda::Agenda & agenda, const Path & path) const;

    Type GetType() const { return m_AgendaType; }

private:
    Path                    m_DataPath;

    Type m_AgendaType;
    Agenda::Date            m_DefaultStartDate;
    Agenda::Date            m_DefaultEndDate;
    std::vector<Activity>   m_DefaultActivities;
};
