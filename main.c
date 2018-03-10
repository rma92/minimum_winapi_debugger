#include <windows.h>
/*Rich A. Marino - Minimum Windows Debugger Code

This is the minimum amount of code to meet the requirements to be
a windows debugger.  In other words, the resultant executable can 
be invoked using Image File Execution Options, and the targeted
process can be started.

For example, to launch this executable each time notepad.exe is opened,
create a key at (no quotes)
"HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\notepad.exe"
Create a REG_SZ (String) called Debugger in there and set it to "main.exe"
(or the full path to whatever the resultant executable is called).

In this particular case, this serves no purpose as no debugging, nor anything
else particularly interesting occurs.  To verify that it worked with this code,
look in Task Manager and see that both the debug target and this program are running.
Close the debug target, and this program should close as well.

This sample code is public domain, as it's intended to be used as a template
for special purpose debuggers.

compile: pretty minimum - no dependencies beyond core windows Dlls.
tcc: tcc main.c
*/
int WINAPI WinMain( HINSTANCE hi, HINSTANCE hpi, LPSTR c, int s)
{
    DEBUG_EVENT debug_event = {0};
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line)
        c,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        DEBUG_ONLY_THIS_PROCESS,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        //silently fail
        return;
    }

    for(;;)
    {
      if( !WaitForDebugEvent( &debug_event, INFINITE ) )
      {
        //MessageBox(0, "debug loop aborted.", 0, 0);
        return;
      }
      //process debug events here.
      switch( debug_event.dwDebugEventCode )
      {
        case EXIT_PROCESS_DEBUG_EVENT:
          goto EGRESS;
        break;
      }
      //end process debug events.
      ContinueDebugEvent( debug_event.dwProcessId, debug_event.dwThreadId, DBG_CONTINUE );
    }
    
EGRESS:
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    return;
}

