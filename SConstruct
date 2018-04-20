# Setup
import os
env = Environment(ENV = os.environ)
try:
    env.Tool('config', toolpath = [os.environ.get('CBANG_HOME')])
except Exception, e:
    raise Exception, 'CBANG_HOME not set?\n' + str(e)

env.CBLoadTools('compiler cbang dist libfah fah-client-version fah-viewer ' +
                'packager')

# Override mostly_static to default True
env.CBAddVariables(
    BoolVariable('mostly_static', 'Link most libraries statically', 1))

conf = env.CBConfigure()

# Version
version = env.FAHClientVersion()

# Config vars
env.Replace(BUILD_INFO_NS = 'FAH::BuildInfo')

win32 = env['PLATFORM'] == 'win32' or int(env.get('cross_mingw', 0))

if not env.GetOption('clean'):
    conf.CBConfig('compiler')
    conf.CBConfig('libfah')
    conf.CBConfig('fah-viewer')
    env.CBDefine('GLEW_STATIC')
    env.CBDefine('USING_CBANG') # Using CBANG macro namespace

    pkg_config = '"%s"' % os.environ.get('PKG_CONFIG', 'pkg-config')

    # X
    if not win32:
        conf.CBRequireHeader('X11/X.h')
        conf.CBRequireHeader('X11/Xlib.h')
        conf.CBRequireLib('X11')

    # Win32
    if win32:
        conf.CBRequireLib('scrnsave')
        conf.CBRequireLib('comctl32')

    if not win32:
        env.Append(PREFER_DYNAMIC = 'bz2 z m GLU glut X11'.split())

    env.CBConfConsole() # Build console app on Windows

conf.Finish()

# Main program
Export('env win32')
prog, lib = \
    SConscript('src/FAHScreensaver.scons', variant_dir = 'build', duplicate = 0)
Default(prog)

# Clean
Clean(prog, ['build', 'config.log'])

# Dist
docs = ['README.md', 'AUTHORS', 'CHANGELOG.md', 'copyright']
distfiles = docs + [prog, 'src', 'SConstruct']
tar = env.TarBZ2Dist('FAHScreensaver', distfiles)
Alias('dist', tar)
AlwaysBuild(tar)

# Package
description = \
'''Folding@home is a distributed computing project using volunteered
computer resources.

This package contains the 3D screensaver.  It can connect to
local or remote FAHClients and visualize the running simulations.'''

if 'package' in COMMAND_LINE_TARGETS:
    # Don't package Windows here
    if win32:
        f = open('package.txt', 'w');
        f.write('none');
        f.close()
        Exit(0)

    # Package
    pkg = env.Packager(
        'FAHScreensaver',

        version = version,
        maintainer = 'Joseph Coffland <joseph@cauldrondevelopment.com>',
        vendor = 'Folding@home',
        url = 'https://foldingathome.org/',
        license = 'copyright',
        bug_url = 'https://apps.foldingathome.org/bugs',
        summary = 'Folding@home 3D Screensaver',
        description = description,
        prefix = '/usr',
        copyright = 'Copyright 2010-2018, foldingathome.org',

        documents = docs,
        programs = [str(prog[0])],
        changelog = 'CHANGELOG.md',

        deb_directory = 'debian',
        deb_section = 'science',
        deb_depends = \
            'libx11-6, libc6, bzip2, zlib1g, libexpat1, libgl1-mesa-glx, ' \
            'libglu1, freeglut3, xscreensaver',
        deb_priority = 'optional',
        deb_recommends = 'fahclient (=%s), fahcontrol (=%s), fahviewer (=%s)' \
            % (version, version, version),

        rpm_license = 'GPL+2',
        rpm_group = 'Applications/Multimedia',
        rpm_requires = 'libX11, mesa-libGL, expat, bzip2-libs, freeglut, ' + \
            'mesa-libGLU, xscreensaver',
        rpm_build = 'rpm/build',

        app_id = 'org.foldingathome.fahscreensaver',
        app_resources = [['osx/Resources/', '.']],
        pkg_resources = [['osx/Resources/', '.']],
        app_signature = '????',
        app_other_info = {'CFBundleIconFile': 'FAHScreensaver.icns'},
        app_programs = [str(prog[0])],
        pkg_files = [['osx/FAHScreensaver', 'usr/bin/', 0755]],
        )

    AlwaysBuild(pkg)
    env.Alias('package', pkg)
