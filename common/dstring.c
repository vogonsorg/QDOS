/*
	dstring.c

	dynamic string buffer functions

	Copyright (C) 2002  Brian Koropoff.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

*/
#ifndef VISIBLE /* FS */
#define VISIBLE
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
//#include <libc/file.h> /* FS: Unneeded in DOS */

#include "sys.h"
#include "dstring.h"

//#include "compat.h" /* FS: Unneeded in DOS */

#ifdef _MSC_VER
	#define inline __inline
	#define vsnprintf _vsnprintf
#else
	int
	vsnprintf(char *str, size_t n, const char *fmt, va_list ap);
#endif

size_t
strnlen (const char *s, size_t maxlen) /* FS: Fix linkage errors */
{
	size_t i;
	for (i = 0; i < maxlen && s[i]; i++);
	return i;
}


static void *
dstring_alloc (void *data, size_t size)
{
	return calloc (1, size);
}

static void
dstring_free (void *data, void *ptr)
{
	free (ptr);
}


static void *
dstring_realloc (void *data, void *ptr, size_t size)
{
	return realloc (ptr, size);
}

VISIBLE dstring_mem_t dstring_default_mem = {
	dstring_alloc,
	dstring_free,
	dstring_realloc,
	0
};

inline dstring_t  *
_dstring_new (dstring_mem_t *mem)
{
	dstring_t  *new;

	new = mem->alloc (mem->data, sizeof (dstring_t));

	if (!new)
		Sys_Error ("dstring_new: Failed to allocate memory.");
	new->mem = mem;
	return new;
}

VISIBLE dstring_t *
dstring_new (void)
{
	return _dstring_new (&dstring_default_mem);
}

VISIBLE void
dstring_delete (dstring_t *dstr)
{
	if (dstr->str)
		dstr->mem->free (dstr->mem->data, dstr->str);
	dstr->mem->free (dstr->mem->data, dstr);
}

VISIBLE inline void
dstring_adjust (dstring_t *dstr)
{
	if (dstr->size > dstr->truesize) {
		dstr->truesize = (dstr->size + 1023) & ~1023;
		dstr->str = dstr->mem->realloc (dstr->mem->data, dstr->str,
										dstr->truesize);
		if (!dstr->str)
			Sys_Error ("dstring_adjust:  Failed to reallocate memory.");
	}
}

VISIBLE char *
dstring_reserve (dstring_t *dstr, unsigned len)
{
	dstr->size += len;
	dstring_adjust (dstr);
	return dstr->str + dstr->size - len;
}

VISIBLE void
dstring_copy (dstring_t *dstr, const char *data, unsigned int len)
{
	dstr->size = len;
	dstring_adjust (dstr);
	memcpy (dstr->str, data, len);
}

VISIBLE void
dstring_append (dstring_t *dstr, const char *data, unsigned int len)
{
	unsigned int ins = dstr->size;		// Save insertion point

	dstr->size += len;
	dstring_adjust (dstr);
	memcpy (dstr->str + ins, data, len);
}

VISIBLE void
dstring_insert (dstring_t *dstr, unsigned int pos, const char *data,
				unsigned int len)
{
	unsigned int oldsize = dstr->size;

	if (pos > dstr->size)
		pos = dstr->size;
	dstr->size += len;
	dstring_adjust (dstr);
	memmove (dstr->str + pos + len, dstr->str + pos, oldsize - pos);
	memcpy (dstr->str + pos, data, len);
}

VISIBLE void
dstring_snip (dstring_t *dstr, unsigned int pos, unsigned int len)
{
	if (pos > dstr->size)
		pos = dstr->size;
	if (pos + len > dstr->size)
		len = dstr->size - pos;
	if (!len)
		return;
	memmove (dstr->str + pos, dstr->str + pos + len, dstr->size - pos - len);
	dstr->size -= len;
	dstring_adjust (dstr);
}

VISIBLE void
dstring_clear (dstring_t *dstr)
{
	dstr->size = 0;
	dstring_adjust (dstr);
}

VISIBLE void
dstring_replace (dstring_t *dstr, unsigned int pos, unsigned int rlen,
				const char *data, unsigned int len)
{
	unsigned int oldsize = dstr->size;
	if (pos > dstr->size)
		pos = dstr->size;
	if (rlen > dstr->size - pos)
		rlen = dstr->size - pos;
	if (rlen < len) {
		dstr->size += len - rlen;
		dstring_adjust (dstr);
		memmove (dstr->str + pos + len, dstr->str + pos + rlen,
				 oldsize - (pos + rlen));
	} else if (rlen > len) {
		memmove (dstr->str + pos + len, dstr->str + pos + rlen,
				 oldsize - (pos + rlen));
		dstr->size -= rlen - len;
		dstring_adjust (dstr);
	}
	memcpy (dstr->str + pos, data, len);
}

VISIBLE char *
dstring_freeze (dstring_t *dstr)
{
	char *str = dstr->mem->realloc (dstr->mem->data, dstr->str, dstr->size);
	dstr->mem->free (dstr->mem->data, dstr);
	return str;
}

