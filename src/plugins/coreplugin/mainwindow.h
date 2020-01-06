
#pragma once

#include <utils/appmainwindow.h>
#include <utils/dropsupport.h>

#include <QMap>
#include <QColor>

#include <functional>

QT_BEGIN_NAMESPACE
class QPrinter;
class QToolButton;
QT_END_NAMESPACE

namespace Core {

namespace Internal {

class MainWindow : public Utils::AppMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override;

    void init();
    void extensionsInitialized();
    void aboutToShutdown();

    void saveSettings();

public slots:
    void exit();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void openDroppedFiles(const QList<Utils::DropSupport::FileSpec> &files);
    void restoreWindowState();

    void readSettings();
    void saveWindowSettings();

};

} // namespace Internal
} // namespace Core
