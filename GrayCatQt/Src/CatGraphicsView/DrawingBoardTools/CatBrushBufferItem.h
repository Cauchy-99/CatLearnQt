﻿#ifndef CATBRUSHBUFFERITEM_H
#define CATBRUSHBUFFERITEM_H

#include <QGraphicsItem>
#include <QPainter>

#include "CatBrushObject.h"

class CatBrushBufferItem : public QGraphicsItem
{
public:
    explicit CatBrushBufferItem(QGraphicsItem *parent = nullptr);
    ~CatBrushBufferItem();

    void Clear();
    void DrawToBuffer(CatBrushObject *object);

    void InitSizeRect(QRectF size);
    void UpdateSizeRect(QRectF size);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void InitProperty();
    void InitBrush();

private:
    QRectF m_ySizeRect;
    QRectF m_yLastSizeRect;

    QPixmap *m_pBufferBrush;
    QPainter *m_pBufferPainter;

};

#endif // CATBRUSHBUFFERITEM_H