VISIBLE inline dstring_t  *
_dstring_newstr (dstring_mem_t *mem)
{
	dstring_t  *new;

	new = mem->alloc (mem->data, sizeof (dstring_t));

	if (!new)
		Sys_Error ("dstring_newstr:  Failed to allocate memory.");
	new->mem = mem;
	new->size = 1;
	dstring_adjust (new);
	new->str[0] = 0;
	return new;
}

VISIBLE dstring_t *
dstring_newstr (void)
{
	return _dstring_newstr (&dstring_default_mem);
}

VISIBLE dstring_t *
dstring_strdup (const char *str)
{
	dstring_t  *dstr = dstring_new ();
	dstring_copystr (dstr, str);
	return dstr;
}

VISIBLE char *
dstring_reservestr (dstring_t *dstr, unsigned len)
{
	int pos = dstr->size;
	if (pos && !dstr->str[pos - 1])
		pos--;
	dstr->size = pos + len + 1;
	dstring_adjust (dstr);
	return dstr->str + pos;
}

VISIBLE void
dstring_copystr (dstring_t *dstr, const char *str)
{
	dstr->size = strlen (str) + 1;
	dstring_adjust (dstr);
	strcpy (dstr->str, str);
}

VISIBLE void
dstring_copysubstr (dstring_t *dstr, const char *str, unsigned int len)
{
	len = strnlen (str, len);

	dstr->size = len + 1;
	dstring_adjust (dstr);
	strncpy (dstr->str, str, len);
	dstr->str[len] = 0;
}

VISIBLE void
dstring_appendstr (dstring_t *dstr, const char *str)
{
	unsigned int pos = strnlen (dstr->str, dstr->size);
	unsigned int len = strlen (str);

	dstr->size = pos + len + 1;
	dstring_adjust (dstr);
	strcpy (dstr->str + pos, str);
}

VISIBLE void
dstring_appendsubstr (dstring_t *dstr, const char *str, unsigned int len)
{
	unsigned int pos = strnlen (dstr->str, dstr->size);

	len = strnlen (str, len);
	dstr->size = pos + len + 1;
	dstring_adjust (dstr);
	strncpy (dstr->str + pos, str, len);
	dstr->str[pos + len] = 0;
}

VISIBLE void
dstring_insertstr (dstring_t *dstr, unsigned int pos, const char *str)
{
	// Don't insert strlen + 1 to achieve concatenation
	dstring_insert (dstr, pos, str, strlen (str));
}

VISIBLE void
dstring_insertsubstr (dstring_t *dstr, unsigned int pos, const char *str,
					  unsigned int len)
{
	len = strnlen (str, len);

	dstring_insert (dstr, pos, str, len);
}

VISIBLE void
dstring_clearstr (dstring_t *dstr)
{
	dstr->size = 1;
	dstring_adjust (dstr);
	dstr->str[0] = 0;
}

#if defined (HAVE_VA_COPY)
# define VA_COPY(a,b) va_copy (a, b)
#elif defined (HAVE__VA_COPY)
# define VA_COPY(a,b) __va_copy (a, b)
#else
# define VA_COPY(a,b) memcpy (a, b, sizeof (a))
#endif

static int
_dvsprintf (dstring_t *dstr, int offs, const char *fmt, va_list args)
{
	int size;

#ifdef VA_LIST_IS_ARRAY
	va_list tmp_args;
	VA_COPY (tmp_args, args);
#endif

	if (!dstr->truesize)
		dstring_clearstr (dstr); // Make it a string
	// Some vsnprintf implementations return -1 on truncation
	while ((size = vsnprintf (dstr->str + offs, dstr->truesize - offs, fmt,
							  args)) == -1) {
		dstr->size = (dstr->truesize & ~1023) + 1024;
		dstring_adjust (dstr);
#ifdef VA_LIST_IS_ARRAY
		VA_COPY (args, tmp_args);
#endif
	}
	dstr->size = size + offs + 1;
	// "Proper" implementations return the required size 
	if (dstr->size > dstr->truesize) {
		dstring_adjust (dstr);
#ifdef VA_LIST_IS_ARRAY
		VA_COPY (args, tmp_args);
#endif
		vsnprintf (dstr->str + offs, dstr->truesize - offs, fmt, args);
	}
	return size;
}

VISIBLE int
dvsprintf (dstring_t *dstr, const char *fmt, va_list args)
{
	return _dvsprintf (dstr, 0, fmt, args);
}

VISIBLE int
dsprintf (dstring_t *dstr, const char *fmt, ...)
{
	va_list args;
	int ret;

	va_start (args, fmt);
	ret = _dvsprintf (dstr, 0, fmt, args);
	va_end (args);

	return ret;
}

VISIBLE int
davsprintf (dstring_t *dstr, const char *fmt, va_list args)
{
	int offs = 0;

	if (dstr->size)
		offs = dstr->size - 1;
	return _dvsprintf (dstr, offs, fmt, args);
}

VISIBLE int
dasprintf (dstring_t *dstr, const char *fmt, ...)
{
	va_list args;
	int ret;
	int offs = 0;

	if (dstr->size)
		offs = dstr->size - 1;
	va_start (args, fmt);
	ret = _dvsprintf (dstr, offs, fmt, args);
	va_end (args);

	return ret;
}
