﻿#pragma once

#include <QWidget>


class QScrollArea;
class QPushButton;
class QSpacerItem;
class QButtonGroup;
class QVBoxLayout;
class QHBoxLayout;

class ListiongOptions : public QWidget
{
    Q_OBJECT
public:
    enum LAYOUT {
        VBox,
        HBox
    };
    explicit ListiongOptions(LAYOUT layout, QWidget *parent = nullptr);
    ~ListiongOptions();

public:
    void AddButton(QPushButton *button, int id);
    void AddButtonNoGroup(QPushButton *button);
    void AddWidget(QWidget *widget);
    void AddItem(QSpacerItem *item);
    QWidget *GetRootWidget( void ) const;
    QWidget *GetWidget( void ) const;
    QButtonGroup *GetButtonGroup( void ) const;
    void Clear();
    void *GetButtonlayout( void );
    void RemoveBackGaugeItem();

    void RemoveItem(QSpacerItem *item);
    void RemoveWidget(QWidget *widget);

    QScrollArea* GetScrollArea() const { return m_pScrollArea; };

private:
    void InitUi( void );
    void InitProperty( void );

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void showEvent(QShowEvent *event);

private:
    QVBoxLayout *m_pRootLayout;
    QWidget *m_pRootWidget;
    QVBoxLayout *m_pRootWidgetLayout;
    QScrollArea *m_pScrollArea;
    QWidget *m_pButtonLists;
    void *m_pHboxLayoutButton;
    void *m_pButtonListsLayout;
    QList<QPushButton*> m_lButtonList;
    QList<QSpacerItem*> m_lItemList;
    QButtonGroup *m_pButtonGroup;
    LAYOUT m_yLayout;
    QSpacerItem *m_pItem_left;
    QSpacerItem *m_pItem_right;
};


