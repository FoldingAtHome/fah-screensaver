/******************************************************************************\

                     This file is part of the FAHScreensaver.

          The FAHScreensaver displays 3D views of Folding@home proteins.
                    Copyright (c) 2016-2020, foldingathome.org
                   Copyright (c) 2003-2016, Stanford University
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

#pragma once

#include "ScreensaverApp.h"

#ifdef _WIN32
#include <cbang/os/Subprocess.h>

#include <windows.h>


namespace FAH {
  namespace Screensaver {
    class WinScreensaverApp : public ScreensaverApp {
      HDC hDC;
      HGLRC hRC;
      RECT rect;

      cb::Subprocess subproc;

      WinScreensaverApp();
      WinScreensaverApp(WinScreensaverApp const &);
      void operator=(WinScreensaverApp const &);

    public:
      static WinScreensaverApp &getInstance();

      BOOL WINAPI configureDialog(HWND hDlg, UINT message,
                                  WPARAM wParam, LPARAM lParam);
      LRESULT WINAPI proc(HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam);

      void render();
      void initGL(HWND hWnd);
      void closeGL(HWND hWnd);
    };
  }
}

#endif // _WIN32
