/* $Id: gvrender.c,v 1.142 2007/12/04 21:22:17 erg Exp $ $Revision: 1.142 $ */
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

/*
 *  graphics code generator wrapper
 *
 *  This library forms the socket for run-time loadable render plugins.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include "memory.h"
#include "const.h"
#include "macros.h"
#include "colorprocs.h"
#include "gvplugin_render.h"
#include "graph.h"
#include "gvcint.h"
#include "geom.h"
#include "geomprocs.h"
#include "gvcproc.h"

extern int emit_once(char *str);
extern shape_desc *find_user_shape(char *name);
extern boolean mapbool(char *s);

/* storage for temporary hacks until client API is FP */
static pointf *AF;
static int sizeAF;
#ifdef WITH_CODEGENS
static point *A;
static int sizeA;
#endif
/* end hack */

int gvrender_select(GVJ_t * job, char *str)
{
    GVC_t *gvc = job->gvc;
    gvplugin_available_t *plugin;
    gvplugin_installed_t *typeptr;
#ifdef WITH_CODEGENS
    codegen_info_t *cg_info;
#endif

    gvplugin_load(gvc, API_device, str);

    job->flags = 0;
    plugin = gvc->api[API_device];
    if (plugin) {
#ifdef WITH_CODEGENS
	if (strcmp(plugin->packagename, "cg") == 0) {
	    cg_info = (codegen_info_t *) (plugin->typeptr);
	    job->codegen = cg_info->cg;
	    job->render.engine = NULL;
	    return cg_info->id;
	}
#endif
        typeptr = plugin->typeptr;
        job->device.engine = (gvdevice_engine_t *) (typeptr->engine);
        job->device.features = (gvdevice_features_t *) (typeptr->features);
        job->device.id = typeptr->id;
        job->device.type = plugin->typestr;

        job->flags |= job->device.features->flags;
    }
    else
	return NO_SUPPORT;  /* FIXME - should differentiate problem */
    
    /* The device plugin has a dependency on a render plugin,
     * so the render plugin should be available as well now */
    plugin = gvc->api[API_render];
    if (plugin) {
        typeptr = plugin->typeptr;
        job->render.engine = (gvrender_engine_t *) (typeptr->engine);
        job->render.features = (gvrender_features_t *) (typeptr->features);
        job->render.type = plugin->typestr;

        job->flags |= job->render.features->flags;

        if (job->device.engine)
            job->render.id = typeptr->id;
	else
	    /* A null device engine indicates that the device id is also the renderer id
	     * and that the renderer doesn't need "device" functions.
	     * Device "features" settings are still available */
            job->render.id = job->device.id;
        return GVRENDER_PLUGIN;
    }
    job->render.engine = NULL;
    return NO_SUPPORT;  /* FIXME - should differentiate problem */
}

int gvrender_features(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;
    int features = 0;

    if (gvre) {
	features = job->render.features->flags;
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg) {
	    if (cg->bezier_has_arrows)
		features |= GVRENDER_DOES_ARROWS;
	    if (cg->begin_layer)
		features |= GVDEVICE_DOES_LAYERS;
	}
    }
#endif
    return features;
}

void gvrender_begin_job(GVJ_t * job)
{
    GVC_t *gvc = job->gvc;
    gvrender_engine_t *gvre = job->render.engine;

    gvdevice_initialize(job);
    if (gvre) {
        if (gvre->begin_job)
	    gvre->begin_job(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->begin_job)
	    cg->begin_job(job->output_file, gvc->g,
		    gvc->common.lib,
		    gvc->common.user,
		    gvc->common.info,
		    job->pagesArraySize);
    }
#endif
}

void gvrender_end_job(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
       	if (gvre->end_job)
	    gvre->end_job(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->end_job)
	    cg->end_job();
    }
#endif
    job->gvc->common.lib = NULL;    /* FIXME - minimally this doesn't belong here */
    gvdevice_finalize(job);
}

