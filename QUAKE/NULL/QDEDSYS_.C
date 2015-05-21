// These are all over the place from SDL, but the 
// qded server goes APE 100% cpu if it's not with a real
// system driver....

#include "quakedef.h"

char *Sys_ConsoleInput (void)
{
	return NULL;
}

void Sys_SendKeyEvents (void)
{
}

int main(int argc, char*argv[])
{
int rc;
rc=	SDL_main(argc, argv);
return rc;
}
