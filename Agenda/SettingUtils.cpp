#include "stdafx.h"

#include "SettingUtils.h"

#include <Utilities/PathUtils.h>

#include "Settings.h"

namespace SettingUtils
{

AgendaLoaderFromFile::AgendaLoaderFromFile(const Settings& settings)
    : m_Settings(settings)
{
}

bool AgendaLoaderFromFile::Load(const Agenda::Date& date, Agenda::Agenda& agenda) const
{
    Path path(m_Settings.GetDataPath());
    path += date.String() + _T(".age");
    Load(agenda, path);
    return !agenda.Empty();
}

void AgendaLoaderFromFile::Load(Agenda::Agenda& agenda, const Path& path) const
{
    agenda.Clear();
    std::tifstream stream(path.AsString().c_str());
    stream >> agenda;
}

std::vector<std::tstring> ActvitiesToIgnore(const Settings& settings)
{
    const std::vector<Settings::Activity> activities(settings.GetDefaultActivities());

    std::vector<std::tstring> toIgnore;

    for (const auto& activity : activities)
    {
        if (!activity.m_AddTimeToTotal)
            toIgnore.push_back(activity.m_Description);
    }

    return toIgnore;
}

}
