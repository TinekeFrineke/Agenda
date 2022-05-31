#include "stdafx.h"

#include "SettingUtils.h"

#include <Utilities/PathUtils.h>

#include "Settings.h"



namespace Agenda
{

namespace SettingUtils
{

AgendaLoaderFromFile::AgendaLoaderFromFile(const Settings& settings)
    : m_Settings(settings)
{
}

bool AgendaLoaderFromFile::LoadAgenda(const Date& date, Agenda& agenda) const
{
    Path path(m_Settings.GetDataPath());
    path += date.String() + _T(".age");
    LoadAgenda(agenda, path);
    return !agenda.Empty();
}

void AgendaLoaderFromFile::LoadAgenda(Agenda& agenda, const Path& path) const
{
    agenda.Clear();
    std::tifstream stream(path.AsString().c_str());
    stream >> agenda;
}

}

} // namespace SettingUtils

