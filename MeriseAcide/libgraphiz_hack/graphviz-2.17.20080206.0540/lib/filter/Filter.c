/* $Id: Filter.c,v 1.1 2008/01/23 20:52:43 ellson Exp $ $Revision: 1.1 $ */
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

#include "filter.h"

int clear_filter(tv_filter* f)
{
	delete_node(f->root);
}

int init_filters(tv_filters* filters)
{
	filters->filter_count=0;
	filters->filters=0;
}
int add_filter_to_filters(tv_filters* filters,tv_filter* filter)
{
	filters->filters=realloc(filters->filters,sizeof(tv_filter*)*(filters->filter_count+1));
	filters->filters[filters->filter_count]=filter;
	filters->filter_count++;
}
int clear_filters(tv_filters* filters)
{
	//deep clear
	int ind=0;
	for (ind =0;ind < filters->filter_count ; ind ++)
	{
		clear_filter(filters->filters[ind]);
		free(filters->filters[ind]);
	}
	filters->filter_count=0;
}
int union_filter(tv_filter* f1,tv_filter* f2)
{
		
}
int intersect_filter(tv_filter* f1,tv_filter* f2);
