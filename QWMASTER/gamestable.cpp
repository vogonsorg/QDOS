#include "dk_essentials.h"

typedef struct
{
	const char *gamename;
	const char *seckey;
} game_table_t;

game_table_t gameTable[] =
{
	{"quake1", "7W7yZz"},
	{"quake2", "rtW0xg"},
	{"quakeworld", "FU6Vqn"},
	{NULL, NULL}
};

const char *Gamespy_Get_Game_SecKey (char *gamename)
{
	int x = 0;

	if (!gamename || gamename[0] == 0)
		return NULL;

	DK_strlwr(gamename); // FS: Some games (mainly sin) stupidly send it partially uppercase

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