/* font modifiers */
#define REGULAR 0
#define BOLD    1
#define ITALIC  2

pointf gvrender_ptf(GVJ_t *job, pointf p)
{
    pointf rv, translation, scale;

    translation = job->translation;
    scale.x = job->zoom * job->devscale.x;
    scale.y = job->zoom * job->devscale.y;

    if (job->rotation) {
	rv.x =  -(p.y + translation.y) * scale.x;
	rv.y =  (p.x + translation.x) * scale.y;
    } else {
	rv.x =  (p.x + translation.x) * scale.x;
	rv.y =  (p.y + translation.y) * scale.y;
    }
    return rv;
}

/* transform an array of n points */
/*  *AF and *af must be preallocated */
/*  *AF can be the same as *af for inplace transforms */
pointf* gvrender_ptf_A(GVJ_t *job, pointf *af, pointf *AF, int n)
{
    int i;
    double t;
    pointf translation, scale;

    translation = job->translation;
    scale.x = job->zoom * job->devscale.x;
    scale.y = job->zoom * job->devscale.y;

    if (job->rotation) {
        for (i = 0; i < n; i++) {
	          t = -(af[i].y + translation.y) * scale.x;
	    AF[i].y =  (af[i].x + translation.x) * scale.y;
	    AF[i].x = t;
	}
    }
    else {
        for (i = 0; i < n; i++) {
	    AF[i].x =  (af[i].x + translation.x) * scale.x;
	    AF[i].y =  (af[i].y + translation.y) * scale.y;
	}
    }
    return AF;
}

static int gvrender_comparestr(const void *s1, const void *s2)
{
    return strcmp(*(char **) s1, *(char **) s2);
}

static void gvrender_resolve_color(gvrender_features_t * features,
				   char *name, gvcolor_t * color)
{
    char *tok;
    int rc;

    color->u.string = name;
    color->type = COLOR_STRING;
    tok = canontoken(name);
    if (!features->knowncolors || (bsearch(&tok, features->knowncolors, features->sz_knowncolors,
		 sizeof(char *), gvrender_comparestr)) == NULL) {
	/* if tok was not found in known_colors */
	rc = colorxlate(name, color, features->color_type);
	if (rc != COLOR_OK) {
	    if (rc == COLOR_UNKNOWN) {
		char *missedcolor = gmalloc(strlen(name) + 16);
		sprintf(missedcolor, "color %s", name);
		if (emit_once(missedcolor))
		    agerr(AGWARN, "%s is not a known color.\n", name);
		free(missedcolor);
	    }
	    else {
		agerr(AGERR, "error in colxlate()\n");
	    }
	}
    }
}

void gvrender_begin_graph(GVJ_t * job, graph_t * g)
{
    GVC_t *gvc = job->gvc;
    gvrender_engine_t *gvre = job->render.engine;
    char *s;

    if (gvre) {
	/* render specific init */
	if (gvre->begin_graph)
	    gvre->begin_graph(job);

	/* background color */
	if (((s = agget(g, "bgcolor")) != 0) && s[0]) {
	    gvrender_resolve_color(job->render.features, s, &(gvc->bgcolor));
	    if (gvre->resolve_color)
		gvre->resolve_color(job, &(gvc->bgcolor));
	}

    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;
	box canvasBox;

	BF2B(job->canvasBox, canvasBox);

	if (cg && cg->begin_graph)
	    cg->begin_graph(gvc, g, canvasBox, gvc->pb);
    }
#endif
}

void gvrender_end_graph(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
       	if (gvre->end_graph)
	    gvre->end_graph(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->end_graph)
	    cg->end_graph();
    }
#endif
    gvdevice_format(job);
}

void gvrender_begin_page(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
        if (gvre->begin_page)
	    gvre->begin_page(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;
        point offset;

        PF2P(job->pageOffset, offset);
        if (cg && cg->begin_page)
            cg->begin_page(job->gvc->g, job->pagesArrayElem,
               job->zoom, job->rotation, offset);
    }
#endif
}

