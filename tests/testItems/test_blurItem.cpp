/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     Zhang Hao <zhanghao@uniontech.com>
 *
 * Maintainer: WangYu <wangyu@uniontech.com>
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
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#define protected public
#define private public
#include "cgraphicsview.h"
#include <qaction.h>
#undef protected
#undef private
#include "ccentralwidget.h"
#include "clefttoolbar.h"
#include "toptoolbar.h"
#include "frame/cgraphicsview.h"
#include "drawshape/cdrawscene.h"
#include "drawshape/cdrawparamsigleton.h"
#include "drawshape/drawItems/cgraphicsitemselectedmgr.h"
#include "application.h"

#include "crecttool.h"
#include "ccuttool.h"
#include "cellipsetool.h"
#include "cmasicotool.h"
#include "cpentool.h"
#include "cpolygonalstartool.h"
#include "cpolygontool.h"
#include "ctexttool.h"
#include "ctriangletool.h"

#include <DFloatingButton>
#include <DComboBox>
#include <dzoommenucombobox.h>
#include "cspinbox.h"

#include "cpictureitem.h"
#include "cgraphicsrectitem.h"
#include "cgraphicsellipseitem.h"
#include "cgraphicstriangleitem.h"
#include "cgraphicspolygonalstaritem.h"
#include "cgraphicspolygonitem.h"
#include "cgraphicslineitem.h"
#include "cgraphicspenitem.h"
#include "cgraphicstextitem.h"
#include "cgraphicscutitem.h"

#include <QDebug>
#include <DLineEdit>

#include "publicApi.h"

#if TEST_BLUR_ITEM

TEST(BlurItem, TestBlurItemCreateView)
{
    int i = 0;
    while (i++ < 50) {
        QTest::qWait(200);
        if (getCurView() != nullptr) {
            break;
        }
    }
    if (getCurView() == nullptr) {
        qDebug() << __FILE__ << __LINE__ << "get CGraphicsView is nullptr.";
    }
    ASSERT_NE(getCurView(), nullptr);

    CGraphicsView *view = getCurView();
    ASSERT_NE(view, nullptr);

    // 打开ddf
    QFile file(":/test.ddf");
    ASSERT_EQ(true, file.open(QIODevice::ReadOnly));
    QByteArray data =  file.readAll();
    file.close();

    QString BlurItemPath = QApplication::applicationDirPath() + "/test.ddf";
    QFile bfile(BlurItemPath);
    bfile.open(QIODevice::WriteOnly);
    ASSERT_TRUE(bfile.write(data));
    QTest::qWait(200);
    bfile.close();

    QMimeData mimedata;
    QList<QUrl> li;
    li.append(QUrl(BlurItemPath));
    mimedata.setUrls(li);

    const QPoint pos = view->viewport()->rect().center();
    QDragEnterEvent eEnter(pos, Qt::IgnoreAction, &mimedata, Qt::LeftButton, Qt::NoModifier);
    dApp->sendEvent(view->viewport(), &eEnter);

    QDropEvent e(pos, Qt::IgnoreAction, &mimedata, Qt::LeftButton, Qt::NoModifier);
    dApp->sendEvent(view->viewport(), &e);
    QTest::qWait(100);
}


TEST(BlurItem, TestDrawBlurItem)
{
    CGraphicsView *view = getCurView();
    ASSERT_NE(view, nullptr);
    CCentralwidget *c = getMainWindow()->getCCentralwidget();
    ASSERT_NE(c, nullptr);

    QToolButton *tool = c->getLeftToolBar()->findChild<QToolButton *>("Blur tool button");
    ASSERT_NE(tool, nullptr);
    tool->clicked();

    int addedCount = view->drawScene()->getBzItems().count();
    // 模糊图元不支持alt进行复制
    createItemByMouse(view, true);
    ASSERT_EQ(view->drawScene()->getBzItems().count(), addedCount);
    ASSERT_EQ(view->drawScene()->getBzItems().first()->type(), BlurType);

}
TEST(BlurItem, TestCopyBlurItem)
{
    keyShortCutCopyItem();
}

