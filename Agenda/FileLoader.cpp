#include "stdafx.h"

#include "FileLoader.h"

#include <Utilities/date.h>
#include <Utilities/Path.h>

#include "Settings.h"
#include <Agenda/Logger.h>

FileLoader::FileLoader(const Settings& settings)
    : m_Settings(settings)
{
}

bool FileLoader::Load(const Utils::Date& date, Agenda::Agenda& agenda) const
{
    Path path(m_Settings.GetDataPath());
    path += Utils::ToString(date) + _T(".age");
    return Load(agenda, path);
}

bool FileLoader::Load(Agenda::Agenda& agenda, const Path& path) const
{
    agenda.Clear();
    std::ifstream stream(path.AsString());
    if (!stream.is_open())
    {
        Logger::Instance() << "FileLoader::LoadAgenda could not load "
            << path.AsString().c_str()
            << Logger::endl;
        return false;
    }

    stream >> agenda;
    return true;
}
