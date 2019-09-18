/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     Renran
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
#include "cgraphicslineitem.h"

#include <DSvgRenderer>

#include <QPen>
#include <QPainter>
#include <QPointF>
#include <QtMath>

DTK_USE_NAMESPACE

static QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath() || pen == Qt::NoPen)
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

CGraphicsLineItem::CGraphicsLineItem(QGraphicsItem *parent)
    : CGraphicsItem(parent)
{
    initLine();
}

CGraphicsLineItem::CGraphicsLineItem(const QLineF &line, QGraphicsItem *parent)
    : CGraphicsItem(parent)
{
    m_line = line;
    initLine();
}

CGraphicsLineItem::CGraphicsLineItem(const QPointF &p1, const QPointF &p2, QGraphicsItem *parent)
    : CGraphicsItem(parent)
{
    setLine(p1.x(), p1.y(), p2.x(), p2.y());
    initLine();
}

CGraphicsLineItem::CGraphicsLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent)
    : CGraphicsItem(parent)
    , m_line(x1, y1, x2, y2)
{

}

CGraphicsLineItem::CGraphicsLineItem(const SGraphicsLineUnitData *data, const SGraphicsUnitHead &head, CGraphicsItem *parent)
    : CGraphicsItem (head, parent)
{
    setLine(data->point1, data->point2);
    initLine();
}


CGraphicsLineItem::~CGraphicsLineItem()
{

}

int CGraphicsLineItem::type() const
{
    return LineType;
}

QPainterPath CGraphicsLineItem::shape() const
{
    QPainterPath path;
    if (m_line == QLineF())
        return path;

    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());

    return qt_graphicsItem_shapeFromPath(path, pen());
}

QRectF CGraphicsLineItem::boundingRect() const
{
    if (this->pen().widthF() == 0.0) {
        const qreal x1 = m_line.p1().x();
        const qreal x2 = m_line.p2().x();
        const qreal y1 = m_line.p1().y();
        const qreal y2 = m_line.p2().y();
        qreal lx = qMin(x1, x2);
        qreal rx = qMax(x1, x2);
        qreal ty = qMin(y1, y2);
        qreal by = qMax(y1, y2);
        return QRectF(lx, ty, rx - lx, by - ty);
    }
    return shape().controlPointRect();
}

QRectF CGraphicsLineItem::rect() const
{
    QRectF rect(m_line.p1(), m_line.p2());
    return rect.normalized();
}

void CGraphicsLineItem::resizeTo(CSizeHandleRect::EDirection dir, const QPointF &point)
{
    QPointF local = mapFromScene(point);
    QPointF p1;
    QPointF p2;
    QPointF pos = this->pos();
    if (dir == CSizeHandleRect::LeftTop) {
        p1 = local;
        p2 = m_line.p2();
    } else {
        p1 = m_line.p1();
        p2 = local;
    }

    p1 = mapToScene(p1);
    p2 = mapToScene(p2);
    setRotation(0);
    setPos(0, 0);
    setLine(p1, p2);
}

void CGraphicsLineItem::resizeTo(CSizeHandleRect::EDirection dir, const QPointF &point, bool bShiftPress, bool bAltPress)
{
    resizeTo(dir, point);
}

QLineF CGraphicsLineItem::line() const
{
    return m_line;
}

void CGraphicsLineItem::setLine(const QLineF &line)
{
    prepareGeometryChange();
    m_line = line;
    updateGeometry();
}

void CGraphicsLineItem::setLine(const QPointF &p1, const QPointF &p2)
{
    setLine(p1.x(), p1.y(), p2.x(), p2.y());
}

void CGraphicsLineItem::setLine(qreal x1, qreal y1, qreal x2, qreal y2)
{
    setLine(QLineF(x1, y1, x2, y2));
}

void CGraphicsLineItem::duplicate(CGraphicsItem *item)
{
    static_cast<CGraphicsLineItem *>(item)->setLine(this->m_line);
    CGraphicsItem::duplicate(item);
}


