
#include "calculate.h"

#include <stdio.h>
#include <fstream>

#include <Utilities/strutils.h>

#include "AgendaUtilities.h"


namespace Agenda
{

void GrandTotal(const std::tstring &                         aTimeFrom,
                const std::tstring &                         aTimeTo,
                std::map<std::tstring, Time, Str::ci_less> & aTotalMap)
{
  std::list<std::tstring> filenames;

  WIN32_FIND_DATA data;
  std::tstring fileprefix = _T("*.age");
  HANDLE file = FindFirstFile(fileprefix.c_str(), &data);
  if (file != INVALID_HANDLE_VALUE)
  {
    // Don't remove "." and ".."!
    if (data.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
      filenames.push_back(data.cFileName);

    // Retrieve next file starting with aDatasetName in aSourceDir:
    while (FindNextFile(file, &data) != FALSE)
    {
      std::list<std::tstring>::iterator iter = filenames.begin();
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

  for (std::list<std::tstring>::const_iterator citer = filenames.begin();
       citer != filenames.end();
       citer++)
  {
    std::tstring timestring = citer->substr(0, citer->find('.'));

    if (timestring >= aTimeFrom && timestring <= aTimeTo)
    {
      try
      {
        Agenda agenda;
        std::tifstream instream(Str::ToString(citer->c_str()).c_str(), std::ios::in);
        if (instream.bad())
        {
          MessageBox(0, (_T("Error reading ") + *citer).c_str(), _T("Error"), MB_OK);
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
        MessageBox(0, _T("Exception reading file"), _T("ERROR"), MB_OK);
      }
    }
  }
}


void GrandTotal(const std::tstring &                         aTimeFrom,
                std::map<std::tstring, Time, Str::ci_less> & aTotalMap)
{
  SYSTEMTIME timeto;
  GetLocalTime(&timeto);

  TCHAR stimeto[9];
  _stprintf_s(stimeto, _T("%04d%02d%02d"), timeto.wYear, timeto.wMonth, timeto.wDay);

  GrandTotal(aTimeFrom, stimeto, aTotalMap);
}

} // namespace Agenda