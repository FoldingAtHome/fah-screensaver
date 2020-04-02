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

#include "ScreensaverApp.h"

#include <fah/viewer/GL.h>
#include <fah/viewer/Client.h>

#include <cbang/String.h>

using namespace cb;
using namespace FAH;
using namespace FAH::Screensaver;


ScreensaverApp::ScreensaverApp() :
  Application("Folding@home Screensaver"), View(getOptions()) {
  setShowButtons(false);
  getOptions()["verbosity"].setDefault(0);
}


void ScreensaverApp::initView(unsigned width, unsigned height) {
  GLenum err = glewInit();
  if (err != GLEW_OK) THROWS("Initializing GLEW: " << glewGetErrorString(err));

  if (!GLEW_VERSION_1_1)
    THROW("Need at least OpenGL 1.1 to run this application");
  if (!getBasic() && !GLEW_VERSION_2_0) setBasic(true);

  setWidth(width);
  setHeight(height);

  if (width < 400 || height < 400) {
    setShowInfo(false);
    setShowLogos(false);
  }

  View::initView();
  loadTestData();

  Client &client = getClient();
  client.setCommand(client.getCommand() + "screensaver\n");
}
