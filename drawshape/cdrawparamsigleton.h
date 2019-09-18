/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     WangXing
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
#ifndef CDRAWPARAMSIGLETON_H
#define CDRAWPARAMSIGLETON_H
#include "globaldefine.h"
#include <QtGlobal>
#include <QColor>
#include <QPen>
#include <QFont>

class CDrawParamSigleton
{
private :
    static CDrawParamSigleton *m_pInstance;
    CDrawParamSigleton();

public :
    static CDrawParamSigleton *GetInstance();

public:
    void setLineWidth(int lineWidth);
    int getLineWidth() const;

    void setLineColor(const QColor &lineColor);
    QColor getLineColor() const;

    void setFillColor(const QColor &fillColor);
    QColor getFillColor() const;

    void setPen(const QPen &pen);
    QPen getPen() const;

    void setBrush(const QBrush &brush);
    QBrush getBrush() const;


    void setCurrentDrawToolMode(EDrawToolMode mode);
    EDrawToolMode getCurrentDrawToolMode() const;

    int getRadiusNum() const;
    void setRadiusNum(int radiusNum);

    int getAnchorNum() const;
    void setAnchorNum(int anchorNum);

    int getSideNum() const;
    void setSideNum(int sideNum);

    EPenType getPenType() const;
    void setPenType(const EPenType &penType);

    EPenType getCurrentPenType() const;
    void setCurrentPenType(const EPenType &currentPenType);

    QFont getTextFont() const;
    void setTextFont(const QString &strFont);

    void setShiftKeyStatus(bool flag);
    bool getShiftKeyStatus();

    void setAltKeyStatus(bool flag);
    bool getAltKeyStatus();

    void setCtlKeyStatus(bool flag);
    bool getCtlKeyStatus();

    void setScale(qreal scale);
    qreal getScale() const;

    void setTextSize(qreal size);
    qreal getTextSize() const;

    void setTextColor(const QColor &fillColor);
    QColor getTextColor() const;

    ECutType getCutType() const;
    void setCutType(const ECutType &cutType);

    QSize getCutSize() const;
    void setCutSize(const QSize &cutSize);

    QSize getCutDefaultSize() const;
    void setCutDefaultSize(const QSize &cutSize);

    ECutAttributeType getCutAttributeType() const;
    void setCutAttributeType(const ECutAttributeType &cutAttributeType);


    bool getIsModify() const;
    void setIsModify(bool isModify);

    bool getIsQuit() const;
    void setIsQuit(bool isQuit);

    EBlurEffect getBlurEffect() const;
    void setBlurEffect(const EBlurEffect &blurEffect);

    int getBlurWidth() const;
    void setBlurWidth(const int width);

    void setSingleFontFlag(bool flag);
    bool getSingleFontFlag() const;

    int getThemeType() const;
    void setThemeType(const int type);

private:
    int m_nlineWidth;
    QColor m_sLineColor;
    QColor m_nFillColor;

    int m_radiusNum; //多角星半径
    int m_anchorNum;//多角星锚点数

    int m_sideNum;//多边形边数

    EPenType m_currentPenType; //当前画笔类型

    QFont m_textFont; //文本字体
    //qreal m_textSize; //文本大小
    QColor m_textColor;//文本颜色
    bool m_singleFontFlag; //只包含一个字体

    EDrawToolMode m_currentDrawToolMode;

    //按键状态
    bool m_bShiftKeyPress;
    bool m_bAltKeyPress;
    bool m_bCtlKeyPress;

    //当前比例尺
    qreal m_Scale;

    //cut
    ECutAttributeType m_cutAttributeType;
    ECutType m_cutType;
    QSize m_cutSize;
    QSize m_cutDefaultSize;


    //图元是否有修改
    bool m_isModify;
    //保存完成后是否需要退出
    bool m_isQuit;

    //blur
    EBlurEffect m_effect; //模糊效果
    int m_blurWidth;    //模糊半径

    //当前主题
    int m_thremeType;
};


#endif // CDRAWPARAMSIGLETON_H
