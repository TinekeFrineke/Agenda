#pragma once

#include "afxwin.h"

#include <Utilities/PathUtils.h>

class Path;
class Settings;

class AgendaFileListBox:
  public CListBox
{
public:
  AgendaFileListBox(const Settings & settings);
  virtual ~AgendaFileListBox();

  void Fill(const Path & path);

  void AddString(const std::tstring & string);

private:
  Path        m_RootPath;
};

