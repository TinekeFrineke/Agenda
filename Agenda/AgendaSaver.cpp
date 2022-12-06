#include "stdafx.h"

#include "AgendaSaver.h"

#include <Utilities/PathUtils.h>

#include "Settings.h"
#include <Agenda/Logger.h>

AgendaSaver::AgendaSaver(const Settings& settings)
    : m_Settings(settings)
{
}

bool AgendaSaver::Save(const Agenda::Date& date, const Agenda::Agenda& agenda) const
{
    Path path(m_Settings.GetDataPath());
    path += date.String() + _T(".age");
    return Save(agenda, path);
}

bool AgendaSaver::Save(const Agenda::Agenda& agenda, const Path& path) const
{
    std::wofstream outstream(path.AsString().c_str());
 
    if (!outstream.is_open())
    {
        Logger::Instance() << "AgendaSaver::LoadAgenda could not load "
            << path.AsString().c_str()
            << Logger::endl;
        return false;
    }

    outstream << agenda;
    return true;
}
