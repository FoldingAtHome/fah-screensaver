/******************************************************************************\

                  This file is part of the FAHScreensaver.

         The FAHScreensaver displays 3D views of Folding@home proteins.
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

#ifndef FAH_SCREENSAVER_APP_H
#define FAH_SCREENSAVER_APP_H

#include <fah/viewer/View.h>

#include <cbang/Application.h>


namespace FAH {
  namespace Screensaver {
    class ScreensaverApp : public cb::Application, public View {
    public:
      ScreensaverApp();

      // From cb::Application
      using cb::Application::getName;

      void initView(unsigned width, unsigned height);
    };
  }
}

#endif // FAH_SCREENSAVER_APP_H
