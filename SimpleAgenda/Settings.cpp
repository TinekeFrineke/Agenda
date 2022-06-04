#include "stdafx.h"

#include <Utilities/Inifile.h>

#include "Settings.h"

Settings::Settings(const Path & path)
: m_DataPath(path)
{
}

void Settings::AddDefaultActivity(const std::tstring& rName, bool KeepScore)
{
    m_DefaultActivities.push_back(Activity(rName, KeepScore));
}

void Settings::FillFrom(const Inifile & inifile)
{
  std::tstringstream stream(inifile[_T("General")][_T("StartDate")]);
  stream >> m_DefaultStartDate;

  stream.str(inifile[_T("General")][_T("EndDate")]);
  stream >> m_DefaultEndDate;

  unsigned int count(_ttoi(inifile[_T("Items")][_T("Count")].c_str()));
  for (unsigned int i = 0; i < count; ++i) {
    TCHAR number[10];
    _stprintf_s(number, _T("%u"), i);
    stream.str(inifile[_T("Items")][number]);
    if (number[0] != 0) {
        size_t pos = stream.str().find(_T(';'));
        if (pos != std::string::npos && pos > 0) {
            AddDefaultActivity(stream.str().substr(0, pos),
                stream.str()[stream.str().length() - 1] == _T('0') ? false : true);
        }
    }
  }
}

void Settings::WriteTo(Inifile & inifile)
{
  inifile[_T("General")][_T("StartDate")] = m_DefaultStartDate.String();
  inifile[_T("General")][_T("EndDate")]   = m_DefaultEndDate.String();

  TCHAR number[10];
  _stprintf_s(number, _T("%zu"), m_DefaultActivities.size());
  inifile[_T("Items")][_T("Count")]       = number;

  for (unsigned int i = 0; i < m_DefaultActivities.size(); ++i) {
    _stprintf_s(number, _T("%u"), i);
    inifile[_T("Items")][number] = m_DefaultActivities[i].m_Description
                                   + (m_DefaultActivities[i].m_AddTimeToTotal ? _T(";1") : _T(";0"));
  }
}

bool Settings::HasDefaultActivity(const std::tstring & description) const
{
  for (size_t i = 0; i < m_DefaultActivities.size(); ++i)
    if (m_DefaultActivities[i].m_Description == description)
      return true;

  return false;
}

bool Settings::ShouldAddDefeaultActivity(const std::tstring & description) const
{
  for (size_t i = 0; i < m_DefaultActivities.size(); ++i)
    if (m_DefaultActivities[i].m_Description == description)
      return m_DefaultActivities[i].m_AddTimeToTotal;

  return false;
}

void Settings::LoadAgenda(Agenda::Agenda & agenda, const Agenda::Date & date) const
{
  Path path(m_DataPath);
  path += date.String() + _T(".age");
  LoadAgenda(agenda, path);
}

void Settings::LoadAgenda(Agenda::Agenda & agenda, const Path & path) const
{
  agenda.Clear();
  std::tifstream stream(path.AsString().c_str());
  stream >> agenda;
}

void Settings::SaveAgenda(const Agenda::Agenda & agenda, const Agenda::Date & date) const
{
  if (!agenda.Empty()) {
    Path path(m_DataPath);
    path += date.String() + _T(".age");
    SaveAgenda(agenda, path);
  }
}

void Settings::SaveAgenda(const Agenda::Agenda & agenda, const Path & path) const
{
  if (!agenda.Empty()) {
    std::tofstream stream(path.AsString().c_str());
    stream << agenda;
  }
}
