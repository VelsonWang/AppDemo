include(AppDemo.pri)

#version check qt
!minQtVersion(5, 9, 0) {
    message("Cannot build $$IDE_DISPLAY_NAME with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.9.0.")
}

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS = src

contains(QT_ARCH, i386): ARCHITECTURE = x86
else: ARCHITECTURE = $$QT_ARCH

macx: PLATFORM = "mac"
else:win32: PLATFORM = "windows"
else:linux-*: PLATFORM = "linux-$${ARCHITECTURE}"
else: PLATFORM = "unknown"

BASENAME = $$(INSTALL_BASENAME)
isEmpty(BASENAME): BASENAME = qt-creator-$${PLATFORM}$(INSTALL_EDITION)-$${QTCREATOR_VERSION}$(INSTALL_POSTFIX)

linux {
    appstream.files = dist/org.qt-project.AppDemo.appdata.xml
    appstream.path = $$QTC_PREFIX/share/metainfo/

    desktop.files = dist/org.qt-project.AppDemo.desktop
    desktop.path = $$QTC_PREFIX/share/applications/

    INSTALLS += appstream desktop
}

macx {
    APPBUNDLE = "$$OUT_PWD/bin/$${IDE_APP_TARGET}.app"
    BINDIST_SOURCE = "$$OUT_PWD/bin/$${IDE_APP_TARGET}.app"
    deployqt.commands = $$PWD/scripts/deployqtHelper_mac.sh \"$${APPBUNDLE}\" \"$$[QT_INSTALL_BINS]\" \"$$[QT_INSTALL_TRANSLATIONS]\" \"$$[QT_INSTALL_PLUGINS]\" \"$$[QT_INSTALL_IMPORTS]\" \"$$[QT_INSTALL_QML]\"
    codesign.commands = codesign --deep -s \"$(SIGNING_IDENTITY)\" $(SIGNING_FLAGS) \"$${APPBUNDLE}\"
    dmg.commands = python -u \"$$PWD/scripts/makedmg.py\" \"$${BASENAME}.dmg\" \"Qt Creator\" \"$$IDE_SOURCE_TREE\" \"$$OUT_PWD/bin\"
    #dmg.depends = deployqt
    QMAKE_EXTRA_TARGETS += codesign dmg
} else {
    BINDIST_SOURCE = "$(INSTALL_ROOT)$$QTC_PREFIX"
    BINDIST_EXCLUDE_ARG = "--exclude-toplevel"
    deployqt.commands = python -u $$PWD/scripts/deployqt.py -i \"$(INSTALL_ROOT)$$QTC_PREFIX/bin/$${IDE_APP_TARGET}\" \"$(QMAKE)\"
    deployqt.depends = install
    win32 {
        deployartifacts.depends = install
        deployartifacts.commands = git clone --depth 1 -b $$BINARY_ARTIFACTS_BRANCH \
                "http://code.qt.io/qt-creator/binary-artifacts.git" \
            && xcopy /s /q /y /i "binary-artifacts\\win32" \"$(INSTALL_ROOT)$$QTC_PREFIX\" \
            && rmdir /s /q binary-artifacts
        QMAKE_EXTRA_TARGETS += deployartifacts
    }
}

INSTALLER_ARCHIVE_FROM_ENV = $$(INSTALLER_ARCHIVE)
isEmpty(INSTALLER_ARCHIVE_FROM_ENV) {
    INSTALLER_ARCHIVE = $$OUT_PWD/$${BASENAME}-installer-archive.7z
} else {
    INSTALLER_ARCHIVE = $$OUT_PWD/$$(INSTALLER_ARCHIVE)
}

INSTALLER_ARCHIVE_DEBUG = $$INSTALLER_ARCHIVE
INSTALLER_ARCHIVE_DEBUG ~= s/(.*)[.]7z/\1-debug.7z

bindist.commands = python -u $$PWD/scripts/createDistPackage.py $$OUT_PWD/$${BASENAME}.7z \"$$BINDIST_SOURCE\"
bindist_installer.commands = python -u $$PWD/scripts/createDistPackage.py $$BINDIST_EXCLUDE_ARG $${INSTALLER_ARCHIVE} \"$$BINDIST_SOURCE\"
bindist_debug.commands = python -u $$PWD/scripts/createDistPackage.py --debug $$BINDIST_EXCLUDE_ARG $${INSTALLER_ARCHIVE_DEBUG} \"$$BINDIST_SOURCE\"

win32 {
    deployqt.commands ~= s,/,\\\\,g
    bindist.commands ~= s,/,\\\\,g
    bindist_installer.commands ~= s,/,\\\\,g
}

QMAKE_EXTRA_TARGETS += deployqt bindist bindist_installer bindist_debug
