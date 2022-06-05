#include "stdafx.h"

#include "FileLoader.h"

#include <Utilities/PathUtils.h>

#include "Settings.h"

FileLoader::FileLoader(const Settings& settings)
    : m_Settings(settings)
{
}

bool FileLoader::LoadAgenda(const Agenda::Date& date, Agenda::Agenda& agenda) const
{
    Path path(m_Settings.GetDataPath());
    path += date.String() + _T(".age");
    return LoadAgenda(agenda, path);
}

bool FileLoader::LoadAgenda(Agenda::Agenda& agenda, const Path& path) const
{
    agenda.Clear();
    std::tifstream stream(path.AsString().c_str());
    if (!stream.is_open())
        return false;

    stream >> agenda;
    return true;
}
