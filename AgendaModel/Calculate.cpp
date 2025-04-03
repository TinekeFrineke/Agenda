
#include "calculate.h"

#include <stdio.h>
#include <fstream>

#include <Utilities/strutils.h>

#include "AgendaUtilities.h"


namespace Agenda
{

void GrandTotal(const std::string &                         aTimeFrom,
                const std::string &                         aTimeTo,
                std::map<std::string, Time, Str::ci_less> & aTotalMap)
{
  std::list<std::string> filenames;

  WIN32_FIND_DATA data;
  std::string fileprefix = "*.age";
  HANDLE file = FindFirstFile(fileprefix.c_str(), &data);
  if (file != INVALID_HANDLE_VALUE)
  {
    // Don't remove "." and ".."!
    if (data.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
      filenames.push_back(data.cFileName);

    // Retrieve next file starting with aDatasetName in aSourceDir:
    while (FindNextFile(file, &data) != FALSE)
    {
      std::list<std::string>::iterator iter = filenames.begin();
      if (data.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
      {
        while (iter != filenames.end() && *iter < data.cFileName)
          iter++;

        filenames.insert(iter, data.cFileName);
      }
    }

    // Close the file handle (otherwise the directory can't be removed):
    FindClose(file);
  }

  for (std::list<std::string>::const_iterator citer = filenames.begin();
       citer != filenames.end();
       citer++)
  {
    std::string timestring = citer->substr(0, citer->find('.'));

    if (timestring >= aTimeFrom && timestring <= aTimeTo)
    {
      try
      {
        Agenda agenda;
        std::ifstream instream(*citer, std::ios::in);
        if (instream.bad())
        {
          MessageBox(0, ("Error reading " + *citer).c_str(), "Error", MB_OK);
          continue;
        }
        else if (instream.is_open())
        {
          instream >> agenda;
          GetTotals(agenda, aTotalMap);
          // Else file does not exist
        }
      }
      catch(...)
      {
        MessageBox(0, "Exception reading file", "ERROR", MB_OK);
      }
    }
  }
}


void GrandTotal(const std::string &                         aTimeFrom,
                std::map<std::string, Time, Str::ci_less> & aTotalMap)
{
  SYSTEMTIME timeto;
  GetLocalTime(&timeto);

  char stimeto[9];
  sprintf_s(stimeto, "%04d%02d%02d", timeto.wYear, timeto.wMonth, timeto.wDay);

  GrandTotal(aTimeFrom, stimeto, aTotalMap);
}

} // namespace Agenda