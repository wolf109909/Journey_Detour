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

script : run Lua script code from console. | example: script ToggleDMActive(game)

queuelevel: load a map with map name. | example: queuelevel Level_Graveyard

netgui: toggles netgui. | example: netgui

(you can now press enter to do netgui execute.)

## Update:

Now redirects print() function in Lua to stdout. 

Which means now you can get some basic output from Lua to C.

Added proper error handling to Lua immediate script. now it returns the last value from top of the stack which usually contains Error message to stdout.


I will add more concommands as this project goes on.