void gvrender_end_page(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
       	if (gvre->end_page)
	    gvre->end_page(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->end_page)
	    cg->end_page();
    }
#endif
}

void gvrender_begin_layer(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->begin_layer)
	    gvre->begin_layer(job, job->gvc->layerIDs[job->layerNum], job->layerNum, job->numLayers);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->begin_layer)
	    cg->begin_layer(job->gvc->layerIDs[job->layerNum], job->layerNum, job->numLayers);
    }
#endif
}

void gvrender_end_layer(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
        if (gvre->end_layer)
	    gvre->end_layer(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->end_layer)
	    cg->end_layer();
    }
#endif
}

void gvrender_begin_cluster(GVJ_t * job, graph_t * sg)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->begin_cluster)
	    gvre->begin_cluster(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->begin_cluster)
	    cg->begin_cluster(sg);
    }
#endif
}

void gvrender_end_cluster(GVJ_t * job, graph_t *g)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
       	if (gvre->end_cluster)
	    gvre->end_cluster(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->end_cluster)
	    cg->end_cluster();
    }
#endif
}

void gvrender_begin_nodes(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->begin_nodes)
	    gvre->begin_nodes(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->begin_nodes)
	    cg->begin_nodes();
    }
#endif
}

void gvrender_end_nodes(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->end_nodes)
	    gvre->end_nodes(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->end_nodes)
	    cg->end_nodes();
    }
#endif
}

void gvrender_begin_edges(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->begin_edges)
	    gvre->begin_edges(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->begin_edges)
	    cg->begin_edges();
    }
#endif
}

void gvrender_end_edges(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->end_edges)
	    gvre->end_edges(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->end_edges)
	    cg->end_edges();
    }
#endif
}

void gvrender_begin_node(GVJ_t * job, node_t * n)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->begin_node)
	    gvre->begin_node(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->begin_node)
	    cg->begin_node(n);
    }
#endif
}

void gvrender_end_node(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->end_node)
	    gvre->end_node(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->end_node)
	    cg->end_node();
    }
#endif
}

void gvrender_begin_edge(GVJ_t * job, edge_t * e)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->begin_edge)
	    gvre->begin_edge(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->begin_edge)
	    cg->begin_edge(e);
    }
#endif
}

void gvrender_end_edge(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->end_edge)
	    gvre->end_edge(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->end_edge)
	    cg->end_edge();
    }
#endif
}

void gvrender_begin_context(GVJ_t * job)
{
#ifdef WITH_CODEGENS
    gvrender_engine_t *gvre = job->render.engine;
    codegen_t *cg = job->codegen;

    if (!gvre && cg && cg->begin_context)
	cg->begin_context();
#endif
}

void gvrender_end_context(GVJ_t * job)
{
#ifdef WITH_CODEGENS
    gvrender_engine_t *gvre = job->render.engine;
    codegen_t *cg = job->codegen;

    if (!gvre && cg && cg->end_context)
	cg->end_context();
#endif
}

void gvrender_begin_anchor(GVJ_t * job, char *href, char *tooltip, char *target)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->begin_anchor)
	    gvre->begin_anchor(job, href, tooltip, target);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->begin_anchor)
	    cg->begin_anchor(href, tooltip, target);
    }
#endif
}

void gvrender_end_anchor(GVJ_t * job)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->end_anchor)
	    gvre->end_anchor(job);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->end_anchor)
	    cg->end_anchor();
    }
#endif
}

void gvrender_set_font(GVJ_t * job, char *fontname, double fontsize)
{
    gvrender_engine_t *gvre = job->render.engine;

#ifdef WITH_CODEGENS
    if (!gvre) {
	codegen_t *cg = job->codegen;

	if (cg && cg->set_font)
	    cg->set_font(fontname, fontsize);
    }
#endif
}

