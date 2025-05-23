#include "stdafx.h"

#include "AgendaSaver.h"

#include <Utilities/Date.h>
#include <Utilities/Path.h>

#include "Settings.h"
#include <Agenda/Logger.h>

AgendaSaver::AgendaSaver(const Settings& settings)
    : m_Settings(settings)
{
}

bool AgendaSaver::Save(const Utils::Date& date, const Agenda::Agenda& agenda) const
{
    Path path(m_Settings.GetDataPath());
    path += Utils::ToString(date) + _T(".age");
    return Save(agenda, path);
}

bool AgendaSaver::Save(const Agenda::Agenda& agenda, const Path& path) const
{
    std::ofstream outstream(path.AsString());
 
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
