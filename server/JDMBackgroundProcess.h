/*
 *     Module Name       : JDMBackgroundProcess.h
 *
 *     Author/Location   : J.R.F. De Maeijer, Amstelveen
 *
 *     Copyright         : 2013, J.R.F. De Maeijer, Amstelveen
 *
 * ----------------------------------------------------------------------------
 * Copyright (c) 2013, J.R.F. De Maeijer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 *                            MODIFICATION HISTORY
 * ----------------------------------------------------------------------------
 * DATE        REASON                                                    AUTHOR
 * ----------------------------------------------------------------------------
 * 15-Oct-2013 Initial Release                                           J.D.M.
 * 19-Dec-2013 - Structured the interface functions, made the functions
 *               PauseReceived() and ContinueReceived() private.
 *             - Added the functions GetProcessName() and
 *               GetProcessDescription()                                 J.D.M.
 * 12-Jun-2014 - Added enum JDM_BGP_LOG_TYPES
 *             - Added functions Kill(), WriteToSystemLog(), Destruct()
 *               and ServiceReportEvent().                               J.D.M.
 * ----------------------------------------------------------------------------
 */

#ifndef __JDMBackgroundProcess_h__
#define __JDMBackgroundProcess_h__

#undef UNICODE

//#define JDM_BGP_STATIC_LINKING

#undef JDM_BGP_CALL_CONV
#undef JDM_BGP_INTERFACE_DEF_NOCC
#undef JDM_BGP_INTERFACE_DEF

#ifdef JDM_BGP_STATIC_LINKING
#  define JDM_BGP_CALL_CONV
#  define JDM_BGP_INTERFACE_DEF_NOCC
#  define JDM_BGP_INTERFACE_DEF(x)   x
#else
#  define JDM_BGP_CALL_CONV  __stdcall
//#  define JDM_BGP_CALL_CONV  __cdecl
#  ifdef JDM_BGP_EXPORTS
#    define JDM_BGP_INTERFACE_DEF_NOCC __declspec(dllexport)                     /* used to export DLL names */
#    define JDM_BGP_INTERFACE_DEF(x)   __declspec(dllexport) x JDM_BGP_CALL_CONV /* used to export DLL names */
#  else
#    define JDM_BGP_INTERFACE_DEF_NOCC __declspec(dllimport)                     /* used to import DLL names */
#    define JDM_BGP_INTERFACE_DEF(x)   __declspec(dllimport) x JDM_BGP_CALL_CONV /* used to import DLL names */
#  endif
#endif

#include <windows.h>
#include <string>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

enum JDM_BGP_ERRORS {
  JDM_BGP_NO_ERROR    =  0
 ,JDM_BGP_ERROR_START = -1
};

enum JDM_BGP_EVENTS {
  JDM_BGP_EVENT_UNKNOWN
 ,JDM_BGP_EVENT_STOP
 ,JDM_BGP_EVENT_PAUSE
 ,JDM_BGP_EVENT_CONTINUE
 ,JDM_BGP_EVENT_INTERROGATE
 ,JDM_BGP_EVENT_SHUTDOWN
 ,JDM_BGP_EVENT_PARAMCHANGE
 ,JDM_BGP_EVENT_NETBINDADD
 ,JDM_BGP_EVENT_NETBINDREMOVE
 ,JDM_BGP_EVENT_NETBINDENABLE
 ,JDM_BGP_EVENT_NETBINDDISABLE
 ,JDM_BGP_EVENT_DEVICEEVENT
 ,JDM_BGP_EVENT_HARDWAREPROFILECHANGE
 ,JDM_BGP_EVENT_POWEREVENT
 ,JDM_BGP_EVENT_SESSIONCHANGE
 ,JDM_BGP_EVENT_PRESHUTDOWN
 ,JDM_BGP_EVENT_CTRL_BREAK
 ,JDM_BGP_EVENT_CTRL_C
 ,JDM_BGP_EVENT_CTRL_CLOSE
 ,JDM_BGP_EVENT_CTRL_LOGOFF
 ,JDM_BGP_EVENT_CTRL_SHUTDOWN
 ,JDM_BGP_EVENT_CTRL_UNKNOWN
};

enum JDM_BGP_CURRENT_STATES {
  JDM_BGP_CURRENT_STATE_UNKNOWN          = 0x00000000
 ,JDM_BGP_CURRENT_STATE_STOPPED          = 0x00000001
 ,JDM_BGP_CURRENT_STATE_START_PENDING    = 0x00000002
 ,JDM_BGP_CURRENT_STATE_STOP_PENDING     = 0x00000003
 ,JDM_BGP_CURRENT_STATE_RUNNING          = 0x00000004
 ,JDM_BGP_CURRENT_STATE_CONTINUE_PENDING = 0x00000005
 ,JDM_BGP_CURRENT_STATE_PAUSE_PENDING    = 0x00000006
 ,JDM_BGP_CURRENT_STATE_PAUSED           = 0x00000007
};

enum JDM_BGP_ACCEPTED_CONTROLS {
  JDM_BGP_ACCEPTED_CONTROL_NONE                  = 0x00000000
 ,JDM_BGP_ACCEPTED_CONTROL_STOP                  = 0x00000001
 ,JDM_BGP_ACCEPTED_CONTROL_PAUSE_CONTINUE        = 0x00000002
 ,JDM_BGP_ACCEPTED_CONTROL_SHUTDOWN              = 0x00000004
 ,JDM_BGP_ACCEPTED_CONTROL_PARAMCHANGE           = 0x00000008
 ,JDM_BGP_ACCEPTED_CONTROL_NETBINDCHANGE         = 0x00000010
 ,JDM_BGP_ACCEPTED_CONTROL_HARDWAREPROFILECHANGE = 0x00000020
 ,JDM_BGP_ACCEPTED_CONTROL_POWEREVENT            = 0x00000040
 ,JDM_BGP_ACCEPTED_CONTROL_SESSIONCHANGE         = 0x00000080
 ,JDM_BGP_ACCEPTED_CONTROL_PRESHUTDOWN           = 0x00000100
};

