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
#include "textcolorbutton.h"

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

#if TEST_TEXT_ITEM

TEST(TextItem, TestTextItemCreateView)
{
    createNewViewByShortcutKey();
}

TEST(TextItem, TestDrawTextItem)
{
    CGraphicsView *view = getCurView();
    ASSERT_NE(view, nullptr);
    CCentralwidget *c = getMainWindow()->getCCentralwidget();
    ASSERT_NE(c, nullptr);

    QToolButton *tool = nullptr;
    tool = c->getLeftToolBar()->findChild<QToolButton *>("Text tool button");
    ASSERT_NE(tool, nullptr);
    tool->clicked();

    int addedCount = view->drawScene()->getBzItems().count();
    createItemByMouse(view);
    ASSERT_EQ(view->drawScene()->getBzItems().count(), addedCount + 1);
    ASSERT_EQ(view->drawScene()->getBzItems().first()->type(), TextType);
}

TEST(TextItem, TestCopyTextItem)
{
    drawApp->topMainWindow()->setFocus();
    keyShortCutCopyItem();
}


TEST(TextItem, TestTextItemProperty)
{
    CGraphicsView *view = getCurView();
    ASSERT_NE(view, nullptr);
    CGraphicsTextItem *text = dynamic_cast<CGraphicsTextItem *>(view->drawScene()->getBzItems().first());
    ASSERT_NE(text, nullptr);

    // Font color
    QColor color(Qt::red);
    TextColorButton *stroke = drawApp->topToolbar()->findChild<TextColorButton *>("Text color button");
    stroke->setColor(color);
    QTest::qWait(100);
    ASSERT_EQ(text->textColor(), color);

    // Font Family Type
    DComboBox *typeCombox = drawApp->topToolbar()->findChild<DComboBox *>("Text font family comboBox");
    ASSERT_NE(typeCombox, nullptr);
    QString family = "Bitstream Charter";//Andale Mono
    typeCombox->activated(family);
    QTest::qWait(100);
    ASSERT_EQ(text->fontFamily(), family);

    // Font Style Type
    typeCombox = drawApp->topToolbar()->findChild<DComboBox *>("Text font style comboBox");
    ASSERT_NE(typeCombox, nullptr);
    QString style = "Bold";
    typeCombox->setCurrentText(style);
    QTest::qWait(100);
    ASSERT_EQ(text->fontStyle(), style);

    // Font Size Type
    typeCombox = drawApp->topToolbar()->findChild<DComboBox *>("Text font size comboBox");
    ASSERT_NE(typeCombox, nullptr);
    QString size = "61p";
    typeCombox->lineEdit()->setText(size);
    typeCombox->lineEdit()->returnPressed();
    QTest::qWait(100);
    ASSERT_EQ(text->fontSize(), 61);


    view->drawScene()->clearSelectGroup();
    view->drawScene()->selectItem(text);

    DTestEventList e;

    QPoint posDClick = view->mapFromScene(text->mapToScene(text->boundingRect().center()));
    e.addMouseMove(posDClick, 100);
    e.addDelay(100);
    e.simulate(view->viewport());

    static_cast<CGraphicsTextItem *>(text)->setTextState(CGraphicsTextItem::EInEdit);

    QContextMenuEvent event(QContextMenuEvent::Mouse, QPoint(100, 100));
    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 270), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 270), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());

    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 250), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 250), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());

    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 290), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 290), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());

    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 100), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 100), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());

    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 45), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 45), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());

    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 10), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 10), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());

    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 80), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 80), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());

    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 100), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 100), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());

    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 130), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 130), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());
    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 170), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 170), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());

    dApp->sendEvent(view->viewport(), &event);
    e.clear();
    e.addDelay(100);
    e.addMousePress(Qt::LeftButton, Qt::NoModifier, QPoint(28, 200), 100);
    e.addMouseRelease(Qt::LeftButton, Qt::NoModifier, QPoint(28, 200), 100);
    e.addDelay(100);
    e.simulate(QApplication::activePopupWidget());

    e.addKeyPress(Qt::Key_L, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addKeyRelease(Qt::Key_L, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addDelay(100);
    e.addKeyPress(Qt::Key_R, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addKeyRelease(Qt::Key_R, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addDelay(100);
    e.addKeyPress(Qt::Key_T, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addKeyRelease(Qt::Key_T, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addDelay(100);
    e.addKeyPress(Qt::Key_B, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addKeyRelease(Qt::Key_B, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addDelay(100);
    e.addKeyPress(Qt::Key_V, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addKeyRelease(Qt::Key_V, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addDelay(100);
    e.addKeyPress(Qt::Key_H, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addKeyRelease(Qt::Key_H, Qt::ControlModifier | Qt::ShiftModifier, 100);
    e.addDelay(100);
    e.addKeyPress(Qt::Key_Z, Qt::ControlModifier, 100);
    e.addKeyRelease(Qt::Key_Z, Qt::ControlModifier, 100);
    e.addDelay(100);
    e.addKeyPress(Qt::Key_Y, Qt::ControlModifier, 100);
    e.addKeyRelease(Qt::Key_Y, Qt::ControlModifier, 100);
    e.addDelay(100);
    e.simulate(view->viewport());


    view->drawScene()->clearSelectGroup();
    view->drawScene()->selectItem(text);

    QVector<CSizeHandleRect *> handles = view->drawScene()->selectGroup()->handleNodes();

    int delay = 50;

    // 普通拉伸
    for (int i = 0; i < handles.size(); ++i) {
        CSizeHandleRect *pNode = handles[i];
        QPoint posInView = view->mapFromScene(pNode->mapToScene(pNode->boundingRect().center()));
        DTestEventList e;
        e.addMouseMove(posInView, delay);
        e.addMousePress(Qt::LeftButton, Qt::ShiftModifier, posInView, delay);
        e.addMouseMove(posInView + QPoint(20, 20), delay);
        e.addMouseRelease(Qt::LeftButton, Qt::ShiftModifier, posInView + QPoint(20, 20), delay);
        e.simulate(view->viewport());

        //  Undo Redo
        e.clear();
        e.addKeyPress(Qt::Key_Z, Qt::ControlModifier, delay);
        e.addKeyRelease(Qt::Key_Z, Qt::ControlModifier, delay);
        e.addDelay(100);
        e.clear();
        e.addKeyPress(Qt::Key_Y, Qt::ControlModifier, delay);
        e.addKeyRelease(Qt::Key_Y, Qt::ControlModifier, delay);
        e.simulate(view->viewport());
    }

    // SHIFT   ALT拉伸:  QTestEvent mouseMove 中移动鼠标的实现是直接设置全局鼠标位置 5.15中解决了此问题
    for (int i = 0; i < handles.size(); ++i) {
        CSizeHandleRect *pNode = handles[i];
        QPoint posInView = view->mapFromScene(pNode->mapToScene(pNode->boundingRect().center()));
        QMouseEvent mouseEvent(QEvent::MouseButtonPress, posInView, Qt::LeftButton, Qt::LeftButton, Qt::ShiftModifier);
        QApplication::sendEvent(view->viewport(), &mouseEvent);
        QTest::qWait(delay);
        QMouseEvent mouseEvent1(QEvent::MouseMove, posInView + QPoint(20, 20), Qt::LeftButton, Qt::LeftButton, Qt::ShiftModifier);
        QApplication::sendEvent(view->viewport(), &mouseEvent1);
        QTest::qWait(delay);
    }
    for (int i = 0; i < handles.size(); ++i) {
        CSizeHandleRect *pNode = handles[i];
        QPoint posInView = view->mapFromScene(pNode->mapToScene(pNode->boundingRect().center()));
        QMouseEvent mouseEvent(QEvent::MouseButtonPress, posInView, Qt::LeftButton, Qt::LeftButton, Qt::AltModifier);
        QApplication::sendEvent(view->viewport(), &mouseEvent);
        QTest::qWait(delay);
        QMouseEvent mouseEvent1(QEvent::MouseMove, posInView - QPoint(20, 20), Qt::LeftButton, Qt::LeftButton, Qt::AltModifier);
        QApplication::sendEvent(view->viewport(), &mouseEvent1);
        QTest::qWait(delay);
    }
    for (int i = 0; i < handles.size(); ++i) {
        CSizeHandleRect *pNode = handles[i];
        QPoint posInView = view->mapFromScene(pNode->mapToScene(pNode->boundingRect().center()));
        QMouseEvent mouseEvent(QEvent::MouseButtonPress, posInView, Qt::LeftButton, Qt::LeftButton, Qt::ShiftModifier | Qt::AltModifier);
        QApplication::sendEvent(view->viewport(), &mouseEvent);
        QTest::qWait(delay);
        QMouseEvent mouseEvent1(QEvent::MouseMove, posInView + QPoint(20, 20), Qt::LeftButton, Qt::LeftButton, Qt::ShiftModifier | Qt::AltModifier);
        QApplication::sendEvent(view->viewport(), &mouseEvent1);
        QTest::qWait(delay);
    }

    // 全选拉伸
    view->slotOnSelectAll();
    handles = view->drawScene()->selectGroup()->handleNodes();
    for (int i = 0; i < handles.size(); ++i) {
        CSizeHandleRect *pNode = handles[i];
        QPoint posInView = view->mapFromScene(pNode->mapToScene(pNode->boundingRect().center()));
        DTestEventList e;
        e.addMouseMove(posInView, 100);
        e.addMousePress(Qt::LeftButton, Qt::ShiftModifier, posInView, 100);
        e.addMouseMove(posInView - QPoint(50, 50), 100);
        e.addMouseRelease(Qt::LeftButton, Qt::ShiftModifier, posInView - QPoint(50, 50), 100);
        e.simulate(view->viewport());
    }
}

TEST(TextItem, TestSelectAllTextItem)
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

TEST(TextItem, TestLayerChange)
{
    layerChange();
}

TEST(TextItem, TestGroupUngroup)
{
    groupUngroup();
}

TEST(TextItem, TestSaveTextItemToFile)
{
    CGraphicsView *view = getCurView();
    ASSERT_NE(view, nullptr);
    CCentralwidget *c = getMainWindow()->getCCentralwidget();
    ASSERT_NE(c, nullptr);

    // save ddf file
    QString TextItemPath = QApplication::applicationDirPath() + "/test_text.ddf";
    QFile file(TextItemPath);
    file.open(QIODevice::ReadWrite);
    file.close();
    view->getDrawParam()->setDdfSavePath(TextItemPath);
    c->slotSaveToDDF(true);
    QTest::qWait(100);

    QFileInfo info(TextItemPath);
    ASSERT_TRUE(info.exists());
}

TEST(TextItem, TestOpenTextItemFromFile)
{
    CGraphicsView *view = getCurView();
    ASSERT_NE(view, nullptr);

    // 打开保存绘制的 ddf
    QString TextItemPath = QApplication::applicationDirPath() + "/test_text.ddf";

    QMimeData mimedata;
    QList<QUrl> li;
    li.append(QUrl(TextItemPath));
    mimedata.setUrls(li);

    const QPoint pos = view->viewport()->rect().center();
    QDragEnterEvent eEnter(pos, Qt::IgnoreAction, &mimedata, Qt::LeftButton, Qt::NoModifier);
    dApp->sendEvent(view->viewport(), &eEnter);

    QDropEvent e(pos, Qt::IgnoreAction, &mimedata, Qt::LeftButton, Qt::NoModifier);
    dApp->sendEvent(view->viewport(), &e);
    QTest::qWait(100);

    view = getCurView();
    ASSERT_NE(view, nullptr);
    int addedCount = view->drawScene()->getBzItems(view->drawScene()->items()).count();
    ASSERT_EQ(addedCount, 2);
}

#endif
