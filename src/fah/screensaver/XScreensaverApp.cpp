/******************************************************************************\

                  This file is part of the FAHScreensaver.

         The FAHScreensaver displays 3D views of Folding@home proteins.
                 Copyright (c) 2003-2013, Stanford University
                             All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

                For information regarding this software email:
                               Joseph Coffland
                        joseph@cauldrondevelopment.com

\******************************************************************************/

#include "XScreensaverApp.h"

#include <fah/viewer/GL.h>

#include <cbang/Exception.h>
#include <cbang/String.h>
#include <cbang/log/Logger.h>
#include <cbang/os/SystemUtilities.h>

#ifndef _WIN32
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

using namespace std;
using namespace cb;
using namespace FAH::Screensaver;


/*****************************************************************************/
/**                   Copyright 1991 by Andreas Stolcke                     **/
/**               Copyright 1990 by Solbourne Computer Inc.                 **/
/**                          Longmont, Colorado                             **/
/**                                                                         **/
/**                           All Rights Reserved                           **/
/**                                                                         **/
/**    Permission to use, copy, modify, and distribute this software and    **/
/**    its documentation  for  any  purpose  and  without  fee is hereby    **/
/**    granted, provided that the above copyright notice appear  in  all    **/
/**    copies and that both  that  copyright  notice  and  this  permis-    **/
/**    sion  notice appear in supporting  documentation,  and  that  the    **/
/**    name of Solbourne not be used in advertising                         **/
/**    in publicity pertaining to distribution of the  software  without    **/
/**    specific, written prior permission.                                  **/
/**                                                                         **/
/**    ANDREAS STOLCKE AND SOLBOURNE COMPUTER INC. DISCLAIMS ALL WARRANTIES **/
/**    WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF    **/
/**    MERCHANTABILITY  AND  FITNESS,  IN  NO  EVENT SHALL ANDREAS STOLCKE  **/
/**    OR SOLBOURNE BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL    **/
/**    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA   **/
/**    OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER    **/
/**    TORTIOUS ACTION, ARISING OUT OF OR IN  CONNECTION  WITH  THE  USE    **/
/**    OR PERFORMANCE OF THIS SOFTWARE.                                     **/
/*****************************************************************************/
// Following function from vroot.h -- Virtual Root Window handling header file
static Window VirtualRootWindowOfScreen(Screen *screen) {
  static Screen *save_screen = 0;
  static Window root = 0;

  if (screen != save_screen) {
    Display *dpy = DisplayOfScreen(screen);
    Atom __SWM_VROOT = None;
    Window rootReturn, parentReturn, *children;
    unsigned int numChildren;

    root = RootWindowOfScreen(screen);

    // go look for a virtual root
    __SWM_VROOT = XInternAtom(dpy, "__SWM_VROOT", False);
    if (XQueryTree(dpy, root, &rootReturn, &parentReturn, &children,
                   &numChildren)) {
      for (unsigned i = 0; i < numChildren; i++) {
        Atom actual_type;
        int actual_format;
        unsigned long nitems, bytesafter;
        Window *newRoot = 0;

        if (XGetWindowProperty(dpy, children[i], __SWM_VROOT, 0, 1, False,
                               XA_WINDOW, &actual_type, &actual_format,
                               &nitems, &bytesafter,
                               (unsigned char **)&newRoot) == Success &&
            newRoot) {
          root = *newRoot;
          break;
        }
      }

      if (children) XFree((char *)children);
    }

    save_screen = screen;
  }

  return root;
}


XScreensaverApp::XScreensaverApp() : root(false), existingWin(0) {
  cmdLine.setAllowSingleDashLongOpts(true);
  Options &options = getOptions();
  options.addTarget("root", root, "Enable Fullscreen");
  options.addTarget("window-id", existingWin, "Draw to this window ID");
}


int XScreensaverApp::init(int argc, char *argv[]) {
  this->argc = argc;
  this->argv = argv;

  // Parse command line, etc
  if (ScreensaverApp::init(argc, argv)) return 1;

  if (!existingWin) {
    const char *xwin = SystemUtilities::getenv("XSCREENSAVER_WINDOW");
    if (xwin) existingWin = String::parseU32(xwin);
  }

  return 0;
}


void XScreensaverApp::run() {
  // Get display
  Display *dpy = XOpenDisplay(SystemUtilities::getenv("DISPLAY"));
  if (!dpy) THROW("Cannot connect to X server");

  // Get root window
  unsigned screen = DefaultScreen(dpy);
  Window root = VirtualRootWindowOfScreen(ScreenOfDisplay(dpy, screen));

  XVisualInfo *vi;
  Window win;
  XWindowAttributes gwa;

  if (existingWin || this->root) {
    win = existingWin ? existingWin : root;
    XGetWindowAttributes(dpy, win, &gwa);

    XVisualInfo tmp;
    int count;
    tmp.screen = screen;
    tmp.visualid = XVisualIDFromVisual(gwa.visual);
    vi = XGetVisualInfo(dpy, VisualScreenMask | VisualIDMask, &tmp, &count);
    if (!vi) THROWS("Cannot get visual for window " << win);

  } else {
    // Get visual
    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    vi = glXChooseVisual(dpy, screen, att);
    if (!vi) THROW("No appropriate visual found");

    LOG_DEBUG(3, "depth=" << vi->depth);

    // Configure window
    XSetWindowAttributes swa;
    swa.colormap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.event_mask = KeyPressMask;

    // Create window
    win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput,
                        vi->visual, CWColormap | CWEventMask, &swa);
  }

  // Set command line
  XSetCommand(dpy, win, argv, argc);

  // Show window
  if (!XMapWindow(dpy, win)) THROWS("Failed to map window " << win);

  // Set title
  XStoreName(dpy, win, getName().c_str());

  // Setup OpenGL context
  GLXContext glc = glXCreateContext(dpy, vi, 0, GL_TRUE);
  glXMakeCurrent(dpy, win, glc);

  // Free VisualInfo
  XFree(vi);

  // Init screensaver
  XGetWindowAttributes(dpy, win, &gwa);
  View::slot = screen;
  ScreensaverApp::initView(gwa.width, gwa.height);

  // Main loop
  Timer timer;
  while (!shouldQuit()) {
    // Update animation and connection
    View::update(true);

    // Resize screen
    XGetWindowAttributes(dpy, win, &gwa);
    View::resize(gwa.width, gwa.height);

    // Draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    View::draw();
    if (!shouldQuit()) glXSwapBuffers(dpy, win);

    // Check for window events
    while (!shouldQuit() && XPending(dpy)) {
      XEvent event;
      XNextEvent(dpy, &event);

      if (event.type == KeyPress) {
        KeySym keysym;
        char c = 0;

        XLookupString(&event.xkey, &c, 1, &keysym, 0);
        LOG_DEBUG(3, "Key: '" << String::escapeC(string(1, c)) << "'");
        if (c == 'q' || c == 'Q' || c == 3 || c == 27) requestExit();
      }
    }

    // Throttle
    timer.throttle(1.0 / 10.0);
  }

  // Cleanup
  glXMakeCurrent(dpy, None, 0);
  glXDestroyContext(dpy, glc);
  XCloseDisplay(dpy);
}

#endif // _WIN32
