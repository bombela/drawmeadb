/* $Id: defs.h,v 1.1 2008/01/24 00:10:05 erg Exp $ $Revision: 1.1 $ */
/* vim:set shiftwidth=4 ts=8: */

/**********************************************************
*      This software is part of the graphviz package      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2004 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
**********************************************************/

#ifndef _DEFS_H_
#define _DEFS_H_

typedef enum {regular, invisible} Style;

typedef struct {
	int nedges;
	int *edges;
	float *ewgts;
	Style *styles;
#if 0
	float *edists; // notice, this is a directed dist reflecting the direction of the edge
#endif
} vtx_data;

#endif
