/* FS: TODO Move this into sys_dos.c */

#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

#include "quakedef.h"
#include "dstring.h"

void VID_LockBuffer(void)
{
	/* FS: Nothing in DOS */
}
void VID_UnlockBuffer(void)
{
	/* FS: Nothing in DOS */
}


void Sys_DebugLog(const char *file, const char *fmt, ...)
{
	va_list argptr;
	static dstring_t *data;
	int fd;
    
	if(!data)
		data = dstring_new();

	va_start(argptr, fmt);
	dvsprintf(data, fmt, argptr);
	va_end(argptr);

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	write(fd, data->str, data->size - 1);
	close(fd);
}
