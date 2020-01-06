
#include "introductionwidget.h"
#include <extensionsystem/iplugin.h>
#include <extensionsystem/pluginmanager.h>
#include <app/app_version.h>
#include <utils/algorithm.h>
#include <utils/icon.h>
#include <utils/fileutils.h>
#include <utils/hostosinfo.h>
#include <utils/qtcassert.h>
#include <utils/styledbar.h>
#include <utils/treemodel.h>
#include <utils/theme/theme.h>
#include <QWidget>
#include <QLabel>
#include <QPalette>
#include <QVBoxLayout>

using namespace Core;
using namespace ExtensionSystem;
using namespace Utils;

namespace Welcome {
namespace Internal {

class SideBar;

const int lrPadding = 34;
const char currentPageSettingsKeyC[] = "Welcome2Tab";

static QColor themeColor(Theme::Color role)
{
    return Utils::creatorTheme()->color(role);
}

static QFont sizedFont(int size, const QWidget *widget, bool underline = false)
{
    QFont f = widget->font();
    f.setPixelSize(size);
    f.setUnderline(underline);
    return f;
}

static QPalette lightText()
{
    QPalette pal;
    pal.setColor(QPalette::Foreground, themeColor(Theme::Welcome_ForegroundPrimaryColor));
    pal.setColor(QPalette::WindowText, themeColor(Theme::Welcome_ForegroundPrimaryColor));
    return pal;
}

static void addWeakVerticalSpacerToLayout(QVBoxLayout *layout, int maximumSize)
{
    auto weakSpacer = new QWidget;
    weakSpacer->setMaximumHeight(maximumSize);
    weakSpacer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum);
    layout->addWidget(weakSpacer);
    layout->setStretchFactor(weakSpacer, 1);
}


class WelcomePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Welcome.json")

public:
    ~WelcomePlugin() final { delete m_pWidget; }

    bool initialize(const QStringList &, QString *) final
    {
        m_pWidget = new QWidget;
        QLabel *pLabel = new QLabel(m_pWidget);
        pLabel->setText("WelcomePlugin");
        m_pWidget->resize(320, 240);
        return true;
    }

    void extensionsInitialized() final
    {
        m_pWidget->show();

    }

    QWidget *m_pWidget = nullptr;
};


} // namespace Internal
} // namespace Welcome

#include "welcomeplugin.moc"
