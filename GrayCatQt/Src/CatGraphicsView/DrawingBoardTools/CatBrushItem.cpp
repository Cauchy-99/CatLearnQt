﻿#include "CatBrushItem.h"
#include <QPainter>
#include <QGraphicsScene>

#include <QDebug>

CatBrushItem::CatBrushItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    InitProperty();
    InitBrush();
}

CatBrushItem::~CatBrushItem()
{
    if(m_pRealPainter != nullptr)
    {
        delete m_pRealPainter;
        m_pRealPainter = nullptr;
    }

    if(m_pRealBrush != nullptr)
    {
        delete m_pRealBrush;
        m_pRealBrush = nullptr;
    }
}

void CatBrushItem::DrawPress(int id, const QPointF &point)
{
    UpdateRectSize(point);
    CatBrushObject *BrushObject = new CatBrushObject(point);
    BrushObject->AddToPath(point, point);
    m_yBrushObjects.insert(id, BrushObject);
    m_pCatBrushBufferItem->DrawToBuffer(BrushObject);
}

void CatBrushItem::DrawMove(int id, const QPointF &lastPoint, const QPointF &curPoint)
{
    CatBrushObject *BrushObject = m_yBrushObjects.value(id, nullptr);
    if(BrushObject == nullptr)
    {
        return;
    }
    UpdateRectSize(curPoint);
    BrushObject->AddToPath(lastPoint, curPoint);

    if(m_yBrushMode == CatBrushObject::BrushMode::PenBrushMode)
    {
        DrawToReal(BrushObject);

    } else if(m_yBrushMode == CatBrushObject::BrushMode::EraserMode)
    {
        DoErase(lastPoint, curPoint, 30);
    }
}

void CatBrushItem::DrawRelease(int id, const QPointF &point)
{
    CatBrushObject *BrushObject = m_yBrushObjects.value(id, nullptr);
    if(BrushObject == nullptr)
    {
        return;
    }
    UpdateRectSize(point);
    BrushObject->AddToPath(point, point);

    if(m_yBrushMode == CatBrushObject::BrushMode::PenBrushMode)
    {
        DrawToReal(BrushObject);

    } else if(m_yBrushMode == CatBrushObject::BrushMode::EraserMode)
    {
        DoErase(point, point, 30);
    }

    m_yBrushObjects.remove(id);
    delete BrushObject;
    BrushObject = nullptr;

}

void CatBrushItem::SetBackgroundColor(const QColor &color)
{
    m_yBgColor = color;
    if(m_pRealBrush != nullptr)
    {
        m_pRealBrush->fill(color);
    }
}

void CatBrushItem::SetMode(CatBrushObject::BrushMode mode)
{
    m_yBrushMode = mode;
}

void CatBrushItem::Clear()
{
    if(m_pRealBrush != nullptr)
    {
        m_pRealBrush->fill(m_yBgColor);
        update();
    }
}

QRectF CatBrushItem::boundingRect() const
{
    //qDebug() << "CatBrushItem: " << m_ySizeRect;
    return m_ySizeRect;
}

void CatBrushItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    if(m_pRealBrush != nullptr)
    {
        painter->save();
        painter->drawPixmap(QRectF(0, 0, m_ySizeRect.width(), m_ySizeRect.height()), *m_pRealBrush, QRectF(-10, -10, m_ySizeRect.width(), m_ySizeRect.height()));

        painter->restore();

        painter->save();
        QPen pen;
        pen.setWidth(2);
        pen.setBrush(QColor(Qt::white));
        painter->setPen(pen);
        painter->drawRect(m_ySizeRect);
        painter->restore();
    }
}

void CatBrushItem::InitProperty()
{
    m_pRealBrush = nullptr;
    m_pRealPainter = nullptr;
    m_pCatBrushBufferItem = nullptr;
    m_ySizeRect = QRectF(0,0,0,0);
    m_yLastSizeRect = QRectF(0,0,0,0);
}

