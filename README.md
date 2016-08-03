# Chip-8_VM
This project is a Chip 8 emulator written in C++. The original purpose of the project was to learn how to write an emulator. it can :
 read the game into a byte array
 emulate registers, program counters and stacks
 fetch an opcode from memory
 decode the next opcode and execute its instructions
 

#Make Sure...


 
 ●SDL.dll is in the same directory as the executable and depending on how you're running the exectuable you will need to either put game.ini and the ROMS directory in the same folder as the executable or in its parent folder. Make sure you edit game.ini to get the timing correct and to which Chip8 rom file to load.
 
●Inside the Project Properties:
 Add those lines in the Additional Dependencies 
 Legacy_stdio_definitions.lib;SDL.lib;SDLmain.lib;OpenGL32.Lib;
 
 
 

#Useful links to setup SDL in Visual Studo  2015:


 http://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/msvsnet2010u/index.php
 http://headerphile.com/sdl2/sdl2-part-0-setting-up-visual-studio-for-sdl2/
 
