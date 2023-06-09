/* $Id: gvdevice_gdk_pixbuf.c,v 1.11 2007/10/04 20:20:46 ellson Exp $ $Revision: 1.11 $ */
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gvplugin_device.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

typedef enum {
	FORMAT_BMP,
	FORMAT_ICO,
	FORMAT_JPEG,
	FORMAT_PNG,
	FORMAT_TIFF,
    } format_type;

/* 
 * Does an in-place conversion of a CAIRO ARGB32 image to GDK RGBA
 */
static void
argb2rgba ( unsigned int width, unsigned int height, unsigned char *data)
{
/* define indexes to color bytes in each format */
#define Ba 0
#define Ga 1
#define Ra 2
#define Aa 3

#define Rb 0
#define Gb 1
#define Bb 2
#define Ab 3

    unsigned int x, y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            /* swap red and blue */
            unsigned char r = data[Ra];
            data[Bb] = data[Ba];
	    data[Rb] = r;
            data += 4;
        }
    }
}

extern size_t gvdevice_write(GVJ_t * job, const unsigned char *s, unsigned int len);

static gboolean
writer ( const gchar *buf, gsize count, GError **error, gpointer data)
{
    if (count == gvdevice_write((GVJ_t *)data, (const unsigned char*)buf, count))
        return TRUE;
    return FALSE;
}

static void gdk_pixbuf_format(GVJ_t * job)
{
    char *format_str = "";
    GdkPixbuf *pixbuf;

    switch (job->device.id) {
    case FORMAT_BMP:
	format_str = "bmp";
	break;
    case FORMAT_ICO:
	format_str = "ico";
	break;
    case FORMAT_JPEG:
	format_str = "jpeg";
	break;
    case FORMAT_PNG:
	format_str = "png";
	break;
    case FORMAT_TIFF:
	format_str = "tiff";
	break;
    }

    argb2rgba(job->width, job->height, job->imagedata);

    pixbuf = gdk_pixbuf_new_from_data(
                job->imagedata,         // data
                GDK_COLORSPACE_RGB,     // colorspace
                TRUE,                   // has_alpha
                8,                      // bits_per_sample
                job->width,             // width
                job->height,            // height
                4 * job->width,         // rowstride
                NULL,                   // destroy_fn
                NULL                    // destroy_fn_data
               );

    gdk_pixbuf_save_to_callback(pixbuf, writer, job, format_str, NULL, NULL);

    gdk_pixbuf_unref(pixbuf);
}

static gvdevice_engine_t gdk_pixbuf_engine = {
    NULL,		/* gdk_pixbuf_initialize */
    gdk_pixbuf_format,
    NULL,		/* gdk_pixbuf_finalize */
};

static gvdevice_features_t device_features_gdk_pixbuf = {
    GVDEVICE_BINARY_FORMAT
      | GVDEVICE_DOES_TRUECOLOR,/* flags */
    {0.,0.},                    /* default margin - points */
    {0.,0.},                    /* default page width, height - points */
    {96.,96.},                  /* dpi */
};

gvplugin_installed_t gvdevice_gdk_pixbuf_types[] = {
    {FORMAT_BMP, "bmp:cairo", 4, &gdk_pixbuf_engine, &device_features_gdk_pixbuf},
    {FORMAT_ICO, "ico:cairo", 4, &gdk_pixbuf_engine, &device_features_gdk_pixbuf},
    {FORMAT_JPEG, "jpe:cairo", 4, &gdk_pixbuf_engine, &device_features_gdk_pixbuf},
    {FORMAT_JPEG, "jpeg:cairo", 4, &gdk_pixbuf_engine, &device_features_gdk_pixbuf},
    {FORMAT_JPEG, "jpg:cairo", 4, &gdk_pixbuf_engine, &device_features_gdk_pixbuf},
    {FORMAT_PNG, "png:cairo", 4, &gdk_pixbuf_engine, &device_features_gdk_pixbuf},
    {FORMAT_TIFF, "tif:cairo", 4, &gdk_pixbuf_engine, &device_features_gdk_pixbuf},
    {FORMAT_TIFF, "tiff:cairo", 4, &gdk_pixbuf_engine, &device_features_gdk_pixbuf},
    {0, NULL, 0, NULL, NULL}
};
