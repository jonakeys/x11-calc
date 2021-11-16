/*
 * x11-calc.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2019 - MT
 *
 * Emulation of various models of HP calculator for X11.
 *
 * Deliberately parses the command line without using 'getopt' or 'argparse'
 * to maximise portability.
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
 * 13 Jun 13   0.1   - Initial version - MT
 * 16 Jun 13         - Can use predefined button colours - MT
 *                   - Added text to buttons - MT
 *                   - Defined  all  the font names and colours defined  as
 *                     constants - MT
 * 17 Jun 13         - All fonts now loaded in main routine - MT
 * 18 Jun 13         - Added font attributes to each button - MT
 * 23 Jun 13         - Added  button_create to create a button and return a
 *                     pointer to the new structure - MT
 *                   - Changed button_draw now extracts parameters from the
 *                     button structure - MT
 *                   - Modified  string  constants  in  key  definitions  to
 *                     define any hex character codes properly - MT
 * 27 Jun 13         - Any error opening the display are now handled in the
 *                     same way as other errors - MT
 * 30 Jun 13         - Defined text colours as constants - MT
 * 01 Jul 13         - Buttons now move when pressed - MT
 * 02 Jul 13         - Separated  code  and  headers for button  and  colour
 *                     manipulation into separate files - MT
 * 07 Jul 13         - Made  all  global variables local  variables  (except
 *                     fonts) - MT
 *                   - Use window size hints to set window size - MT
 *                   - Changed  display of inverse trig functions  so  that
 *                     minus sign is shown as a super script - MT
 * 14 Jul 13         - Added display segments - MT
 * 15 Jul 13         - Created a display , to allow the 7 segment LEDs
 *                     to  be  handled as a single display entity  to  make
 *                     updating the display easier - MT
 * 14 Aug 13         - Tidied up comments - MT
 * 17 Aug 13         - Window is now redrawn automatically when application
 *                     loads - MT
 * 09 Mar 14         - Created  separate files for code that is dependent on
 *                     calculator the model - MT
 * 10 Mar 14         - Changed key code indexes to BCD hex values - MT
 *                   - Changed  names  of display masks to highlight  their
 *                     association with the display module - MT
 * 07 Dec 18         - Finally remembered how to pass WM_DELETE messages to
 *                     the  message loop so the application exits  properly
 *                     when the user closes the application window - MT
 *                   - When a key is 'pressed' the display goes blank for a
 *                     brief interval (just like the real thing) - MT
 * 10 Dec 18         - Alternate  function key now LIGHT_BLUE, allowing  it
 *                     to be a different colour to the alternate text - MT
 * 11 Dec 18         - Tidied up comments again - MT
 * 23 Aug 20         - Modified the debug code (again), optional debug code
 *                     executed using the debug() macro, with print() being
 *                     used to print diagnostic messages - MT
 * 30 Aug 20         - Added number formatting routines - MT
 * 31 Aug 20         - Made  version(), about(), and error() model specific
 *                     so that the name reflects the model number - MT
 * 31 Aug 20         - Resolved dependencies between header files by moving
 *                     common function definitions to a separate file - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 09 Aug 21         - The main program loop now checks for pending  events
 *                     before  processing  any event messages, so NextEvent
 *                     no longer blocks program execution - MT
 * 10 Aug 21         - Moved  version(), about(), and error() back to their
 *                     original position - MT
 * 16 Aug 21         - Executes  a single instruction in every iteration of
 *                     the main event loop - MT
 * 19 Aug 21         - Modified processor simulation to make it more ,
 *                     orientated - MT
 * 21 Aug 21         - Removed short form of the help option and fixed help
 *                     text - MT
 *                   - Added a flags for the Shift, Ctrl and Alt keys - MT
 *                   - Added single step and trace command line options and
 *                     the  ability  to control the processor using  ctrl-s
 *                     and ctrl-q for single stepping, and ctrl-t to enable
 *                     or disable tracing - MT
 *                   - Added definition for commit id - MT
 * 30 Aug 21         - Separated version and licence notices into their own
 *                     routines - MT
 *                   - Abort if an error occurs - MT
 *  2 Sep 21         - Can  now enable single stepping and tracing using  a
 *                     hard-coded break-point - MT
 *  5 Sep 21         - The display is no longer blanked automatically  when
 *                     a key is pressed - MT
 *                   - Key press events now handled properly using keypress
 *                     and keystate properties - MT
 *                   - Tidied up comments - MT
 *  9 Sep 21         - Don't trace busy loops - MT
 * 12 Sep 21         - Fixed bug with single step and trace  - MT
 *                   - Added  option to allow breakpoint to be set from the
 *                     command line - MT
 * 14 Sep 21         - Eliminated  the delay between ticks when the display
 *                     is blank - MT
 *             0.2   - HP21 simulator works !!
 * 15 Sep 21         - The  escape key now resets the simulator instead  of
 *                     exiting - MT
 * 16 Sep 21         - Improved the display flicker problem by updating the
 *                     display  only once every 100 ticks instead of  every
 *                     tick- MT
 * 18 Sep 21         - Added keycode to the button properties and a  method
 *                     to  determine if this corresponds to a button  which
 *                     allows a button to be operated by a key - MT
 * 19 Sep 21         - Created a keyboard class to keep track of the  input
 *                     state and translate keystrokes into characters - MT
 * 21 Sep 21         - Set the default active button to NULL - MT
 * 28 Sep 21         - The  escape key now mapped to 'Clx' - MT
 *  1 Oct 21         - Converted flags to Boolean variables - MT
 *  2 Oct 21         - Changed property names - MT
 *  4 Oct 21         - Added the ability to display the CPU registers using
 *                     Ctrl-R when in trace mode - MT
 *  7 Oct 21   0.3   - HP25 simulator now working..
 * 10 Oct 21         - Allows  switches and buttons to be undefined if  not
 *                     used - MT
 * 11 Oct 21         - Most  text  messages are now defined in the  header,
 *                     this allows the content to be defined separately for
 *                     each platform which means that about() and licence()
 *                     can  be replaced by simple calls to fprintf().  This
 *                     also  makes it easier to support multiple  languages
 *                     if necessary in the future  - MT
 *                   - Required  colour depth now defined in header (allows
 *                     code to work on VMS using a black and white  display
 *                     without being modified) - MT
 * 12 Oct 21         - Added routine to display warnings - MT
 * 14 Oct 21         - Added support for continuous memory - MT
 * 16 Oct 21   0.4   - HP29 simulator works..!
 *                   - Undefined  switches should be off.  Rather strangely
 *                     the HP27 tests S3 even though it doesn't have a mode
 *                     select switch - go figure - MT
 *             0.5   - HP27 simulator works (requires testing).
 *             0.6   - HP31 and HP32 simulators work (require testing).
 * 26 Oct 21   0.7   - Added the ability to allow the user to specify which
 *                     file should be used when restoring the initial state
 *                     of  the registers on the command line (only  applies
 *                     to models with continuous memory) - MT
 * 29 Oct 21         - Draw display when window is exposed - MT
 * 01 Nov 21         - Explicitly define the cursor - MT
 *                   - Attempts  to center the window on the display.  Most
 *                     window managers ignore this, but it does work if the
 *                     application is invoked directly by startx - MT
 * 02 Nov 21         - Allows size of the window to be changed by modifying
 *                     the value of SCALE at compile time - MT
 * 15 Nov 21         - Holding down the off switch for two seconds will end
 *                     the simulation - MT
 *
 * To Do             - Combine error and warning routines (add severity  to
 *                     parameters).
 *                   - Parse command line in a separate routine.
 *                   - Save trace and single step options and restore when
 *                     resetting the processor...
 *                   - Load ROMs from a separate file?
 *                   - Fix display update problem on Raspberry Pi.
 *                   - Allow VMS users to set breakpoints?
 *                   - Free up allocated memory on exit.
 *                   - Sort out colour mapping.
 *
 */

