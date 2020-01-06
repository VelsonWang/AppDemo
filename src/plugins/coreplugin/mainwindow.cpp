#include "mainwindow.h"
#include <app/app_version.h>
#include <extensionsystem/pluginmanager.h>
#include <utils/algorithm.h>
#include <utils/historycompleter.h>
#include <utils/hostosinfo.h>
#include <utils/mimetypes/mimedatabase.h>
#include <utils/qtcassert.h>
#include <utils/stylehelper.h>
#include <utils/theme/theme.h>
#include <utils/stringutils.h>
#include <utils/utilsicons.h>

#include <QApplication>
#include <QCloseEvent>
#include <QColorDialog>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMenu>
#include <QMenuBar>
#include <QPrinter>
#include <QSettings>
#include <QStatusBar>
#include <QStyleFactory>
#include <QTimer>
#include <QToolButton>
#include <QUrl>

using namespace ExtensionSystem;
using namespace Utils;

namespace Core {
namespace Internal {

enum { debugMainWindow = 0 };

MainWindow::MainWindow() : AppMainWindow()
{
    HistoryCompleter::setSettings(PluginManager::settings());

    setWindowTitle(Constants::IDE_DISPLAY_NAME);
    QCoreApplication::setApplicationName(QLatin1String(Constants::IDE_CASED_ID));
    QCoreApplication::setApplicationVersion(QLatin1String(Constants::IDE_VERSION_LONG));
    QCoreApplication::setOrganizationName(QLatin1String(Constants::IDE_SETTINGSVARIANT_STR));
    QString baseName = QApplication::style()->objectName();
    // Sometimes we get the standard windows 95 style as a fallback
    if (HostOsInfo::isAnyUnixHost() && !HostOsInfo::isMacHost()
            && baseName == QLatin1String("windows")) {
        baseName = QLatin1String("fusion");
    }


    setDockNestingEnabled(true);

    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);

    // Add small Toolbuttons for toggling the navigation widgets
    int childsCount = statusBar()->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly).count();

    statusBar()->setProperty("p_styled", true);

    auto dropSupport = new DropSupport(this, [](QDropEvent *event, DropSupport *) {
        return event->source() == nullptr; // only accept drops from the "outside" (e.g. file manager)
    });
    connect(dropSupport, &DropSupport::filesDropped,
            this, &MainWindow::openDroppedFiles);
}

MainWindow::~MainWindow()
{

}

void MainWindow::init()
{

}

void MainWindow::extensionsInitialized()
{
    readSettings();
    // Delay restoreWindowState, since it is overridden by LayoutRequest event
    QTimer::singleShot(0, this, &MainWindow::restoreWindowState);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // work around QTBUG-43344
    static bool alreadyClosed = false;
    if (alreadyClosed) {
        event->accept();
        return;
    }

    saveWindowSettings();

    event->accept();
    alreadyClosed = true;
}

void MainWindow::openDroppedFiles(const QList<DropSupport::FileSpec> &files)
{
    raiseWindow();
    QStringList filePaths = Utils::transform(files, &DropSupport::FileSpec::filePath);
    //openFiles(filePaths, ICore::SwitchMode);
}


void MainWindow::exit()
{
    // this function is most likely called from a user action
    // that is from an event handler of an object
    // since on close we are going to delete everything
    // so to prevent the deleting of that object we
    // just append it
    QTimer::singleShot(0, this,  &QWidget::close);
}


void MainWindow::aboutToShutdown()
{
    hide();
}

static const char settingsGroup[] = "MainWindow";
static const char colorKey[] = "Color";
static const char windowGeometryKey[] = "WindowGeometry";
static const char windowStateKey[] = "WindowState";
static const char modeSelectorLayoutKey[] = "ModeSelectorLayout";

void MainWindow::readSettings()
{
    QSettings *settings = PluginManager::settings();
    settings->beginGroup(QLatin1String(settingsGroup));

    settings->endGroup();
}

void MainWindow::saveSettings()
{
    QSettings *settings = PluginManager::settings();
    settings->beginGroup(QLatin1String(settingsGroup));

    settings->endGroup();
}

void MainWindow::saveWindowSettings()
{
    QSettings *settings = PluginManager::settings();
    settings->beginGroup(QLatin1String(settingsGroup));

    // On OS X applications usually do not restore their full screen state.
    // To be able to restore the correct non-full screen geometry, we have to put
    // the window out of full screen before saving the geometry.
    // Works around QTBUG-45241
    if (Utils::HostOsInfo::isMacHost() && isFullScreen())
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    settings->setValue(QLatin1String(windowGeometryKey), saveGeometry());
    settings->setValue(QLatin1String(windowStateKey), saveState());

    settings->endGroup();
}


void MainWindow::restoreWindowState()
{
    QSettings *settings = PluginManager::settings();
    settings->beginGroup(QLatin1String(settingsGroup));
    if (!restoreGeometry(settings->value(QLatin1String(windowGeometryKey)).toByteArray()))
        resize(1260, 700); // size without window decoration
    restoreState(settings->value(QLatin1String(windowStateKey)).toByteArray());
    settings->endGroup();
    show();
}

} // namespace Internal
} // namespace Core
