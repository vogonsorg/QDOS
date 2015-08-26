/*
 * THIS SOFTWARE IS SUBJECT TO COPYRIGHT PROTECTION AND IS OFFERED ONLY
 * PURSUANT TO THE 3DFX GLIDE GENERAL PUBLIC LICENSE. THERE IS NO RIGHT
 * TO USE THE GLIDE TRADEMARK WITHOUT PRIOR WRITTEN PERMISSION OF 3DFX
 * INTERACTIVE, INC. A COPY OF THIS LICENSE MAY BE OBTAINED FROM THE 
 * DISTRIBUTOR OR BY CONTACTING 3DFX INTERACTIVE INC(info@3dfx.com). 
 * THIS PROGRAM IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESSED OR IMPLIED. SEE THE 3DFX GLIDE GENERAL PUBLIC LICENSE FOR A
 * FULL TEXT OF THE NON-WARRANTY PROVISIONS.  
 * 
 * USE, DUPLICATION OR DISCLOSURE BY THE GOVERNMENT IS SUBJECT TO
 * RESTRICTIONS AS SET FORTH IN SUBDIVISION (C)(1)(II) OF THE RIGHTS IN
 * TECHNICAL DATA AND COMPUTER SOFTWARE CLAUSE AT DFARS 252.227-7013,
 * AND/OR IN SIMILAR OR SUCCESSOR CLAUSES IN THE FAR, DOD OR NASA FAR
 * SUPPLEMENT. UNPUBLISHED RIGHTS RESERVED UNDER THE COPYRIGHT LAWS OF
 * THE UNITED STATES.  
 * 
 * COPYRIGHT 3DFX INTERACTIVE, INC. 1999, ALL RIGHTS RESERVED
 *
 * $Header: /cvsroot/glide/glide3x/cvg/glide3/src/fxbldno.c,v 1.1.1.1 1999/12/07 21:42:31 joseph Exp $
 * $Log: fxbldno.c,v $
 * Revision 1.1.1.1  1999/12/07 21:42:31  joseph
 * Initial checkin into SourceForge.
 *
** 
** 1     10/08/98 11:30a Brent
** 
** 2     7/24/98 1:41p Hohn
 * 
 * 1     1/16/98 4:29p Atai
 * create glide 3 src
 * 
 * 1     7/25/97 9:05a Pgj
 * generate fxbldno.h which defines BUILD_NUMBER
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

main(int argc, char **argv)
{
    struct tm	locTime;
    time_t	sysTime;
    char	*build;

    time(&sysTime);
    locTime = *localtime(&sysTime);

    if (build = getenv("BUILD_NUMBER")) {
	printf("#define BUILD_NUMBER	%s\n", build);
    } else {
	unsigned short magic;
	magic = (locTime.tm_yday << 7) |
	        (locTime.tm_hour << 2) |
		(locTime.tm_min / 15);
	printf("#define BUILD_NUMBER	%d\n", magic);
    }
    return 0;

} /* end main() */
