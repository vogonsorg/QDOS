#include "dk_essentials.h"

typedef struct
{
	const char *gamename;
	const char *seckey;
	int	motdPort;
} game_table_t;

game_table_t gameTable[] =
{
	{"quake1", "7W7yZz", 0},
	{"quake2", "rtW0xg", 27901},
	{"quakeworld", "FU6Vqn", 27501},
	{NULL, NULL}
};

const char *Gamespy_Get_Game_SecKey (char *gamename)
{
	int x = 0;

	if (!gamename || gamename[0] == 0)
	{
		return NULL;
	}

	DK_strlwr(gamename); /* FS: Some games (mainly sin) stupidly send it partially uppercase */

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

int Gamespy_Get_MOTD_Port (char *gamename)
{
	int x = 0;

	if (!gamename || gamename[0] == 0)
	{
		return 0;
	}

	DK_strlwr(gamename); /* FS: Some games (mainly sin) stupidly send it partially uppercase */

	while (gameTable[x].gamename != NULL)
	{
		if(!strcmp(gamename, gameTable[x].gamename))
		{
			return gameTable[x].motdPort;
		}

		x++;
	}
	return 0;
}
