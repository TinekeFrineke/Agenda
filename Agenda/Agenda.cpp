
// Agenda.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Agenda.h"
#include "AgendaDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <utilities/Inifile.h>
#include <utilities/Path.h>

#include "../AgendaModel/Agenda.h"
#include "Settings.h"
#include <Agenda/Logger.h>


// AgendaApplication

BEGIN_MESSAGE_MAP(AgendaApplication, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// AgendaApplication construction

AgendaApplication::AgendaApplication()
{
    // support Restart Manager
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only AgendaApplication object

AgendaApplication theApp;


// AgendaApplication initialization

BOOL AgendaApplication::InitInstance()
{
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();


    AfxEnableControlContainer();

    // Create the shell manager, in case the dialog contains
    // any shell tree view or shell list view controls.
    CShellManager* pShellManager = new CShellManager;

    // Activate "Windows Native" visual manager for enabling themes in MFC controls
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    char cpath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, cpath);

    Path path(cpath);
    Path logpath(path + "WorkAndPlay.log");
    Logger::CreateInstance(*logpath);

    // Create the current agenda item
    Agenda::Agenda agenda;
    Agenda::Date today;
    std::string agendaname(today.String() + ".age");
    Path agendapath(path + agendaname);

    // Create the agenda settings item
    Path settingspath(path + "agenda.ini");
    std::ifstream input(settingspath.AsString());
    Inifile inifile;
    input >> inifile;
    Settings settings(path);
    settings.FillFrom(inifile);

    std::ifstream instream(agendapath.AsString());
    instream >> agenda;

    AgendaDialog dlg(agenda, settings);
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
        TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
    }

    // Delete the shell manager created above.
    if (pShellManager != NULL)
    {
        delete pShellManager;
    }

#ifndef _AFXDLL
    ControlBarCleanUp();
#endif

    settings.WriteTo(inifile);
    std::ofstream output(settingspath.AsString());
    output << inifile;

    if (!agenda.Empty()) {
        std::ofstream outstream(agendapath.AsString().c_str());
        outstream << agenda;
    }

    Logger::DeleteInstance();

    // Since the dialog has been closed, return FALSE so that we exit the
      //  application, rather than start the application's message pump.
    return FALSE;
}

