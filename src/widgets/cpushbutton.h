/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     Ji XiangLong <jixianglong@uniontech.com>
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
#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <DPushButton>
#include <QString>

DWIDGET_USE_NAMESPACE

class CPushButton : public DPushButton
{
public:
    explicit CPushButton(const QString &text, QWidget *parent = nullptr);

protected:
    virtual void enterEvent(QEvent *e) override;
    virtual void leaveEvent(QEvent *e) override;
};

#endif // CPUSHBUTTON_H
