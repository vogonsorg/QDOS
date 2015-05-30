#include "dk_essentials.h"

typedef struct
{
	char *gamename;
	char *seckey;
} game_table_t;

game_table_t gameTable[] =
{
	{"quake1", "7W7yZz"},
	{"quakeworld", "FU6Vqn"},
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
