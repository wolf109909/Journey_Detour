# Journey_Detour
## This project is a WIP.

Currently working functions:

Console I/O.

Some useful concommands.

Running lua code from within the console.

Hotkeys(need to define in code).

## How to use:

Download the release dll or compile your own 

Inject the dll into journey.exe with any working injector.

Such as: CheatEngine , Extreme Injector.

## console commands:

1. meminfo(F3)     : Toggle memory info prints.              
2. debug(F1)       : Run debugger function.                  
3. debug <filename>: Debug target lua file.                  
4. debughud(F8)    : Cycle through DebugHud.                 
5. script <luacode>: Force Execute lua code to VM.(May crash)
6. console(F7)     : Toggle console redirect                 
7. help            : Print this screen again                 

## Update:

Now redirects print() function in Lua to stdout. 

Which means now you can get some basic output from Lua to C.

Added proper error handling to Lua immediate script. now it returns the last value from top of the stack which usually contains Error message to stdout.


I will add more concommands as this project goes on.


