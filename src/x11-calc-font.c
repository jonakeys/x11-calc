/*
 * x11-calc-font.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * Font definations and data structures.
 *
 * This  program is free software: you can redistribute it and/or modify  it
 * under  the  terms of the GNU General Public License as published  by  the
 * Free  Software  Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is  distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without  even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License  along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * 24 Feb 24         - Initial version - MT
 *                   - Defined an array of alternative fonts for each style
 *                     of  text required and select the most suitable fonts
 *                     from these alternatives to reduce the dependancy  on
 *                     a single set of fonts - MT
 * 25 Feb 24         - Reformatted arrays of font names in order to make it
 *                     easier to comment out unwanted fonts - MT
 * 27 Feb 24         - Set errno to EBFONT if an error occours loading when
 *                     loading a font - MT
 *
 */

#define NAME           "x11-calc-button"
#define BUILD          "0016"
#define DATE           "26 Nov 22"
#define AUTHOR         "MT"

#define DEBUG

#include <errno.h>

#include <stdio.h>
#include <X11/Xlib.h>

#include "gcc-debug.h"

const char* s_normal_fonts[] =
{
   "6x12", /* X11 base font */
   "*helvetica-medium-r-*-10-*", /* VMS */
   "-*-courier-medium-r-*-*-11-*-*-*-*-*-*-*", /* Fallback */
/* "-*-fixed-medium-r-normal-*-10-*-*-*-*-*-iso8859-*", /* Alternative */
   NULL
};
const char* s_small_fonts[] =
{
   "6x10", /* X11 base font */
   "*helvetica-medium-r-*-8-*", /* VMS */
   "-*-courier-medium-r-*-*-11-*-*-*-*-*-*-*", /* Fallback */
/* "-*-fixed-medium-r-normal-*-10-*-*-*-*-*-iso8859-*", /* Alternative */
   NULL
};
const char* s_alternate_fonts[] =
{
   "5x8", /* X11 base font */
   "*helvetica-medium-r-*-8-*", /* VMS */
   "-*-courier-medium-r-*-*-11-*-*-*-*-*-*-*", /* Fallback */
/* "-*-fixed-medium-r-normal-*-8-*-*-*-*-*-iso8859-*", /* Alternative */
   NULL
};
const char* s_large_fonts[] =
{
   "6x13", /* X11 base font */
   "*helvetica-medium-r-*-12-*", /* VMS */
   "-*-courier-medium-r-*-*-14-*-*-*-*-*-*-*", /* Fallback */
/* "-*-fixed-medium-r-normal-*-13-*-*-*-*-*-iso8859-*", /* Alternative */
   NULL
};

XFontStruct *h_get_font(Display *x_display, const char** s_fonts)
/*
 * The get_font() function returns the first available font from a list  of
 * possible alternative font names.  If none of the fonts exist, it returns
 * NULL.
 *
 */
{
   XFontStruct* h_font;
   int i_count;

   for (i_count = 0; s_fonts[i_count] != NULL; i_count++)
   {
      if ((h_font = XLoadQueryFont(x_display, s_fonts[i_count])))
      {
         debug(printf("Successfully loaded font '%s'\n", s_fonts[i_count]));
         break;
      }
      debug(printf("Unable to load font '%s'\n", s_fonts[i_count]));
      errno = EBFONT;
      h_font = NULL;
   }
   return(h_font);
}