void gvrender_textpara(GVJ_t * job, pointf p, textpara_t * para)
{
    gvrender_engine_t *gvre = job->render.engine;
    pointf PF;

    if (para->str && para->str[0]
	    && ( ! job->obj  /* because of xdgen non-conformity */
		|| job->obj->pen != PEN_NONE)) {
	if (job->flags & GVRENDER_DOES_TRANSFORM)
	    PF = p;
	else
	    PF = gvrender_ptf(job, p);
	if (gvre) {
	    if (gvre->textpara)
		gvre->textpara(job, PF, para);
	}
#ifdef WITH_CODEGENS
	else {
	    codegen_t *cg = job->codegen;
	    point P;

	    PF2P(p, P);
	    if (cg && cg->textpara)
		cg->textpara(P, para);
	}
#endif
    }
}

void gvrender_set_pencolor(GVJ_t * job, char *name)
{
    gvrender_engine_t *gvre = job->render.engine;
    gvcolor_t *color = &(job->obj->pencolor);

    if (gvre) {
	gvrender_resolve_color(job->render.features, name, color);
	if (gvre->resolve_color)
	    gvre->resolve_color(job, color);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->set_pencolor)
	    cg->set_pencolor(name);
    }
#endif
}

void gvrender_set_fillcolor(GVJ_t * job, char *name)
{
    gvrender_engine_t *gvre = job->render.engine;
    gvcolor_t *color = &(job->obj->fillcolor);

    if (gvre) {
	gvrender_resolve_color(job->render.features, name, color);
	if (gvre->resolve_color)
	    gvre->resolve_color(job, color);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->set_fillcolor)
	    cg->set_fillcolor(name);
    }
#endif
}

void gvrender_set_style(GVJ_t * job, char **s)
{
    gvrender_engine_t *gvre = job->render.engine;
    obj_state_t *obj = job->obj;
    char *line, *p;

    obj->rawstyle = s;
    if (gvre) {
	if (s) while ((p = line = *s++)) {
	    if (streq(line, "solid"))
		obj->pen = PEN_SOLID;
	    else if (streq(line, "dashed"))
		obj->pen = PEN_DASHED;
	    else if (streq(line, "dotted"))
		obj->pen = PEN_DOTTED;
	    else if (streq(line, "invis") || streq(line, "invisible"))
		obj->pen = PEN_NONE;
	    else if (streq(line, "bold"))
		obj->penwidth = PENWIDTH_BOLD;
	    else if (streq(line, "setlinewidth")) {
		while (*p)
		    p++;
		p++;
		obj->penwidth = atof(p);
	    } else if (streq(line, "filled"))
		obj->fill = FILL_SOLID;
	    else if (streq(line, "unfilled"))
		obj->fill = FILL_NONE;
	    else {
		agerr(AGWARN,
		      "gvrender_set_style: unsupported style %s - ignoring\n",
		      line);
	    }
	}
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->set_style)
	    cg->set_style(s);
    }
#endif
}

void gvrender_ellipse(GVJ_t * job, pointf * pf, int n, boolean filled)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->ellipse && job->obj->pen != PEN_NONE) {
	    pointf af[2];

	    /* center */
	    af[0].x = (pf[0].x + pf[1].x)/2.;
	    af[0].y = (pf[0].y + pf[1].y)/2.;
	    /* corner */
	    af[1] = pf[1];

	    if (! (job->flags & GVRENDER_DOES_TRANSFORM))
	        gvrender_ptf_A(job, af, af, 2);
	    gvre->ellipse(job, af, filled);
	}
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->ellipse) {
	    pointf af[2];
	    point p, r;

	    /* center */
	    af[0].x = (pf[0].x + pf[1].x)/2.;
	    af[0].y = (pf[0].y + pf[1].y)/2.;
	    /* radius */
	    af[1].x = pf[1].x - af[0].x;
	    af[1].y = pf[1].y - af[0].y;

	    PF2P(af[0], p);
	    PF2P(af[1], r);
	    cg->ellipse(p, r.x, r.y, filled);
	}
    }
