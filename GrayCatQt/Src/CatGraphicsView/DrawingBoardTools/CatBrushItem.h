﻿#ifndef CATBRUSHITEM_H
#define CATBRUSHITEM_H

#include <QGraphicsItem>

#include "CatBrushObject.h"
#include "CatBrushBufferItem.h"

class CatBrushItem : public QGraphicsItem
{
public:
    explicit CatBrushItem(QGraphicsItem *parent = nullptr);
    ~CatBrushItem();

    void DrawPress(int id,const QPointF &point);
    void DrawMove(int id,const QPointF &lastPoint,const QPointF &curPoint);
    void DrawRelease(int id, const QPointF &point);
    void SetBackgroundColor(const QColor & color);
    void SetMode(CatBrushObject::BrushMode mode);

    void Clear();

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void InitProperty();
    void DrawToReal(CatBrushObject* object);  //在真实层绘非矢量图
    void InitBrush();                    //初始化图层
    void DoErase(QPointF pos1, QPointF pos2, int width);    // 线擦除
    QPainterPath CreateStrokePath(const QPointF &pos1, const QPointF &pos2, int width);
    void UpdateRectSize(QPointF point);

private:
    QRectF m_ySizeRect;
    QRectF m_yLastSizeRect;

    CatBrushBufferItem *m_pCatBrushBufferItem;

    QPixmap *m_pRealBrush;
    QPainter *m_pRealPainter;

    QMap<int, CatBrushObject*> m_yBrushObjects;

    QColor m_yBgColor;

    CatBrushObject::BrushMode m_yBrushMode;

};

#endif // CATBRUSHITEM_H
