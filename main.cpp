#ifdef _WINDOWS
#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"
#else
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

#include <map>
#include <string>
#include <fstream>
#include "Chip8CPU.h"

typedef std::map<std::string, std::string> SETTINGS_MAP;

static const int windowWidth = 640;
static const int windowHeight = 320;

///////////////////////////////////////////////////////////////////

void LogMessage(const char* message)
{
#ifdef _WINDOWS
	MessageBox(NULL, message, "Error Occurred", MB_OK);
#else
	printf(message);
	printf("\n");
#endif 
}

///////////////////////////////////////////////////////////////////

void RenderFrame(Chip8CPU* cpu)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRasterPos2i(-1, 1);
	glPixelZoom(1, -1);
	glDrawPixels(windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, cpu->m_ScreenData);
	SDL_GL_SwapBuffers();
	glFlush();
}

///////////////////////////////////////////////////////////////////

void HandleInput(Chip8CPU* cpu, SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
	{
		int key = -1;
		switch (event.key.keysym.sym)
		{
		case SDLK_x: key = 0; break;
		case SDLK_1: key = 1; break;
		case SDLK_2: key = 2; break;
		case SDLK_3: key = 3; break;
		case SDLK_q: key = 4; break;
		case SDLK_w: key = 5; break;
		case SDLK_e: key = 6; break;
		case SDLK_a: key = 7; break;
		case SDLK_s: key = 8; break;
		case SDLK_d: key = 9; break;
		case SDLK_z: key = 10; break;
		case SDLK_c: key = 11; break;
		case SDLK_4: key = 12; break;
		case SDLK_r: key = 13; break;
		case SDLK_f: key = 14; break;
		case SDLK_v: key = 15; break;
		default: break;
		}
		if (key != -1)
		{
			cpu->KeyPressed(key);
		}
	}

	else if (event.type == SDL_KEYUP)
	{
		int key = -1;
		switch (event.key.keysym.sym)
		{
		case SDLK_x: key = 0; break;
		case SDLK_1: key = 1; break;
		case SDLK_2: key = 2; break;
		case SDLK_3: key = 3; break;
		case SDLK_q: key = 4; break;
		case SDLK_w: key = 5; break;
		case SDLK_e: key = 6; break;
		case SDLK_a: key = 7; break;
		case SDLK_s: key = 8; break;
		case SDLK_d: key = 9; break;
		case SDLK_z: key = 10; break;
		case SDLK_c: key = 11; break;
		case SDLK_4: key = 12; break;
		case SDLK_r: key = 13; break;
		case SDLK_f: key = 14; break;
		case SDLK_v: key = 15; break;
		default: break;
		}
		if (key != -1)
		{
			cpu->KeyReleased(key);
		}
	}
}

///////////////////////////////////////////////////////////////////

void DoEmulatorLoop(Chip8CPU* cpu, const SETTINGS_MAP& settings)
{
	SETTINGS_MAP::const_iterator it = settings.find("OpcodesPerSecond");

	// make sure the RomName setting is in the setting map
	if (settings.end() == it)
	{
		LogMessage("The OpcodesPerSecond setting cannot be found in game.ini");
		return;
	}

	int fps = 60;

	// number of opcodes to execute a second
	int numopcodes = atoi((*it).second.c_str());

	// number of opcodes to execute a frame 
	int numframe = numopcodes / fps;

	bool quit = false;
	SDL_Event event;
	float interval = 1000;
	interval /= fps;

	unsigned int time2 = SDL_GetTicks();

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			HandleInput(cpu, event);

			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		unsigned int current = SDL_GetTicks();

		if ((time2 + interval) < current)
		{
			cpu->DecreaseTimers();
			for (int i = 0; i < numframe; i++)
				cpu->ExecuteNextOpcode();

			time2 = current;
			RenderFrame(cpu);
		}
	}
}

///////////////////////////////////////////////////////////////////

// function attempts to load in settings from game.ini
// settings in game.ini are in the form:
// SettingName:SettingValue*Comment
// Example:
// RomName:Tetris*the name of the rom to emulate
// NO SPACES ALLOWED BEFORE THE * CHARACTER
bool LoadGameSettings(SETTINGS_MAP& settings)
{
	const int MAXLINE = 256;
	std::ifstream file;
	file.open("game.ini");

	if (false == file.is_open())
	{
		LogMessage("could not open settings file game.ini");
		return false;
	}

	char line[MAXLINE];
	std::string settingname;
	std::string settingvalue;

	while (false == file.eof())
	{
		memset(line, '\0', sizeof(line));
		file.getline(line, MAXLINE);

		//allow for comments and line breaks
		if ('*' == line[0] || '\r' == line[0] || '\n' == line[0] || '\0' == line[0])
			continue;

		// get the setting name
		char* name = 0;
		name = strtok(line, ":");
		settingname = name;

		// get the setting value
		char* value = 0;
		value = strtok(NULL, "*");
		settingvalue = value;

		// check for errors
		if (value == 0 || name == 0 || settingname.empty() || settingvalue.empty())
		{
			LogMessage("game.ini appears to be malformed");
			file.close();
			return false;
		}

		// add to settings map
		settings.insert(std::make_pair(settingname, settingvalue));
	}

	file.close();

	if (settings.empty())
	{
		LogMessage("no settings could be found in game.ini");
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////

void InitGL()
{
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, windowWidth, windowHeight, 0, -1.0, 1.0);
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_FLAT);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
}

///////////////////////////////////////////////////////////////////

bool CreateSDLWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}
	if (SDL_SetVideoMode(windowWidth, windowHeight, 8, SDL_OPENGL) == NULL)
	{
		return false;
	}

	InitGL();

	SDL_WM_SetCaption("CodeSlinger - Chip8 Emulator", NULL);
	return true;
}

///////////////////////////////////////////////////////////////////

bool LoadChip8Rom(Chip8CPU* cpu, const SETTINGS_MAP& settings)
{
	SETTINGS_MAP::const_iterator it = settings.find("RomName");

	// make sure the RomName setting is in the setting map
	if (settings.end() == it)
	{
		LogMessage("The RomName setting cannot be found in game.ini");
		return false;
	}

	// load the rom file into memory
	bool res = cpu->LoadRom((*it).second);

	return res;
}

///////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	SETTINGS_MAP settings;

	Chip8CPU* cpu = Chip8CPU::CreateSingleton();

	bool success = LoadGameSettings(settings);

	if (false == success)
	{
		LogMessage("error loading settings from game.ini... quitting");
		delete cpu;
		return 0;
	}

	success = CreateSDLWindow();

	if (false == success)
	{
		LogMessage("error creating sdl window... quitting");
		delete cpu;
		return 0;
	}

	success = LoadChip8Rom(cpu, settings);

	if (false == success)
	{
		LogMessage("error loading chip8 rom... file does not exist... quitting");
		SDL_Quit();
		delete cpu;
		return 0;
	}

	DoEmulatorLoop(cpu, settings);

	SDL_Quit();
	delete cpu;
	return 0;
}