TEST(BlurItem, TestBlurItemProperty)
{
    CGraphicsView *view = getCurView();
    ASSERT_NE(view, nullptr);

    CGraphicsMasicoItem *blur = dynamic_cast<CGraphicsMasicoItem *>(view->drawScene()->getBzItems().first());
    ASSERT_NE(blur, nullptr);

    // Blur Blur Type
    DToolButton *btn = drawApp->topToolbar()->findChild<DToolButton *>("Blur type button");
    ASSERT_NE(btn, nullptr);
    btn->released();
    QTest::qWait(100);
    ASSERT_EQ(blur->getBlurEffect(), 0);

    // Blur Masic Type
    btn = drawApp->topToolbar()->findChild<DToolButton *>("Masic type button");
    ASSERT_NE(btn, nullptr);
    btn->released();
    QTest::qWait(100);
    ASSERT_EQ(blur->getBlurEffect(), 1);

    // Blur width
    CSpinBox *sp = drawApp->topToolbar()->findChild<CSpinBox *>("BlurPenWidth");
    ASSERT_NE(sp, nullptr);
    sp->setValue(100);
    QTest::qWait(100);
    ASSERT_EQ(blur->getBlurWidth(), sp->value());
    sp->lineEdit()->setText("300");
    sp->lineEdit()->editingFinished();
    QTest::qWait(100);
    ASSERT_EQ(blur->getBlurWidth(), sp->value());
}

TEST(BlurItem, TestResizeBlurItem)
{
    resizeItem();
}

TEST(BlurItem, TestSelectAllBlurItem)
{
    CGraphicsView *view = getCurView();
    ASSERT_NE(view, nullptr);

    // 全选图元
    DTestEventList e;
    e.addMouseMove(QPoint(20, 20), 100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(10, 10), 100);
    e.addMouseMove(QPoint(1800, 900), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(1000, 1000), 100);
    e.addKeyPress(Qt::Key_A, Qt::ControlModifier, 100);
    e.addKeyRelease(Qt::Key_A, Qt::ControlModifier, 100);
    e.simulate(view->viewport());

    // 水平等间距对齐
    view->m_itemsVEqulSpaceAlign->triggered(true);
    // 垂直等间距对齐
    view->m_itemsHEqulSpaceAlign->triggered(true);

    //滚轮事件
    QWheelEvent wheelevent(QPointF(1000, 1000), 100, Qt::MouseButton::NoButton, Qt::KeyboardModifier::ControlModifier);
    view->wheelEvent(&wheelevent);
    QWheelEvent wheelevent2(QPointF(1000, 1000), 100, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier);
    view->wheelEvent(&wheelevent2);
    QWheelEvent wheelevent3(QPointF(1000, 1000), 100, Qt::MouseButton::NoButton, Qt::KeyboardModifier::ShiftModifier);
    view->wheelEvent(&wheelevent3);
}

TEST(BlurItem, TestLayerChange)
{
    layerChange();
}

TEST(BlurItem, TestGroupUngroup)
{
    groupUngroup();
}


TEST(BlurItem, TestSaveBlurItemToFile)
{
    CGraphicsView *view = getCurView();
    ASSERT_NE(view, nullptr);
    CCentralwidget *c = getMainWindow()->getCCentralwidget();
    ASSERT_NE(c, nullptr);

    // save ddf file
    QString BlurItemPath = QApplication::applicationDirPath() + "/test_blur.ddf";
    QFile file(BlurItemPath);
    file.open(QIODevice::ReadWrite);
    file.close();
    view->getDrawParam()->setDdfSavePath(BlurItemPath);
    c->slotSaveToDDF(true);
    QTest::qWait(100);

    QFileInfo info(BlurItemPath);
    ASSERT_TRUE(info.exists());
}

TEST(BlurItem, TestOpenBlurItemFromFile)
{
    CGraphicsView *view = getCurView();
    ASSERT_NE(view, nullptr);

    // 打开保存绘制的 ddf
    QString BlurItemPath = QApplication::applicationDirPath() + "/test_blur.ddf";

    QMimeData mimedata;
    QList<QUrl> li;
    li.append(QUrl(BlurItemPath));
    mimedata.setUrls(li);

    const QPoint pos = view->viewport()->rect().center();
    QDragEnterEvent eEnter(pos, Qt::IgnoreAction, &mimedata, Qt::LeftButton, Qt::NoModifier);
    dApp->sendEvent(view->viewport(), &eEnter);

    QDropEvent e(pos, Qt::IgnoreAction, &mimedata, Qt::LeftButton, Qt::NoModifier);
    dApp->sendEvent(view->viewport(), &e);
    QTest::qWait(100);

//    view = getCurView();
//    ASSERT_NE(view, nullptr);
//    int addedCount = view->drawScene()->getBzItems(view->drawScene()->items()).count();
//    ASSERT_EQ(true, addedCount == 2 ? true : false);
}

#endif
