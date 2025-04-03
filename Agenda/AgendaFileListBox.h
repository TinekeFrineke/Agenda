#pragma once

#include "afxwin.h"

#include <Utilities/Path.h>

class Settings;

class AgendaFileListBox:
    public CListBox
{
public:
    AgendaFileListBox(const Settings& settings);
    virtual ~AgendaFileListBox();

    void Fill(const Path& path);

    void AddString(const std::string& string);

private:
    Path m_RootPath;
};

