/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     WangXin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "application.h"
#include "frame/mainwindow.h"
#include "frame/cviewmanagement.h"
#include "service/dbusdraw_adaptor.h"

#include <QFileInfo>
#include <QDBusConnection>
#include <DApplicationSettings>

#include <DGuiApplicationHelper>
#include <DApplicationSettings>

#include <DLog>

Application::Application(int &argc, char **argv)
    : QtSingleApplication(argc, argv)
{
    initI18n();

    dApp->setQuitOnLastWindowClosed(true);

    //绑定主题发生变化的信号
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &Application::onThemChanged);

}

int Application::execDraw(const QStringList &paths, QString &glAppPath)
{
    using namespace Dtk::Core;
    Dtk::Core::DLogManager::registerConsoleAppender();
    Dtk::Core::DLogManager::registerFileAppender();

    //判断实例是否已经运行
    if (this->isRunning()) {
        qDebug() << "deepin-draw is already running";
        for (int i = 0; i < paths.count(); i++) {
            this->sendMessage(paths.at(i), 2000); //1s后激活前个实例
        }
        return EXIT_SUCCESS;
    }

    static const QDate buildDate = QLocale( QLocale::English )
                                   .toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
    QString t_date = buildDate.toString("MMdd");

    // Version Time
    this->setApplicationVersion(DApplication::buildVersion(t_date));

    //主题设置
    glAppPath = QDir::homePath() + QDir::separator() + "." + qApp->applicationName();
    QDir t_appDir;
    t_appDir.mkpath(glAppPath);


    this->setOrganizationName("deepin");
    this->setApplicationName("deepin-draw");
    this->setQuitOnLastWindowClosed(true);

    // 应用已保存的主题设置
    //DGuiApplicationHelper::ColorType type = getThemeTypeSetting();
    DApplicationSettings saveTheme;
    CManageViewSigleton::GetInstance()->setThemeType(DGuiApplicationHelper::instance()->themeType());

    showMainWindow(paths);

    return exec();
}

void Application::onMessageRecived(const QString &message)
{
    MainWindow *pWin = dynamic_cast<MainWindow *>(this->activationWindow());

    if (pWin != nullptr) {

        QFileInfo info(message);

        if (info.isFile()) {
            pWin->slotLoadDragOrPasteFile(QStringList() << message);
        }
    }
}

void Application::onThemChanged(DGuiApplicationHelper::ColorType themeType)
{
    MainWindow *pWin = dynamic_cast<MainWindow *>(this->activationWindow());

    if (pWin != nullptr) {
        pWin->slotOnThemeChanged(themeType);
    }
}

void Application::showMainWindow(const QStringList &paths)
{
#ifdef DEBUSVAILD
    MainWindow *w = new MainWindow;

    this->setActivationWindow(w, true);

    //以dbus的方式传递命令
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerService("com.deepin.Draw");
    dbus.registerObject("/com/deepin/Draw", w);
    new dbusdraw_adaptor(w);
#else
    MainWindow *w = new MainWindow(paths);

    this->setActivationWindow(w, true);

    //如果没有通过dbus的方式进行进程通信那么这里要绑定指令
    connect(this, &Application::messageReceived, this, &Application::onMessageRecived, Qt::QueuedConnection);
#endif

    w->initScene();
    w->readSettings();
    w->show();
}

void Application::handleQuitAction()
{
    emit popupConfirmDialog();
}

void Application::initI18n()
{
    loadTranslator(QList<QLocale>() << QLocale::system());
}
