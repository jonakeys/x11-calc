/*
 * x11-calc-button.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * Button functions.
 *
 * Defines the routines and data structures used to draw a button.
 *
 * This  program is free software: you can redistribute it and/or modify it
 * under  the terms of the GNU General Public License as published  by  the
 * Free  Software Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 02 Jul 13         - Initial version - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 18 Sep 21         - Added keycode to the button properties and a  method
 *                     to determine if this corrisponds to a button - MT
 * 23 Nov 21         - Added an additional parameter to allow the alternate
 *                     function text to be defined - MT
 * 06 Dec 21         - Label text colour now explicitly defined to allow it
 *                     to be different from the main text colour- MT
 */

typedef struct { /* Calculator button structure. */
   int index;
   XRectangle button_position;   /* Current button position */
   XRectangle button_geometry;   /* Original button position */
   XFontStruct* text_font;       /* Pointer to text font. */
   XFontStruct* function_font;   /* Pointer to function font. */
   XFontStruct* label_font;      /* Pointer to label font. */
   int state;
   int style;
   char key;                     /* Key code corisponding to button */
   char* text;                   /* Text */
   char* function;               /* Function Text */
   char* alternate;              /* Alternate Function Text */
   char* label;                  /* Label Text */
   unsigned int colour;          /* Button colour */
   unsigned int function_colour; /* Function key colour */
   unsigned int shifted_colour;  /* Alternate function key colour */
   unsigned int label_colour;    /* Shifted function key colour */
} obutton;

obutton *h_button_key_pressed(obutton *h_button, char c_key);

obutton *h_button_pressed(obutton *h_button, int i_xpos, int i_ypos);

obutton *h_button_create(int i_index, char c_key,
   char* s_text, char* s_function ,char* s_alternate ,char* s_label,
   XFontStruct *h_normal_font, XFontStruct *h_shift_font, XFontStruct *h_label_font,
   int i_left, int i_top, int i_width, int i_height,
   int i_state, int i_style,
   unsigned int i_colour, unsigned int i_function_colour,
   unsigned int i_shifted_colour, unsigned int i_label_colour);

int i_button_resize(obutton *h_button, float f_scale);

int i_button_draw(Display *h_display, int x_application_window, int i_screen,obutton *h_button);