#endif
}

void gvrender_polygon(GVJ_t * job, pointf * af, int n, boolean filled)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->polygon && job->obj->pen != PEN_NONE) {
	    if (job->flags & GVRENDER_DOES_TRANSFORM)
		gvre->polygon(job, af, n, filled);
	    else {
	        if (sizeAF < n) {
		    sizeAF = n+10;
		    AF = grealloc(AF, sizeAF * sizeof(pointf));
	        }
	        gvrender_ptf_A(job, af, AF, n);
	        gvre->polygon(job, AF, n, filled);
	    }
	}
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;
        int i;

	if (sizeA < n) {
	    sizeA = n+10;
	    A = grealloc(A, sizeA * sizeof(point));
	}
	for (i = 0; i < n; i++)
	    PF2P(af[i], A[i]);
	if (cg && cg->polygon)
	    cg->polygon(A, n, filled);
    }
#endif
}

void gvrender_box(GVJ_t * job, boxf B, boolean filled)
{
    pointf A[4];

    A[0] = B.LL;
    A[2] = B.UR;
    A[1].x = A[0].x;
    A[1].y = A[2].y;
    A[3].x = A[2].x;
    A[3].y = A[0].y;
    
    gvrender_polygon(job, A, 4, filled);
}

void gvrender_beziercurve(GVJ_t * job, pointf * af, int n,
			  int arrow_at_start, int arrow_at_end, boolean filled)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->beziercurve && job->obj->pen != PEN_NONE) {
	    if (job->flags & GVRENDER_DOES_TRANSFORM)
		gvre->beziercurve(job, af, n, arrow_at_start, arrow_at_end,filled);
	    else {
	        if (sizeAF < n) {
		    sizeAF = n+10;
		    AF = grealloc(AF, sizeAF * sizeof(pointf));
	        }
	        gvrender_ptf_A(job, af, AF, n);
	        gvre->beziercurve(job, AF, n, arrow_at_start, arrow_at_end,filled);
	    }
	}
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;
	int i;
	if (sizeA < n) {
	    sizeA = n+10;
	    A = grealloc(A, sizeA * sizeof(point));
	}
	for (i = 0; i < n; i++) {
	    PF2P(af[i], A[i]);
	}
	if (cg && cg->beziercurve)
	    cg->beziercurve(A, n, arrow_at_start, arrow_at_end, filled);
    }
#endif
}

void gvrender_polyline(GVJ_t * job, pointf * af, int n)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
	if (gvre->polyline && job->obj->pen != PEN_NONE) {
	    if (job->flags & GVRENDER_DOES_TRANSFORM)
	        gvre->polyline(job, af, n);
            else {
	        if (sizeAF < n) {
		    sizeAF = n+10;
		    AF = grealloc(AF, sizeAF * sizeof(pointf));
	        }
	        gvrender_ptf_A(job, af, AF, n);
	        gvre->polyline(job, AF, n);
	    }
	}
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;
        int i;

	if (sizeA < n) {
	    sizeA = n+10;
	    A = grealloc(A, sizeA * sizeof(point));
	}
	for (i = 0; i < n; i++)
	    PF2P(af[i], A[i]);
	if (cg && cg->polyline)
	    cg->polyline(A, n);
    }
#endif
}

void gvrender_comment(GVJ_t * job, char *str)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (!str || !str[0])
	return;

    if (gvre) {
	if (gvre->comment)
	    gvre->comment(job, str);
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->comment)
	    cg->comment(str);
    }
#endif
}

static imagescale_t get_imagescale(char *s)
{
        if (*s == '\0') return IMAGESCALE_FALSE;
        if (!strcasecmp(s, "width")) return IMAGESCALE_WIDTH;
        if (!strcasecmp(s, "height")) return IMAGESCALE_HEIGHT;
        if (!strcasecmp(s, "both")) return IMAGESCALE_BOTH;
        if (mapbool(s)) return IMAGESCALE_TRUE;
        return IMAGESCALE_FALSE;
}

