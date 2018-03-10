# minimum_winapi_debugger
This repository contains a single C file with the minimum amount of code to make a functional 
debugger on Windows.  That is, the debugger can be launcehd using Image File Execution Options.

## Image File Execution Options
Windows contains a feature to assist in deubgging where the kernel can invoke a debugger when a process with a specific image name is invoked.  The debugger is invoked with the initial command line passed as a command line parameter.  The debugger is then responsible for invoking the target process with the flag DEBUG_ONLY_THIS_PROCESS or DEBUG_PROCESS (the debugger can also debug subprocesses). 

Be very careful with this, as it is possible to create complications on your system.  (The standard registry disclaimer that you can break your system applies too, but if you're reading this document you probably already know that :-) )

Note that there are two Image File Execution Options on 64-bit systems (a 32-bit one and a 64-bit one).  For sake of simplicity, let's assume the system, debugger, and debug target (debugee) are all the same architecture. 

To test this with 64-bit notepad, 64-bit debugger, and 64-bit OS, or 32-bit notepad, 32-bit debugger, and 32-bit system:
-Compile main.c to the same bitness as your system. (I'll assume the executable is called main.exe). 
-To make this easier, place it in the root of your System Drive (usually C:) 
-Create a registry key at "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVerison\Image File Execution Options\notepad.exe" (no quotes)
-Create a REG_SZ (String) value in that key called "Debugger".  Set the value to the full path of the debug executable (so "C:\main.exe" if you followed the first two steps).  This can be a relative path as long as the debugger executable is in the system's %PATH% variable.
-Launch notepad from system32 (so we get the architecture native verison).  In Task Manager, you should see both main.exe and notepad.exe.  If you close notepad.exe, main.exe should exit. (Closing the debugee emits a debug event, which is handled by exiting the loop, closing the handles the debugger opened, and exiting).

Remember to remove Debugger value when your done.

Of course, this code doesn't serve much of a purpose by itself, it is only a template.  


# COPYRIGHT
2016-2018 Rich A. Marino.  This code is in the public domain (and is intended to be a template for special purpose debuggers).