#define NAME           "x11-calc"
#define VERSION        "0.6"
#define BUILD          "0071"
#define DATE           "16 Oct 21"
#define AUTHOR         "MT"

#define DEBUG 1        /* Enable/disable debug*/

#define INTERVAL 25    /* Number of ticks to execute before updating the display */

#include <stdarg.h>    /* strlen(), etc */
#include <string.h>    /* strlen(), etc */
#include <stdio.h>     /* fprintf(), etc */
#include <stdlib.h>    /* getenv(), etc */

#include <ctype.h>     /* isprint(), etc */

#include <sys/timeb.h>

#include <X11/Xlib.h>  /* XOpenDisplay(), etc */
#include <X11/Xutil.h> /* XSizeHints etc */
#include <X11/cursorfont.h>

#include "x11-calc-font.h"
#include "x11-calc-button.h"
#include "x11-calc-switch.h"
#include "x11-calc-colour.h"

#include "x11-calc.h"

#include "x11-calc-segment.h"
#include "x11-calc-display.h"
#include "x11-calc-cpu.h"

#include "x11-keyboard.h"

#include "gcc-debug.h" /* print() */
#include "gcc-wait.h"  /* i_wait() */

void v_version() /* Display version information */
{
   fprintf(stderr, "%s: Version %s %s", FILENAME, VERSION, COMMIT_ID);
   if (__DATE__[4] == ' ') fprintf(stderr, " 0"); else fprintf(stderr, " %c", __DATE__[4]);
   fprintf(stderr, "%c %c%c%c %s %s", __DATE__[5],
      __DATE__[0], __DATE__[1], __DATE__[2], __DATE__ +9, __TIME__ );
   fprintf(stderr, " (Build: %s)\n", BUILD );
}

