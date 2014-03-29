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

#ifdef _WIN32
#include "WinScreensaverApp.h"

#include <fah/viewer/GL.h>

#include <cbang/util/DefaultCatch.h>
#include <cbang/os/SystemUtilities.h>
#include <cbang/os/Win32Registry.h>

#include <string>
#include <vector>

#include <scrnsave.h>

using namespace std;
using namespace cb;
using namespace FAH;
using namespace FAH::Screensaver;

#define TIMER 1

LRESULT WINAPI
ScreenSaverProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  return WinScreensaverApp::getInstance().proc(hWnd, message, wParam, lParam);
}


BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam,
                                       LPARAM lParam) {
  return WinScreensaverApp::getInstance().
    configureDialog(hDlg, message, wParam, lParam);
}


BOOL WINAPI RegisterDialogClasses(HANDLE hInst) {
  return TRUE;
}


WinScreensaverApp::WinScreensaverApp() : hDC(0), hRC(0) {}


WinScreensaverApp &WinScreensaverApp::getInstance() {
  static WinScreensaverApp *singleton = new WinScreensaverApp;
  return *singleton;
}


BOOL WINAPI WinScreensaverApp::configureDialog(HWND hDlg, UINT message,
                                               WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_INITDIALOG: return TRUE;
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK) EndDialog(hDlg, LOWORD(wParam));
    break;
  }

  return FALSE;
}


LRESULT WINAPI WinScreensaverApp::proc(HWND hWnd, UINT message,
                                       WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_CREATE:
    GetClientRect(hWnd, &rect);
    initGL(hWnd);
    ScreensaverApp::initView(rect.right, rect.bottom);
    SetTimer(hWnd, TIMER, 1000.0 / 24, 0);
    break;

  case WM_DESTROY:
    KillTimer(hWnd, TIMER);
    closeGL(hWnd);
    break;

  case WM_TIMER: render(); break;

  default: return DefScreenSaverProc(hWnd, message, wParam, lParam);
  }

  return 0;
}


void WinScreensaverApp::render() {
  View::update(true);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  View::draw();
  SwapBuffers(hDC);
}


void WinScreensaverApp::initGL(HWND hWnd) {
  PIXELFORMATDESCRIPTOR pfd;
  ZeroMemory(&pfd, sizeof pfd);
  pfd.nSize = sizeof pfd;
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;

  hDC = GetDC(hWnd);
  SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);

  hRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hRC);
}


void WinScreensaverApp::closeGL(HWND hWnd) {
  wglMakeCurrent(0, 0);
  wglDeleteContext(hRC);
  ReleaseDC(hWnd, hDC);
}

#endif // _WIN32
