#include "stdafx.h"

#include "FileLoader.h"

#include <Utilities/PathUtils.h>

#include "Settings.h"
#include <Agenda/Logger.h>

FileLoader::FileLoader(const Settings& settings)
    : m_Settings(settings)
{
}

bool FileLoader::Load(const Agenda::Date& date, Agenda::Agenda& agenda) const
{
    Path path(m_Settings.GetDataPath());
    path += date.String() + _T(".age");
    return Load(agenda, path);
}

bool FileLoader::Load(Agenda::Agenda& agenda, const Path& path) const
{
    agenda.Clear();
    std::tifstream stream(path.AsString().c_str());
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