void v_error(const char *s_fmt, ...) /* Print formatted error message and exit */
{
   va_list t_args;
   va_start(t_args, s_fmt);
   fprintf(stderr, "%s : ", FILENAME);
   vfprintf(stderr, s_fmt, t_args);
   va_end(t_args);
   exit(-1);
}

void v_warning(const char *s_fmt, ...) /* Print formatted warning message and return */
{
   va_list t_args;
   va_start(t_args, s_fmt);
   fprintf(stderr, "%s : ", FILENAME);
   vfprintf(stderr, s_fmt, t_args);
   va_end(t_args);
}


void v_set_blank_cursor(Display *x_display, Window x_application_window, Cursor *x_cursor)
{
   Pixmap x_blank;
   XColor x_Color;
   char c_pixmap_data[1] = {0}; /* An empty pixmap */

   x_blank = XCreateBitmapFromData (x_display, x_application_window, c_pixmap_data, 1, 1); /* Create an empty bitmap */
   (*x_cursor) = XCreatePixmapCursor(x_display, x_blank, x_blank, &x_Color, &x_Color, 0, 0); /* Use the empty pixmap to create a blank cursor */
   XFreePixmap (x_display, x_blank); /* Free up pixmap */
}

long l_now() /* Returns the current time in ms (since 1 )*/
{
   struct timeb h_time;
   ftime(&h_time);
   return ((1000 * h_time.time) + h_time.millitm);
}

