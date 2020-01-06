include(../AppDemo.pri)

TEMPLATE  = subdirs
CONFIG   += ordered

QBS_DIRS = \
    qbscorelib \
    qbsqtprofilesetup \
    qbsapps \
    qbslibexec \
    qbsplugins \
    qbsstatic

qbscorelib.subdir = shared/qbs/src/lib/corelib
qbsqtprofilesetup.subdir = shared/qbs/src/lib/qtprofilesetup
qbsqtprofilesetup.depends = qbscorelib
qbsapps.subdir = shared/qbs/src/app
qbsapps.depends = qbsqtprofilesetup
qbslibexec.subdir = shared/qbs/src/libexec
qbslibexec.depends = qbscorelib
qbsplugins.subdir = shared/qbs/src/plugins
qbsstatic.file = shared/qbs/static.pro


SUBDIRS += \
    libs \
    app \
    plugins
