/* $Id: libgraph.h,v 1.5 2007/09/07 12:05:44 ellson Exp $ $Revision: 1.5 $ */
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

#ifdef __cplusplus
extern "C" {
#endif


#ifndef _LIBGRAPH_H
#define _LIBGRAPH_H 1

#if _PACKAGE_ast
#include    <ast.h>
#else
#include <string.h>
#include <stdlib.h>
#ifndef MSWIN32
#include <unistd.h>
#endif
#endif
#include <ctype.h>


#ifndef EXTERN
#define EXTERN extern
#endif
#ifndef uchar
#define uchar	unsigned char
#endif

#ifndef NIL
#define NIL(t)	((t)0)
#endif

    void ag_yyerror(char *);
    int ag_yylex(void);

    typedef struct Agraphinfo_t {
	char notused;
    } Agraphinfo_t;
    typedef struct Agnodeinfo_t {
	char notused;
    } Agnodeinfo_t;
    typedef struct Agedgeinfo_t {
	char notused;
    } Agedgeinfo_t;

#ifndef _BLD_graph
#define _BLD_graph 1
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "graph.h"

#ifdef offsetof
#undef offsetof
#endif
#define offsetof(typ,fld)  ((int)(&(((typ*)0)->fld)))

#ifndef NOT
#define NOT(v)		(!(v))
#endif
#ifndef FALSE
#define	FALSE		0
#endif
#ifndef TRUE
#define TRUE		NOT(FALSE)
#endif
#define NEW(t)		 (t*)calloc(1,sizeof(t))
#define N_NEW(n,t)	 (t*)calloc((n),sizeof(t))
#define ALLOC(size,ptr,type) (ptr? (type*)realloc(ptr,(size)*sizeof(type)):(type*)malloc((size)*sizeof(type)))
#define MIN(a,b)	((a)<(b)?(a):(b))
#define MAX(a,b)	((a)>(b)?(a):(b))
#define SMALLBUF	128

#define ISALNUM(c) ((isalnum(c)) || ((c) == '_') || (!isascii(c)))

#define	NOPRINT		0
#define MULTIPLE    1
#define MUSTPRINT	2

#define ISEMPTYSTR(s) 	(((s) == NULL) || (*(s) == '\0'))
#define NULL_FN(t) 		(t(*)())0
#define ZFREE(p)		if (p) free(p);

#define	TAG_NODE			1
#define	TAG_EDGE			2
#define	TAG_GRAPH			3
#define TAG_OF(p)			(((Agraph_t*)(p))->tag)

#define AGFLAG_STRICT		(1<<1)
#define AGFLAG_METAGRAPH	(1<<2)
#define METAGRAPH			(AGFLAG_DIRECTED | AGFLAG_STRICT | AGFLAG_METAGRAPH)

#define KEY_ID				"key"
#define	KEYX				0
#define	TAILX				1
#define HEADX				2

    EXTERN struct AG_s {
	int graph_nbytes, node_nbytes, edge_nbytes;
	Agraph_t *proto_g, *parsed_g;
	char *edge_op;
	char *linebuf;
	short syntax_errors;
	uchar accepting_state, init_called;
    } AG;

/* follow structs used in graph parser */
    typedef struct objport_t {
	void *obj;
	char *port;
    } objport_t;

    typedef struct objlist_t {
	objport_t data;
	struct objlist_t *link;
    } objlist_t;

    typedef struct objstack_t {
	Agraph_t *subg;
	objlist_t *list, *last;
	int in_edge_stmt;
	struct objstack_t *link;
    } objstack_t;

    Agdict_t *agdictof(void *);
    Agnode_t *agidnode(Agraph_t *, int);
    Agdict_t *agNEWdict(char *);
    Agedge_t *agNEWedge(Agraph_t *, Agnode_t *, Agnode_t *, Agedge_t *);
    Agnode_t *agNEWnode(Agraph_t *, char *, Agnode_t *);
    Agsym_t *agNEWsym(Agdict_t *, char *, char *);
    int agcmpid(Dt_t *, int *, int *, Dtdisc_t *);
    int agcmpin(Dt_t *, Agedge_t *, Agedge_t *, Dtdisc_t *);
    int agcmpout(Dt_t *, Agedge_t *, Agedge_t *, Dtdisc_t *);
    void agcopydict(Agdict_t *, Agdict_t *);
    void agDELedge(Agraph_t *, Agedge_t *);
    void agDELnode(Agraph_t *, Agnode_t *);
    void agerror(char *);
    void agFREEdict(Agraph_t *, Agdict_t *);
    void agFREEedge(Agedge_t *);
    void agFREEnode(Agnode_t *);
    void aginitlib(int, int, int);
    void agINSedge(Agraph_t *, Agedge_t *);
    void agINSgraph(Agraph_t *, Agraph_t *);
    void agINSnode(Agraph_t *, Agnode_t *);
    int aglex(void);
    void aglexinit(FILE *, gets_f mygets);
    int agparse(void);
    void agpopproto(Agraph_t *);
    void agpushproto(Agraph_t *);
    char *agstrdup_html(char *s);
    int agtoken(char *);
    int aglinenumber(void);
    void agwredge(Agraph_t *, FILE *, Agedge_t *, int);
    void agwrnode(Agraph_t *, FILE *, Agnode_t *, int, int);
    extern Dtdisc_t agNamedisc, agNodedisc, agOutdisc, agIndisc,
	agEdgedisc;

#endif

#ifdef __cplusplus
}
#endif