/* gvrender_usershape:
 * Scale image to fill polygon bounding box according to "imagescale"
 */
void gvrender_usershape(GVJ_t * job, char *name, pointf * a, int n, 
    boolean filled, char *imagescale)
{
    gvrender_engine_t *gvre = job->render.engine;
    usershape_t *us;
    double iw, ih, pw, ph;
    double scalex, scaley;  /* scale factors */
    boxf b;		    /* target box */
    int i;
    point  isz;

    if (! (us = gvusershape_find(name))) {
	if (find_user_shape(name)) {
	    if (gvre && gvre->library_shape)
	        gvre->library_shape(job, name, a, n, filled);
	}
        return;
    }

    isz = gvusershape_size_dpi (us, job->dpi);
    if ((isz.x <= 0) && (isz.y <= 0)) return;

    /* compute bb of polygon */
    b.LL = b.UR = a[0];
    for (i = 1; i < n; i++) {
	EXPANDBP(b, a[i]);
    }

    pw = b.UR.x - b.LL.x;
    ph = b.UR.y - b.LL.y;
    ih = (double)isz.y;
    iw = (double)isz.x;

    scalex = pw / iw;
    scaley = ph / ih;

    switch (get_imagescale(imagescale)) {
    case IMAGESCALE_TRUE:
        /* keep aspect ratio fixed by just using the smaller scale */
	if (scalex < scaley) {
	    iw *= scalex;
	    ih *= scalex;
	} else {
	    iw *= scaley;
	    ih *= scaley;
	}
	break;
    case IMAGESCALE_WIDTH:
	iw *= scalex;
	break;
    case IMAGESCALE_HEIGHT:
	ih *= scaley;
	break;
    case IMAGESCALE_BOTH:
	iw *= scalex;
	ih *= scaley;
	break;
    case IMAGESCALE_FALSE:
    default:
	break;
    }

    /* if image is smaller than target area then center it */
    if (iw < pw) {
	    b.LL.x += (pw - iw) / 2.0;
	    b.UR.x -= (pw - iw) / 2.0;
    }
    if (ih < ph) {
	    b.LL.y += (ph - ih) / 2.0;
	    b.UR.y -= (ph - ih) / 2.0;
    }

    /* convert from graph to device coordinates */
    if (!(job->flags & GVRENDER_DOES_TRANSFORM)) {
	b.LL = gvrender_ptf(job, b.LL);
	b.UR = gvrender_ptf(job, b.UR);
    }

    if (b.LL.x > b.UR.x) {
	double d = b.LL.x;
	b.LL.x = b.UR.x;
	b.UR.x = d;
    }
    if (b.LL.y > b.UR.y) {
	double d = b.LL.y;
	b.LL.y = b.UR.y;
	b.UR.y = d;
    }
    if (gvre) {
	gvloadimage(job, us, b, filled, job->render.type);
    }
#ifdef WITH_CODEGENS
    else {
        codegen_t *cg = job->codegen;

	if (sizeA < n) {
	    sizeA = n+10;
	    A = grealloc(A, sizeA * sizeof(point));
	}
	for (i = 0; i < n; i++)
	    PF2P(a[i], A[i]);
        if (cg && cg->usershape)
            cg->usershape(us, b, A, n, filled);
    }
#endif
}

void gvrender_set_penwidth(GVJ_t * job, double penwidth)
{
    gvrender_engine_t *gvre = job->render.engine;

    if (gvre) {
    	job->obj->penwidth = penwidth;
    	/*if (gvre->set_penwidth) gvre->set_penwidth(job, penwidth);*/
    }
#ifdef WITH_CODEGENS
    else {
	codegen_t *cg = job->codegen;

	if (cg && cg->set_penwidth)
	    cg->set_penwidth(penwidth);
    }
#endif
}
