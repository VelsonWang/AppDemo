#include "coreplugin.h"
#include <coreplugin/mainwindow.h>
#include <app/app_version.h>
#include <extensionsystem/pluginerroroverview.h>
#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>
#include <utils/algorithm.h>
#include <utils/pathchooser.h>
#include <utils/macroexpander.h>
#include <utils/mimetypes/mimedatabase.h>
#include <utils/savefile.h>
#include <utils/stringutils.h>
#include <utils/theme/theme.h>
#include <utils/theme/theme_p.h>

#include <QtPlugin>

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QMenu>
#include <QUuid>

#include <cstdlib>

using namespace Core;
using namespace Core::Internal;
using namespace Utils;

static CorePlugin *m_instance = nullptr;

CorePlugin::CorePlugin()
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    //qRegisterMetaType<Id>();
    m_instance = this;
}

CorePlugin::~CorePlugin()
{
    delete m_mainWindow;
}

CorePlugin *CorePlugin::instance()
{
    return m_instance;
}


bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;

    m_mainWindow = new MainWindow;
    std::srand(unsigned(QDateTime::currentDateTime().toSecsSinceEpoch()));
    m_mainWindow->init();

    return true;
}

void CorePlugin::extensionsInitialized()
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;

    m_mainWindow->extensionsInitialized();
    if (ExtensionSystem::PluginManager::hasError()) {
        auto errorOverview = new ExtensionSystem::PluginErrorOverview(m_mainWindow);
        errorOverview->setAttribute(Qt::WA_DeleteOnClose);
        errorOverview->setModal(true);
        errorOverview->show();
    }
}

bool CorePlugin::delayedInitialize()
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    return true;
}

QObject *CorePlugin::remoteCommand(const QStringList & /* options */,
                                   const QString &workingDirectory,
                                   const QStringList &args)
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;

    if (!ExtensionSystem::PluginManager::isInitializationDone()) {
        connect(ExtensionSystem::PluginManager::instance(), &ExtensionSystem::PluginManager::initializationDone,
                this, [this, workingDirectory, args]() {
                    remoteCommand(QStringList(), workingDirectory, args);
        });
        return nullptr;
    }

    m_mainWindow->raiseWindow();

    return new QObject();
}

void CorePlugin::fileOpenRequest(const QString &f)
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    remoteCommand(QStringList(), QString(), QStringList(f));
}


ExtensionSystem::IPlugin::ShutdownFlag CorePlugin::aboutToShutdown()
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    m_mainWindow->aboutToShutdown();
    return SynchronousShutdown;
}
