#ifndef CGRAPHICSITEMEVENT_H
#define CGRAPHICSITEMEVENT_H

#include <QPointF>
#include "globaldefine.h"

class CGraphItemEvent
{
public:
    enum EItemType {EMove, EScal, ERot, EBlur, EUnKnow};

    CGraphItemEvent(EItemType tp = EUnKnow,
                    const QPointF &oldPos = QPointF(),
                    const QPointF &pos    = QPointF());

    inline QPointF oldPos() {return _oldPos;}
    void   setOldPos(const QPointF &pos);

    inline QPointF pos() {return _pos;}
    void   setPos(const QPointF &pos);

    inline QPointF beginPos() {return _beginPos;}
    void   setBeginPos(const QPointF &pos);

    inline QPointF centerPos() {return _centerPos;}
    void   setCenterPos(const QPointF &pos);

    inline QSizeF orgSize() {return _orgSz;}
    void   setOrgSize(const QSizeF &sz);

    inline EItemType type() {return this->_tp;}

    inline EChangedPhase eventPhase() {return this->_phase;}
    void   setEventPhase(EChangedPhase ph);

    inline bool isXTransBlocked();
    void   setXTransBlocked(bool b);

    inline bool isYTransBlocked();
    void   setYTransBlocked(bool b);

    inline bool isXNegtiveOffset();
    void   setXNegtiveOffset(bool b);

    inline bool isYNegtiveOffset();
    void   setYNegtiveOffset(bool b);

    inline QPointF   offset() {return _pos - _oldPos;}
    inline QPointF   totalOffset() {return _pos - _beginPos;}

    void updateTrans();

    QTransform trans();
    void       setTrans(const QTransform &trans);


    CGraphItemEvent transToEvent(const QTransform &tran, const QSizeF &newOrgSz);

protected:
    EItemType  _tp;
    QPointF    _oldPos;
    QPointF    _pos;

    QPointF    _beginPos;

    QPointF    _centerPos;
    QSizeF     _orgSz;

    EChangedPhase _phase = EChanged;

    QTransform    _trans;

    bool          _transDirty = true;

    bool          _blockXTrans = false;
    bool          _blockYTrans = false;
    bool          _isXNegtiveOffset = false;
    bool          _isYNegtiveOffset = false;

};

#endif // CGRAPHICSITEMEVENT_H
