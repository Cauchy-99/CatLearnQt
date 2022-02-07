/*
 * MIT License
 *
 * Copyright (C) 2021 by wangwenx190 (Yuhang Zhao)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "utilities.h"

#include <QtCore/qvariant.h>
#include <Cocoa/Cocoa.h>
#include <objc/objc.h>
#include <objc/message.h>
#include <QDebug>

FRAMELESSHELPER_BEGIN_NAMESPACE

static constexpr int kDefaultResizeBorderThickness = 8;
static constexpr int kDefaultCaptionHeight = 23;

int Utilities::getSystemMetric(const QWindow *window, const SystemMetric metric, const bool dpiScale, const bool forceSystemValue)
{
    Q_ASSERT(window);
    if (!window) {
        return 0;
    }
    const qreal devicePixelRatio = window->devicePixelRatio();
    const qreal scaleFactor = (dpiScale ? devicePixelRatio : 1.0);
    switch (metric) {
    case SystemMetric::ResizeBorderThickness: {
        const int resizeBorderThickness = window->property(Constants::kResizeBorderThicknessFlag).toInt();
        if ((resizeBorderThickness > 0) && !forceSystemValue) {
            return qRound(static_cast<qreal>(resizeBorderThickness) * scaleFactor);
        } else {
            // ### TO BE IMPLEMENTED: Retrieve system value through official API
            if (dpiScale) {
                return qRound(static_cast<qreal>(kDefaultResizeBorderThickness) * devicePixelRatio);
            } else {
                return kDefaultResizeBorderThickness;
            }
        }
    }
    case SystemMetric::CaptionHeight: {
        const int captionHeight = window->property(Constants::kCaptionHeightFlag).toInt();
        if ((captionHeight > 0) && !forceSystemValue) {
            return qRound(static_cast<qreal>(captionHeight) * scaleFactor);
        } else {
            // ### TO BE IMPLEMENTED: Retrieve system value through official API
            if (dpiScale) {
                return qRound(static_cast<qreal>(kDefaultCaptionHeight) * devicePixelRatio);
            } else {
                return kDefaultCaptionHeight;
            }
        }
    }
    case SystemMetric::TitleBarHeight: {
        const int titleBarHeight = window->property(Constants::kTitleBarHeightFlag).toInt();
        if ((titleBarHeight > 0) && !forceSystemValue) {
            return qRound(static_cast<qreal>(titleBarHeight) * scaleFactor);
        } else {
            const int captionHeight = getSystemMetric(window,SystemMetric::CaptionHeight,
                                                      dpiScale, forceSystemValue);
            const int resizeBorderThickness = getSystemMetric(window, SystemMetric::ResizeBorderThickness,
                                                              dpiScale, forceSystemValue);
            return (((window->windowState() == Qt::WindowMaximized)
                     || (window->windowState() == Qt::WindowFullScreen))
                    ? captionHeight : (captionHeight + resizeBorderThickness));
        }
    }
    }
    return 0;
}

QColor Utilities::getColorizationColor()
{
    // ### TO BE IMPLEMENTED
    return Qt::darkGray;
}

int Utilities::getWindowVisibleFrameBorderThickness(const WId winId)
{
    // ### TO BE IMPLEMENTED
    Q_UNUSED(winId);
    return 1;
}

bool Utilities::shouldAppsUseDarkMode()
{
    // ### TO BE IMPLEMENTED
    return false;
}

ColorizationArea Utilities::getColorizationArea()
{
    // ### TO BE IMPLEMENTED
    return ColorizationArea::None;
}

bool Utilities::isThemeChanged(const void *data)
{
    // ### TO BE IMPLEMENTED
    Q_UNUSED(data);
    return false;
}

bool Utilities::isSystemMenuRequested(const void *data, QPointF *pos)
{
    // ### TO BE IMPLEMENTED
    Q_UNUSED(data);
    Q_UNUSED(pos);
    return false;
}

bool Utilities::showSystemMenu(const WId winId, const QPointF &pos)
{
    // ### TO BE IMPLEMENTED
    Q_UNUSED(winId);
    Q_UNUSED(pos);
    return false;
}

bool applicationShouldHandleReopen(id self,SEL _cmd, ...)
{
    qDebug() <<__FUNCTION__;
    return false;
}

bool Utilities::setupDockClickEvent()
{
    Class cls = objc_getClass("NSApplication");

    SEL sharedApplication = sel_registerName("sharedApplication");

    objc_object *appInst = (objc_object*)objc_msgSend((id)cls, sharedApplication);

    if(appInst != nullptr)
    {
        objc_object* delegate = (objc_object*)objc_msgSend((id)appInst,sel_registerName("delegate"));
        objc_object* delegateClass = (objc_object*)objc_msgSend((id)delegate, sel_registerName("class"));

        //constchar* tst = (constchar*)class_getName((Class)delegateClass->isa);
        SEL shouldHandle = sel_registerName("applicationShouldHandleReopen:hasVisibleWindows:");
        bool bTst = class_getInstanceMethod((Class)delegateClass,shouldHandle);
        if(bTst)
        {
            bool test = class_replaceMethod((Class)delegateClass,
                               shouldHandle,
                               //(IMP)dockClickHandler((id)appInst,sharedApplication),
                                            reinterpret_cast<IMP>(applicationShouldHandleReopen),
                                            "B@:");
            // "B@:" is quite tricky thing called Method Signature which allows to define C function to selector "applicationShouldHandleReopen:hasVisibleWindows:" and dockClickHandler function will be called as if it was written inside objective-c code.

            if(test)
            {
                qDebug("Registration was successful");
            }
            else
            {
                qDebug("Registration was failed");
            }
        }
        else
        {
            bool test = class_addMethod((Class)delegateClass,
                               shouldHandle,
                               //(IMP)dockClickHandler((id)appInst,sharedApplication),
                                        reinterpret_cast<IMP>(applicationShouldHandleReopen),
                                        "B@:");
            // "B@:" is quite tricky thing called Method Signature which allows to define C function to selector "applicationShouldHandleReopen:hasVisibleWindows:" and dockClickHandler function will be called as if it was written inside objective-c code.

            if(test)
            {
                qDebug("Registration was successful");
            }
            else
            {
                qDebug("Registration was failed");
            }
        }

    }

    return false;
}



FRAMELESSHELPER_END_NAMESPACE