int main(int argc, char *argv[]){

   Display *x_display; /* Pointer to X display structure */
   Window x_application_window; /* Application window structure */
   Cursor x_cursor; /* Application cursor */
   XEvent x_event;
   XSizeHints *h_size_hint;
   Atom wm_delete;

   oswitch *h_switch[2];
   /* oswitch *h_selected = NULL; */
   obutton *h_button[BUTTONS]; /* Array to hold pointers to buttons */
   obutton *h_pressed = NULL;
   odisplay *h_display; /* Pointer to display structure */
   okeyboard *h_keyboard;
   oprocessor *h_processor;

   char *s_display_name = ""; /* Just use the default display */
   char *s_font; /* Font description */

   char *s_title = TITLE; /* Windows title */
   char *s_pathname = NULL;

   unsigned int i_screen_width; /* Screen width */
   unsigned int i_screen_height; /* Screen height */
   unsigned int i_window_width = WIDTH; /* Window width in pixels */
   unsigned int i_window_height = HEIGHT; /* Window height in pixels */
   unsigned int i_window_border = 4; /* Window's border width */
   unsigned int i_colour_depth; /* Window's colour depth */
   unsigned int i_background_colour; /* Window's background colour */
   int i_window_left, i_window_top; /* Window's top-left corner */
   int i_screen; /* Default screen number */

   char b_trace = False; /* Trace flag */
   char b_step = False; /* Single step flag flag */
   char b_cursor = True; /* Draw a cursor */
   char b_run = True; /* Run flag controls CPU instruction execution in main loop */
   char b_abort = False; /*Abort flag controls execution of main loop */

   int i_offset, i_count, i_index;
   int i_current = -1; /* Current program counter */
   int i_breakpoint = -1; /* Break-point */

   long l_time = 0; /* Current time in milliseconds */

#ifdef vms /* Parse DEC style command line options */
   for (i_count = 1; i_count < argc; i_count++) {
      if (argv[i_count][0] == '/') {
         for (i_index = 0; argv[i_count][i_index]; i_index++) /* Convert option to uppercase */
            if (argv[i_count][i_index] >= 'a' && argv[i_count][i_index] <= 'z')
               argv[i_count][i_index] = argv[i_count][i_index] - 32; /* TO DO - Assumes 8-bit ASCII encoding */
         if (!strncmp(argv[i_count], "/STEP", i_index))
            b_trace = True; /* Start in single step mode */
         else if (!strncmp(argv[i_count], "/CURSOR", i_index))
            b_cursor = True; /* Enable tracing */
         else if (!strncmp(argv[i_count], "/NOCURSOR", i_index))
            b_trace = False; /* Enable tracing */
         else if (!strncmp(argv[i_count], "/TRACE", i_index))
            b_trace = True; /* Enable tracing */
         else if (!strncmp(argv[i_count], "/VERSION", i_index)) {
            v_version; /* Display version information */
            fprintf(stderr, LICENCE_TEXT, __DATE__ +7, AUTHOR);
            exit(0);
         }
         else if ((!strncmp(argv[i_count], "/HELP", i_index)) | (!strncmp(argv[i_count], "/?", i_index))) {
            fprintf(stdout, HELP_TEXT, FILENAME);
            exit(0);
         }
         else /* If we get here then the we have an invalid option */
            v_error(INVALID_OPTION HELP_COMMAND, argv[i_count] , FILENAME);
         if (argv[i_count][1] != 0) {
            for (i_index = i_count; i_index < argc - 1; i_index++) argv[i_index] = argv[i_index + 1];
            argc--; i_count--;
         }
      }
   }
#else /* Parse UNIX style command line options */
   b_abort = False; /* Stop processing command line */
   for (i_count = 1; i_count < argc && (b_abort != True); i_count++) {
      if (argv[i_count][0] == '-') {
         i_index = 1;
         while (argv[i_count][i_index] != 0) {
            switch (argv[i_count][i_index]) {
            case 'b': /* Breakpoint */
               if (argv[i_count][i_index + 1] != 0)
                  v_error(INVALID_ARGUMENT, argv[i_count][i_index + 1], FILENAME);
               else {
                  if (i_count + 1 < argc) {
                     i_breakpoint = 0;
                     for (i_offset = 0; i_offset < strlen(argv[i_count + 1]); i_offset++) { /* Parse octal number */
                        if ((argv[i_count + 1][i_offset] < '0') || (argv[i_count + 1][i_offset] > '7'))
                           v_error(INVALID_ADDRESS , argv[i_count + 1], FILENAME);
                        else
                           i_breakpoint = i_breakpoint * 8 + argv[i_count + 1][i_offset] - '0';
                     }
                     if ((i_breakpoint < 0)  || (i_breakpoint > (unsigned)(sizeof(i_rom) / sizeof i_rom[0]))) { /* Check address range */
                        v_error(INVALID_ADDRESS HELP_COMMAND, argv[i_count + 1], FILENAME);
                     }
                     else {
                        if (i_count + 2 < argc) /* Remove the parameter from the arguments */
                           for (i_offset = i_count + 1; i_offset < argc - 1; i_offset++)
                              argv[i_offset] = argv[i_offset + 1];
                        argc--;
                     }
                  }
                  else {
                     v_error(MISSING_ARGUMENT HELP_COMMAND, argv[i_count], FILENAME);
                  }
               }
               i_index = strlen(argv[i_count]) - 1;
               break;
            case 's': /* Start in single step mode */
               b_trace = b_step = True;
               break;
            case 't': /* Enable tracing */
               b_trace = True;
               break;
            case '-': /* '--' terminates command line processing */
               i_index = strlen(argv[i_count]);
               if (i_index == 2)
                 b_abort = True; /* '--' terminates command line processing */
               else
                  if (!strncmp(argv[i_count], "--no-cursor", i_index)) {
                     b_cursor = False; /* Don't draw a cursor - unless drawn by the window manager */
                  }
                  else if (!strncmp(argv[i_count], "--cursor", i_index)) {
                     b_cursor = True; /* Draw cursor */
                  }
                  else if (!strncmp(argv[i_count], "--version", i_index)) {
                     v_version(); /* Display version information */
                     fprintf(stderr, LICENCE_TEXT, __DATE__ +7, AUTHOR);
                     exit(0);
                  }
                  else if (!strncmp(argv[i_count], "--help", i_index)) {
                     fprintf(stdout, HELP_TEXT, FILENAME);
                     exit(0);
                  }
                  else  /* If we get here then the we have an invalid long option */
                     v_error(UNRECOGNIZED_OPTION HELP_COMMAND, argv[i_count], FILENAME);
               i_index--; /* Leave index pointing at end of string (so argv[i_count][i_index] = 0) */
               break;
            default: /* If we get here the single letter option is unknown */
               v_error(INVALID_OPTION HELP_COMMAND, argv[i_count][i_index], FILENAME);
            }
            i_index++; /* Parse next letter in option */
         }
         if (argv[i_count][1] != 0) {
            for (i_offset = i_count; i_offset < argc - 1; i_offset++)
               argv[i_offset] = argv[i_offset + 1];
            argc--; i_count--;
         }
      }
   }
#endif

   if (argc > 2) v_error(INVALID_COMMAND, FILENAME); /* There should never be more than one command lime parameter */

   if (argc > 1) {
      if (CONTINIOUS)
         s_pathname = argv[1]; /* Set path name if a parameter was passed and continuous memory is enabled */
      else
         v_error(INVALID_COMMAND, FILENAME); /* There shouldn't any command lime parameters */
   }

   i_wait(200); /* Sleep for 200 milliseconds to 'debounce' keyboard! */

   v_version();

   /* Open the display and create a new window */
   if (!(x_display = XOpenDisplay(s_display_name))) v_error ("Cannot connect to X server '%s'.\n", s_display_name);

   /* Get the default screen for our X server */
   i_screen = DefaultScreen(x_display);
   i_screen_width = DisplayWidth(x_display, i_screen);
   i_screen_height = DisplayHeight(x_display, i_screen);

   /* Set the background colour */
   i_background_colour = BACKGROUND;

   /* Create the application window, as a child of the root window */
   x_application_window = XCreateSimpleWindow(x_display,
      RootWindow(x_display, i_screen),
      (i_screen_width - i_window_width) / 2 , (i_screen_height - i_window_height) / 2, /* Window position -ignored ? */
      i_window_width, /* Window width */
      i_window_height, /* Window height */
      i_window_border, /* Border width - ignored ? */
      BlackPixel(x_display, i_screen), /* Preferred method */
      i_background_colour);

   /* Set application window size */
   h_size_hint = XAllocSizeHints();
   h_size_hint->flags = PMinSize | PMaxSize;
   h_size_hint->min_height = i_window_height;
   h_size_hint->min_width = i_window_width;
   h_size_hint->max_height = i_window_height;
   h_size_hint->max_width = i_window_width;
   XSetWMNormalHints(x_display, x_application_window, h_size_hint);
   XStoreName(x_display, x_application_window, s_title); /* Set the window title */

   /* Get window geometry */
   if (XGetGeometry(x_display, x_application_window,
         &RootWindow(x_display, i_screen),
         &i_window_left, &i_window_top,
         &i_window_width,
         &i_window_height,
         &i_window_border,
         &i_colour_depth) == False)
      v_error(DISPLAY_ERROR);

   /* Check colour depth */
   if (i_colour_depth != COLOUR_DEPTH) v_error(COLOUR_ERROR, COLOUR_DEPTH);

   if (b_cursor)
      x_cursor = XCreateFontCursor(x_display, XC_arrow); /* Create a 'default' cursor */
   else
      v_set_blank_cursor(x_display, x_application_window, &x_cursor); /* Get a blank cursor */

   XDefineCursor(x_display, x_application_window, x_cursor); /* Define the desired X cursor */

   /* Load fonts */
   s_font = NORMAL_TEXT; /* Normal text font */
   if (!(h_normal_font = XLoadQueryFont(x_display, s_font))) v_error(FONT_ERROR, s_font);

   s_font = SMALL_TEXT; /* Small text font */
   if (!(h_small_font = XLoadQueryFont(x_display, s_font))) v_error(FONT_ERROR, s_font);

   s_font = ALTERNATE_TEXT; /* Alternate text font */
   if (!(h_alternate_font = XLoadQueryFont(x_display, s_font))) v_error(FONT_ERROR, s_font);

   s_font = LARGE_TEXT; /* Large text font */
   if (!(h_large_font = XLoadQueryFont(x_display, s_font))) v_error(FONT_ERROR, s_font);

   v_init_keypad(h_button, h_switch); /* Create buttons */

   h_display = h_display_create(0, DISPLAY_LEFT, DISPLAY_TOP, DISPLAY_WIDTH, DISPLAY_HEIGHT, RED, DARK_RED, RED_BACKGROUND); /* Create display */

#ifdef linux

   h_keyboard = h_keyboard_create(x_display); /* Only works with Linux */

#endif

   /* Select kind of events we are interested in */
   XSelectInput(x_display, x_application_window, FocusChangeMask | ExposureMask |
      KeyPressMask | KeyReleaseMask | ButtonPressMask |
      ButtonReleaseMask | StructureNotifyMask | SubstructureNotifyMask);

   wm_delete = XInternAtom(x_display, "WM_DELETE_WINDOW", False); /* Create a windows delete message 'atom'. */
   XSetWMProtocols(x_display, x_application_window, &wm_delete, 1); /* Tell the display to pass wm_delete messages to the application window */

   /* Show window on display */
   XMapWindow(x_display, x_application_window);
   XRaiseWindow(x_display, x_application_window); /* Raise window - ensures expose event is raised? */

   fprintf(stderr, "ROM Size : %4u words \n", (unsigned)(sizeof(i_rom) / sizeof i_rom[0]));
   h_processor = h_processor_create(i_rom);
   if (s_pathname == NULL)
      v_processor_restore(h_processor);
   else
      v_processor_load(h_processor, s_pathname); /* Load user specified settings */

   /* Main program event loop */
   b_abort = False;
   i_count = 0;

   if (h_switch[1] != NULL) h_processor->select = h_switch[1]->state; else h_processor->select = False; /* Allow switches to be undefined if not used */

   while (!b_abort) {
      i_count--;
      if (i_count < 0) {
         i_display_update(x_display, x_application_window, i_screen, h_display, h_processor);
         i_display_draw(x_display, x_application_window, i_screen, h_display); /* Redraw display */
         i_count = INTERVAL;
         i_wait(INTERVAL / 2); /* Sleep for 0.5 ms per tick */
         if ((l_time > 0) && (l_now() > (l_time + 2000))) b_abort = True;
      }

      if (h_processor->pc == i_breakpoint) b_trace = b_step = True;/* Breakpoint */
      h_processor->flags[TRACE] = b_trace;
      if (h_processor->pc == i_current) h_processor->flags[TRACE] = False; /* Don't trace busy loops */
      if (b_run) {
         i_current = h_processor->pc; v_processor_tick(h_processor);
      }
      if (b_step) b_run = False;

      while (XPending(x_display)) {
         XNextEvent(x_display, &x_event);
         switch (x_event.type) {
         case FocusOut:
            if (!(h_pressed == NULL)) {
               h_pressed->state = False;
               i_button_draw(x_display, x_application_window, i_screen, h_pressed);
               h_processor->keypressed = False; /* Don't clear the status bit here!! */
            }
            break;

#ifdef linux

         case KeyPress :
            h_key_pressed(h_keyboard, x_display, x_event.xkey.keycode, x_event.xkey.state); /* Attempts to translate a key code into a character */
            if (h_keyboard->key == (XK_Z & 0x1f)) /* Ctrl-z to exit */
               b_abort = True;
            else if (h_keyboard->key == (XK_Q & 0x1f)) /* Ctrl-Q to resume */
               b_step = !(b_run  = True);
            else if (h_keyboard->key == (XK_S & 0x1f)) /* Ctrl-S or space to single step */
               b_trace = b_step = b_run = True;
            else if (h_keyboard->key == (XK_T & 0x1f)) /* Ctrl-T to toggle tracing */
               b_trace = !b_trace;
            else if (h_keyboard->key == (XK_R & 0x1f)) /* Ctrl-R to display internal CPU registers */
               v_fprint_state(stdout, h_processor);
            else if (h_keyboard->key == (XK_C & 0x1f)) { /* Ctrl-C to reset */
               v_processor_reset(h_processor);
               if (s_pathname == NULL)
                  v_processor_restore(h_processor); /* Load current saved settings */
               else
                  v_processor_load(h_processor, s_pathname); /* Load user specified settings */
               b_run = True;
            }
            else { /* Check for matching button */
               int i_count;
               for (i_count = 0; i_count < BUTTONS; i_count++){
                  h_pressed = h_button_key_pressed(h_button[i_count], h_keyboard->key);
                  if (h_pressed != NULL) {
                     h_pressed->state = True;
                     i_button_draw(x_display, x_application_window, i_screen, h_pressed);

                     h_processor->code = h_pressed->index;
                     h_processor->keypressed = True;
                     /* h_processor->status[15] = True; */
                     break;
                  }
               }
            }
            break;
         case KeyRelease :
            h_key_released(h_keyboard, x_display, x_event.xkey.keycode, x_event.xkey.state);
            if (h_pressed != NULL) {
               if (h_keyboard->key == h_pressed->key) {
                  h_pressed->state = False;
                  i_button_draw(x_display, x_application_window, i_screen, h_pressed);
                  h_processor->keypressed = False; /* Don't clear the status bit here!! */
               }
            }
            break;

#endif

         case ButtonPress :
            if (x_event.xbutton.button == 1) {
               int i_count;
               for (i_count = 0; i_count < sizeof(h_button) / sizeof(*h_button); i_count++) {
                  h_pressed = h_button_pressed(h_button[i_count], x_event.xbutton.x, x_event.xbutton.y);
                  if (!(h_pressed == NULL)) {
                     h_pressed->state = True;
                     i_button_draw(x_display, x_application_window, i_screen, h_pressed);
                     h_processor->code = h_pressed->index;
                     h_processor->keypressed = True;
                     debug(fprintf(stderr, "Button pressed - keycode(%.2X).\n", h_pressed->index));
                     break;
                  }
               }
               if (h_pressed == NULL) { /* It wasn't a button that was pressed check the switches */
                  if (!(h_switch_pressed(h_switch[0], x_event.xbutton.x, x_event.xbutton.y) == NULL)) {
                     h_switch[0]->state = !(h_switch[0]->state); /* Toggle switch */
                     i_switch_draw(x_display, x_application_window, i_screen, h_switch[0]);
                     if (h_switch[0]->state) {
                        v_processor_reset(h_processor); /* Reset the processor */
                        v_processor_restore(h_processor); /* Restore saved settings */
                     }
                     else {
                        v_processor_save(h_processor); /* Save current settings */
                        h_processor->enabled = False; /* Disable the processor */
                        l_time = l_now();
                     }
                     debug(fprintf(stderr, "Switch pressed (%s).\n", h_switch[0]->state ? "On" : "Off"));
                  }
                  if (!(h_switch_pressed(h_switch[1], x_event.xbutton.x, x_event.xbutton.y) == NULL)) {
                     h_switch[1]->state = !(h_switch[1]->state); /* Toggle switch */
                     i_switch_draw(x_display, x_application_window, i_screen, h_switch[1]);
                     h_processor->select = h_switch[1]->state;
                     debug(fprintf(stderr, "Switch pressed (%s).\n", h_switch[1]->state ? "On" : "Off"));
                  }
               }
            }
            break;
         case ButtonRelease :
            if (x_event.xbutton.button == 1) {
               if (!(h_pressed == NULL)) {
                  h_pressed->state = False;
                  i_button_draw(x_display, x_application_window, i_screen, h_pressed);
                  h_processor->keypressed = False; /* Don't clear the status bit here!! */
                  debug(fprintf(stderr, "Button released - keycode(%.2X).\n", h_pressed->index));
               }
               if (h_pressed == NULL) { /* It wasn't a button that was released check the switches */
                  if (!(h_switch_pressed(h_switch[0], x_event.xbutton.x, x_event.xbutton.y) == NULL)) {
                     l_time = 0; /* Reset timer value */
                     debug(fprintf(stderr, "Switch released (%s).\n", h_switch[0]->state ? "On" : "Off"));
                  }
                  if (!(h_switch_pressed(h_switch[1], x_event.xbutton.x, x_event.xbutton.y) == NULL)) {
                     debug(fprintf(stderr, "Switch released (%s).\n", h_switch[1]->state ? "On" : "Off"));
                  }
               }
            }
            break;
         case Expose : /* Draw or redraw the window */
            i_display_draw(x_display, x_application_window, i_screen, h_display);/* Draw display */
            i_switch_draw(x_display, x_application_window, i_screen, h_switch[0]);
            i_switch_draw(x_display, x_application_window, i_screen, h_switch[1]);
            {
               int i_count;
               for (i_count = 0; i_count < BUTTONS; i_count++) /* Draw buttons */
                  i_button_draw(x_display, x_application_window, i_screen, h_button[i_count]);
            }
            break;
         case ClientMessage : /* Message from window manager */
            if (x_event.xclient.data.l[0] == wm_delete) b_abort = True;
            break;
         }
      }
   }


   v_processor_save(h_processor); /* Save state */

   /** XFreeCursor (x_display, x_cursor); /* Free cursor */
   XDestroyWindow(x_display, x_application_window); /* Close connection to server */
   XCloseDisplay(x_display);

   exit(0);
}
