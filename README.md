Folding@home Screensaver
========================

Folding@home is a distributed computing project using volunteered
computer resources run by Pandegroup of Stanford University.

This package contains the 3D screensaver.  It can connect to
local or remote FAHClients and visualize the running simulations.

See http://folding.stanford.edu/


# Build Instructions
Build instructions are the same as for [fah-viewer](https://github.com/FoldingAtHome/fah-viewer)

# Windows Build Instructions

To build this software you must also have the following source repositories:

 - https://github.com/CauldronDevelopmentLLC/cbang
 - https://github.com/FoldingAtHome/fah-viewer

and the *FAH_VIEWER_HOME* and *CBANG_HOME* environment variables set to their
root directories.

 1. Install the free MSVC Express Edition or the Intel C++ complier.

 2. Install Python 2.6 or newer.

 3. Install the Subversion command line client.

 4. Install Scons 1.2.0 or newer.

 5. Build and install C!
    See: https://github.com/CauldronDevelopmentLLC/cbang#prerequisites

 7. Install freetype
    http://download.savannah.gnu.org/releases/freetype/freetype-2.4.4.tar.bz2

        bzip2 -d "%HOMEPATH%\My Documents\Downloads\freetype-2.4.4.tar.bz2"
        tar xvf "%HOMEPATH%\My Documents\Downloads\freetype-2.4.4.tar"
        cd freetype-2.4.4
        # Build freetype-2.4.4\builds\win32\vc2008\freetype.sln
        cp objs\win32\vc2008\freetype244MT_D.lib freetype.lib

 8. Install freeglut
    http://downloads.sourceforge.net/project/freeglut/freeglut/freeglut-2.6.0.tar.gz

        gzip -d "%HOMEPATH%\My Documents\Downloads\freeglut-2.6.0.tar.gz"
        tar xvf "%HOMEPATH%\My Documents\Downloads\freeglut-2.6.0.tar"
        # Open \build\freeglut-2.6.0\VisualStudio2008Static\freeglut.sln in MSVC 2008
        # Build in Release mode
        cd freeglut-2.6.0
        cp VisualStudio2008Static\Release\freeglut_static.lib .

 10. Build the package with scons

        cd fah-viewer
        scons


# Windows Installation

To install the screensaver first build it, then copy *FAHScreensaver.scr* to
the system directory:

    copy FAHScreensaver.scr %windir%\System32
