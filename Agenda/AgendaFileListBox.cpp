#include "stdafx.h"
#include "AgendaFileListBox.h"

#include <algorithm>

#include <Utilities/PathUtils.h>

#include "Settings.h"

AgendaFileListBox::AgendaFileListBox(const Settings & settings)
: m_RootPath(settings.GetDataPath())
{
}


AgendaFileListBox::~AgendaFileListBox()
{
}

void AgendaFileListBox::Fill(const Path & aPath)
{
  std::vector<std::tstring> listboxtexts;

  Path path(aPath);
  path += _T("*.age");
  WIN32_FIND_DATA finddata;
  memset(&finddata, 0, sizeof(WIN32_FIND_DATA));
  HANDLE hFile = FindFirstFile(path.AsString().c_str(), &finddata);
  if (hFile !=  INVALID_HANDLE_VALUE) {
    listboxtexts.push_back(finddata.cFileName);
    while (FindNextFile(hFile, &finddata) != FALSE)
      listboxtexts.push_back(finddata.cFileName);

    FindClose(hFile);
  }

  std::sort(listboxtexts.begin(), listboxtexts.end());

  for (std::vector<std::tstring>::reverse_iterator riter = listboxtexts.rbegin(); riter != listboxtexts.rend(); ++riter)
    AddString(*riter);
}

void AgendaFileListBox::AddString(const std::tstring & string)
{
  CListBox::AddString(string.c_str());
}