CGraphicsUnit CGraphicsLineItem::getGraphicsUnit() const
{
    CGraphicsUnit unit;

    unit.head.dataType = this->type();
    unit.head.dataLength = sizeof(SGraphicsLineUnitData);
    unit.head.pen = this->pen();
    unit.head.brush = this->brush();
    unit.head.pos = this->pos();
    unit.head.rotate = this->rotation();
    unit.head.zValue = this->zValue();

    unit.data.pLine = new SGraphicsLineUnitData();
    unit.data.pLine->point1 = this->line().p1();
    unit.data.pLine->point2 = this->line().p2();

    return  unit;
}

int CGraphicsLineItem::getQuadrant() const
{
    int nRet = 1;
    if (m_line.p2().x() - m_line.p1().x() > 0.0001 && m_line.p2().y() - m_line.p1().y() < 0.0001) {
        nRet = 1;
    } else if (m_line.p2().x() - m_line.p1().x() > 0.0001 && m_line.p2().y() - m_line.p1().y() > 0.0001) {
        nRet = 2;
    } else if (m_line.p2().x() - m_line.p1().x() < 0.0001 && m_line.p2().y() - m_line.p1().y() > 0.0001) {
        nRet = 3;
    } else if (m_line.p2().x() - m_line.p1().x() < 0.0001 && m_line.p2().y() - m_line.p1().y() < 0.0001) {
        nRet = 4;
    }
}

void CGraphicsLineItem::updateGeometry()
{
    qreal penwidth = this->pen().widthF();
    for (Handles::iterator it = m_handles.begin(); it != m_handles.end(); ++it) {
        CSizeHandleRect *hndl = *it;
        QPointF centerPos = (m_line.p1() + m_line.p2()) / 2;

        qreal k = 0;
        qreal ang = 0;
        qreal w = hndl->boundingRect().width();
        qreal h = hndl->boundingRect().height();
        switch (hndl->dir()) {
        case CSizeHandleRect::LeftTop:
            hndl->move(m_line.p1().x() - w / 2, m_line.p1().y() - h / 2);
            break;
        case CSizeHandleRect::RightBottom:
            hndl->move(m_line.p2().x() - w / 2, m_line.p2().y() - h / 2);
            break;
        case CSizeHandleRect::Rotation:

            //hndl->move(centerPos.x() - w / 2, centerPos.y() - h - h / 2);
            if (qAbs(m_line.p2().x() - m_line.p1().x()) < 0.0001) {
                hndl->move(m_line.p1().x() - h - penwidth, centerPos.y());
            } else {
                k = -(m_line.p2().y() - m_line.p1().y()) / (m_line.p2().x() - m_line.p1().x());
                ang = atan(k);

                //增加线宽的长度防止缩放造成位置不正确
                qreal x = qAbs((h + penwidth) * sin(ang));
                qreal y = qAbs((h + penwidth) * cos(ang));
                //第一象限
                if (m_line.p2().x() - m_line.p1().x() > 0.0001 && m_line.p2().y() - m_line.p1().y() < 0.0001) {
                    hndl->move(centerPos.x() - w / 2 - x, centerPos.y() - h / 2 - y);
                } else if (m_line.p2().x() - m_line.p1().x() > 0.0001 && m_line.p2().y() - m_line.p1().y() > 0.0001) {
                    hndl->move(centerPos.x() - w / 2 + x, centerPos.y() - h / 2 - y);
                } else if (m_line.p2().x() - m_line.p1().x() < 0.0001 && m_line.p2().y() - m_line.p1().y() > 0.0001) {
                    hndl->move(centerPos.x() - w / 2 + x, centerPos.y() - h / 2 + y);
                } else if (m_line.p2().x() - m_line.p1().x() < 0.0001 && m_line.p2().y() - m_line.p1().y() < 0.0001) {
                    hndl->move(centerPos.x() - w / 2 - x, centerPos.y() - h / 2 + y);
                }
            }
            break;
        default:
            break;
        }
    }
}

void CGraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    updateGeometry();
    painter->setPen(pen());
    painter->drawLine(m_line);
}

void CGraphicsLineItem::initLine()
{
    m_handles.reserve(CSizeHandleRect::None);

    m_handles.push_back(new CSizeHandleRect(this, CSizeHandleRect::LeftTop));
    m_handles.push_back(new CSizeHandleRect(this, CSizeHandleRect::RightBottom));
    m_handles.push_back(new CSizeHandleRect(this, CSizeHandleRect::Rotation, QString(":/theme/light/images/mouse_style/icon_rotate.svg")));

    updateGeometry();
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}