enum JDM_BGP_LOG_TYPES {
  JDM_BGP_LOG_SUCCESS         
 ,JDM_BGP_LOG_ERROR_TYPE      
 ,JDM_BGP_LOG_WARNING_TYPE    
 ,JDM_BGP_LOG_INFORMATION_TYPE
 ,JDM_BGP_LOG_AUDIT_SUCCESS   
 ,JDM_BGP_LOG_AUDIT_FAILURE
};

typedef struct {
  JDM_BGP_CURRENT_STATES    CurrentState;
  JDM_BGP_ACCEPTED_CONTROLS AcceptedControls;
  long                      ExitCode;
  long                      BGPSpecificExitCode;
  long                      CheckPoint;
  long                      ExpectedTimeMillisecs;
} JDM_BGP_STATUS;

#ifdef __cplusplus
}

/*---------------------------------------------------------------------------*/
/* CLASS JDMBackgroundProcess                                                */
/*---------------------------------------------------------------------------*/
class JDMBackgroundProcess {
  public:
    /* Construction / Destruction */
    JDM_BGP_INTERFACE_DEF_NOCC JDMBackgroundProcess(char* ProcessName,
                                                    char* ProcessDescription,
                                                    int (JDM_BGP_CALL_CONV *MainFunction)(int argc, char* argv[]));
    JDM_BGP_INTERFACE_DEF_NOCC JDMBackgroundProcess(char* ProcessName,
                                                    char* ProcessDescription,
                                                    int (JDM_BGP_CALL_CONV *MainFunction)(int argc, char* argv[]),
                                                    void (JDM_BGP_CALL_CONV *EventNotifier)(JDM_BGP_EVENTS EventCode));
    JDM_BGP_INTERFACE_DEF_NOCC ~JDMBackgroundProcess();

    /* Installing / Uninstalling */
    JDM_BGP_INTERFACE_DEF (long)  Install(int argc, char* argv[]);
    JDM_BGP_INTERFACE_DEF (long)  Uninstall(int argc, char* argv[]);

    /* Runtime Starting */
    JDM_BGP_INTERFACE_DEF (long)  StartInBackground();
    JDM_BGP_INTERFACE_DEF (long)  StartInForeground(int argc, char* argv[]);
    JDM_BGP_INTERFACE_DEF (bool)  IsStartedInForeground() {return _IsStartedInForeground;}
    JDM_BGP_INTERFACE_DEF (bool)  IsStartedInBackground() {return !_IsStartedInForeground;}

    /* Runtime Interaction */
    JDM_BGP_INTERFACE_DEF (void)  ReportRunning();
    JDM_BGP_INTERFACE_DEF (void)  CheckAndHandlePauseResume(long SleepTime);
    JDM_BGP_INTERFACE_DEF (void)  CheckAndHandlePauseResume(long SleepTime, void (JDM_BGP_CALL_CONV *ExtraPauseFunction)());
    JDM_BGP_INTERFACE_DEF (bool)  StopReceived() {return _Status.CurrentState==JDM_BGP_CURRENT_STATE_STOP_PENDING;}
    JDM_BGP_INTERFACE_DEF (void)  Kill(long ExitCode);

    /* Miscellaneous */
    JDM_BGP_INTERFACE_DEF (const char*) GetProcessName() {return _ProcessName.c_str();}
    JDM_BGP_INTERFACE_DEF (const char*) GetProcessDescription() {return _ProcessDescription.c_str();}
    JDM_BGP_INTERFACE_DEF (void)        WriteToSystemLog(JDM_BGP_LOG_TYPES LogType, char* Message);

  private:
    /* Destruction */
    void        Destruct();

  private:
    static long SetAndReportStatus(JDM_BGP_CURRENT_STATES CurrentState, long ExitCode, long ExpectedTimeMillisecs);
    static long ReportStatusToOS(const JDM_BGP_STATUS& CurrentStatus);
    static long Pause();
    static long Continue();
    static long Stop();
    bool        PauseReceived() {return _Status.CurrentState==JDM_BGP_CURRENT_STATE_PAUSE_PENDING;}
    bool        ContinueReceived() {return _Status.CurrentState==JDM_BGP_CURRENT_STATE_CONTINUE_PENDING;}

    static int  (JDM_BGP_CALL_CONV *_MainFunction)(int argc, char* argv[]);
    static void (JDM_BGP_CALL_CONV *_EventNotifierFunction)(JDM_BGP_EVENTS EventCode);
    static string                   _ProcessName;
    static string                   _ProcessDescription;
    static bool                     _IsStartedInForeground;
    static JDM_BGP_STATUS           _Status;

#ifdef _WIN32
  private: /*WINDOWS*/
    static VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
    static VOID WINAPI ServiceCtrlHandler(DWORD CtrlCode);
    static BOOL        ConsoleCtrlHandler(DWORD CtrlCode);
    long               ServiceInstall(int argc, char* argv[]);
    long               ServiceUninstall(int argc, char* argv[]);
    static void        ServiceReportEvent(JDM_BGP_LOG_TYPES LogType, const char* ProcessName, char* Message);
    static SERVICE_STATUS_HANDLE   _WinStatusHandle;
#else
#endif

}; /* CLASS JDMBackgroundProcess */

#endif

#endif
