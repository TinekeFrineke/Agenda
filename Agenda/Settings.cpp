#include "stdafx.h"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <Utilities/Inifile.h>

#include "Settings.h"

namespace
{

const std::string cWorkAndPlay("WorkAndPlay");
const std::string cOldAgenda("OldAgenda");
const std::string cCombined("Combined");
const std::string cBoth("Both");

Settings::Type ToType(const std::string& aName)
{
    if (aName == cOldAgenda)
        return Settings::Type::OldAgenda;

    if (aName == cBoth)
        return Settings::Type::Both;

    if (aName == cWorkAndPlay)
        return Settings::Type::WorkAndPlay;

    return Settings::Type::Combined;
}

std::string ToString(Settings::Type aType)
{
    switch (aType)
    {
        case Settings::Type::OldAgenda:
            return cOldAgenda;
        case Settings::Type::WorkAndPlay:
            return cWorkAndPlay;
        case Settings::Type::Combined:
            return cCombined;
        case Settings::Type::Both:
            return cBoth;
        default:
            throw std::runtime_error("Wrong settings type");
    }
}

}

Settings::Settings(const Path & path)
    : m_DataPath(path)
    , m_AgendaType(Type::WorkAndPlay)
{
}

void Settings::AddDefaultActivity(const std::string& rName, bool KeepScore)
{
    m_DefaultActivities.push_back(Activity(rName, KeepScore));
}

void Settings::FillFrom(const Inifile & inifile)
{
  std::stringstream stream(inifile["General"]["StartDate"]);
  stream >> m_DefaultStartDate;

  stream.str(inifile["General"]["EndDate"]);
  stream >> m_DefaultEndDate;

  m_AgendaType = ToType(inifile["General"]["Type"]);

  unsigned int count(std::atoi(inifile["Items"]["Count"].c_str()));
  for (unsigned int i = 0; i < count; ++i) {
    char number[10];
    sprintf_s(number, "%u", i);
    stream.str(inifile["Items"][number]);
    if (number[0] != 0) {
        size_t pos = stream.str().find(';');
        if (pos != std::string::npos && pos > 0) {
            AddDefaultActivity(stream.str().substr(0, pos),
                               stream.str()[stream.str().length() - 1] == '0' ? false : true);
        }
    }
  }
}

void Settings::WriteTo(Inifile & inifile)
{
  inifile["General"]["StartDate"] = m_DefaultStartDate.String();
  inifile["General"]["EndDate"]   = m_DefaultEndDate.String();
  inifile["General"]["Type"] = ToString(m_AgendaType);


  char number[10];
  sprintf_s(number, "%zu", m_DefaultActivities.size());
  inifile["Items"]["Count"] = number;

  for (unsigned int i = 0; i < m_DefaultActivities.size(); ++i) {
    sprintf_s(number, _T("%u"), i);
    inifile["Items"][number] = m_DefaultActivities[i].m_Description
                                   + (m_DefaultActivities[i].m_AddTimeToTotal ? ";1" : ";0");
  }
}

bool Settings::HasDefaultActivity(const std::string & description) const
{
  for (size_t i = 0; i < m_DefaultActivities.size(); ++i)
    if (m_DefaultActivities[i].m_Description == description)
      return true;

  return false;
}

bool Settings::ShouldAddDefeaultActivity(const std::string & description) const
{
  for (size_t i = 0; i < m_DefaultActivities.size(); ++i)
    if (m_DefaultActivities[i].m_Description == description)
      return m_DefaultActivities[i].m_AddTimeToTotal;

  return false;
}

void Settings::LoadAgenda(Agenda::Agenda & agenda, const Agenda::Date & date) const
{
  Path path(m_DataPath);
  path += date.String() + ".age";
  LoadAgenda(agenda, path);
}

void Settings::LoadAgenda(Agenda::Agenda & agenda, const Path & path) const
{
  agenda.Clear();
  std::ifstream stream(path.AsString().c_str());
  stream >> agenda;
}

void Settings::SaveAgenda(const Agenda::Agenda & agenda, const Agenda::Date & date) const
{
  if (!agenda.Empty()) {
    Path path(m_DataPath);
    path += date.String() + ".age";
    SaveAgenda(agenda, path);
  }
}

void Settings::SaveAgenda(const Agenda::Agenda & agenda, const Path & path) const
{
  if (!agenda.Empty()) {
    std::ofstream stream(path.AsString());
    stream << agenda;
  }
}
