#include "dk_essentials.h"

typedef struct
{
	char *gamename;
	char *seckey;
} game_table_t;

game_table_t gameTable[] =
{
	{"blood2", "jUOF0p"},
	{"daikatana", "fl8aY7"},
	{"gspylite", "mgNUaC"},
	{"hexenworld", "6SeXQB"},
	{"kingpin", "QFWxY2"},
	{"nolf", "Jn3Ab4"},
	{"quake1", "7W7yZz"},
	{"quake2", "rtW0xg"},
	{"quakeworld", "FU6Vqn"},
	{"shogo", "MQMhRK"}, // FS: Untested, but assumed to work
	{"sin", "Ij1uAB"},
	{"southpark", "yoI7mE"},
	{"turok2", "RWd3BG"},
	{"unreal", "DAncRK"},
	{"ut", "Z5Nfb0"}, // FS: Unreal Tournament 99
	{NULL, NULL}
};

char *Gamespy_Get_Game_SecKey (char *gamename)
{
	int x = 0;
	
	if (!gamename || gamename[0] == 0)
		return NULL;

	gamename = DK_strlwr(gamename); // FS: Some games (mainly sin) stupidly send it partially uppercase

	while (gameTable[x].gamename != NULL)
	{
		if(!strcmp(gamename, gameTable[x].gamename))
		{
			return gameTable[x].seckey;
		}
		x++;
	}
	return NULL;
}
