/* $Id: gvcext.h,v 1.7 2007/05/30 22:28:27 erg Exp $ $Revision: 1.7 $ */
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

/* Common header used by both clients and plugins */

#ifndef GVCEXT_H
#define GVCEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WITH_CODEGENS
    typedef struct codegen_s codegen_t;
    typedef struct codegen_info_s codegen_info_t;
#endif

    typedef struct GVJ_s GVJ_t;
    typedef struct GVC_s GVC_t;

    typedef struct {
	const char *name;
	void* address;
    } lt_symlist_t;


#if defined(GVDLL) && !defined(ENABLE_LTDL)
    extern lt_symlist_t lt_preloaded_symbols[];
#else
    extern const lt_symlist_t lt_preloaded_symbols[];
#endif

#ifdef __cplusplus
}
#endif

#endif
