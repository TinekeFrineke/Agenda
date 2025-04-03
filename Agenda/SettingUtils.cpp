#include "stdafx.h"

#include "SettingUtils.h"

#include <Utilities/Date.h>
#include <Utilities/Path.h>

#include "Settings.h"

namespace SettingUtils
{

AgendaLoaderFromFile::AgendaLoaderFromFile(const Settings& settings)
    : m_Settings(settings)
{
}

bool AgendaLoaderFromFile::Load(const Utils::Date& date, Agenda::Agenda& agenda) const
{
    Path path(m_Settings.GetDataPath());
    path += Utils::ToString(date) + _T(".age");
    Load(agenda, path);
    return !agenda.Empty();
}

void AgendaLoaderFromFile::Load(Agenda::Agenda& agenda, const Path& path) const
{
    agenda.Clear();
    std::ifstream stream(path.AsString());
    stream >> agenda;
}

std::vector<std::string> ActvitiesToIgnore(const Settings& settings)
{
    const std::vector<Settings::Activity> activities(settings.GetDefaultActivities());

    std::vector<std::string> toIgnore;

    for (const auto& activity : activities)
    {
        if (!activity.m_AddTimeToTotal)
            toIgnore.push_back(activity.m_Description);
    }

    return toIgnore;
}

}