void CatBrushItem::DrawToReal(CatBrushObject *object)
{
    if(m_pRealPainter != nullptr && m_pCatBrushBufferItem != nullptr)
    {
        m_pRealPainter->setRenderHint(QPainter::Antialiasing, true);
        m_pRealPainter->setCompositionMode(QPainter::CompositionMode_Source);
        m_pRealPainter->setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        QPainterPath path = object->StrokePath(5);
        m_pRealPainter->fillPath(path, Qt::red);

        m_pCatBrushBufferItem->Clear();
        this->update(path.boundingRect());
    }
}

void CatBrushItem::InitBrush()
{
    m_pCatBrushBufferItem = new CatBrushBufferItem(this);
    m_pCatBrushBufferItem->setZValue(10);
}

void CatBrushItem::DoErase(QPointF pos1, QPointF pos2, int width)
{
    if(m_pRealPainter != nullptr)
    {
        QPainterPath path = CreateStrokePath(pos1, pos2, width);

        m_pRealPainter->setRenderHint(QPainter::Antialiasing, true);
        m_pRealPainter->setCompositionMode(QPainter::CompositionMode_Source);
        m_pRealPainter->setPen(QPen(Qt::transparent, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        m_pRealPainter->fillPath(path, Qt::transparent);

        update();
    }
}

QPainterPath CatBrushItem::CreateStrokePath(const QPointF &pos1, const QPointF &pos2, int width)
{
    QPainterPath path;
    path.moveTo(pos1);
    path.lineTo(pos2);

    if(path.isEmpty())
    {
        path.addRegion(QRegion(QRect(pos1.x() - 0.5, pos2.y() - 0.5, 1, 1),
                               QRegion::Ellipse));
    }

    QPainterPathStroker stroker;
    stroker.setWidth(width);
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);

    return stroker.createStroke(path);
}

void CatBrushItem::UpdateRectSize(QPointF point)
{
    qDebug() << "Scene: " << scene()->sceneRect();
    qreal left = boundingRect().left();
    qreal right = boundingRect().right();
    qreal top = boundingRect().top();
    qreal bottom = boundingRect().bottom();

    if(left == 0 && right == 0 && top == 0 && bottom == 0)
    {
        left = point.x();
        right = point.x() + 5;
        top = point.y();
        bottom = point.y() + 5;
        m_ySizeRect = QRectF(left, top, right - left, bottom - top);
        m_yLastSizeRect = m_ySizeRect;
    } else {
        if(point.x() < boundingRect().left())
        {
            left = point.x();
        }
        if(point.x() > boundingRect().right())
        {
            right = point.x();
        }
        if(point.y() < boundingRect().top())
        {
            top = point.y();
        }
        if(point.y() > boundingRect().bottom())
        {
            bottom = point.y();
        }
        m_yLastSizeRect = m_ySizeRect;
        m_ySizeRect = QRectF(left, top, right - left, bottom - top);
    }


    if(m_pRealBrush == nullptr)
    {
        m_pRealBrush = new QPixmap(m_ySizeRect.size().toSize());
        m_pRealBrush->fill(Qt::transparent);

        m_pRealPainter = new QPainter(m_pRealBrush);
        m_pCatBrushBufferItem->InitSizeRect(m_ySizeRect);


    } else {
        QPixmap *temp = new QPixmap(m_ySizeRect.size().toSize());
        temp->fill(Qt::transparent);

        QPainter *tempPainter = new QPainter(temp);
        m_pCatBrushBufferItem->UpdateSizeRect(m_ySizeRect);
        int x = 0;
        int y = 0;
        if(m_yLastSizeRect.x() - m_ySizeRect.x() > 0)
        {
            x = m_yLastSizeRect.x() - m_ySizeRect.x();
        }

        if(m_yLastSizeRect.y() - m_ySizeRect.y() > 0)
        {
            y = m_yLastSizeRect.y() - m_ySizeRect.y();
        }
        tempPainter->drawPixmap(x, y, *m_pRealBrush);

        delete m_pRealPainter;
        m_pRealPainter = nullptr;

        delete m_pRealBrush;
        m_pRealBrush = nullptr;

        m_pRealBrush = temp;
        m_pRealPainter = tempPainter;
    }
    update();
}
