/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"

#include <QtWidgets>
#include <QtMath>

const int View::SLIDER_MAXVALUE= 600;
const int View::SLIDER_FORMAXEXPONENT= 50;

View::View(QWidget *parent)
    : QFrame(parent)
{
    zoomScale=1.0;
    setFrameStyle(Sunken | StyledPanel);
    graphicsView = new DisplayScene(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":images/zoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":images/zoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(SLIDER_MAXVALUE);
    zoomSlider->setValue(SLIDER_MAXVALUE/2);
    zoomSlider->setTickPosition(QSlider::TicksRight);
    zoomLabel=new QLabel("1");
    zoomLabel->setToolTip(tr("ingrandimento percentuale dello zoom"));
    zoomLabel->setMinimumWidth(70);
    zoomLabel->setAlignment(Qt::AlignCenter);
    zoomLabel->setStyleSheet(tr(
    "QLabel {background-color: #fafafa}"
    "QLabel {color: #000000}"
    "QLabel {border: 1px solid grey}"
    "QLabel {border-radius: 8px}"
    "QLabel {padding: 3px}"
    "QLabel {margin: 5px}"));

    setZoomLabel();

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);


    QGridLayout *topLayout = new QGridLayout;

    topLayout->addWidget(graphicsView, 0, 0);
    topLayout->addLayout(zoomSliderLayout, 0, 1);
    setLayout(topLayout);

    connect(zoomSlider, &QAbstractSlider::valueChanged, this, &View::setZoomLabel);
    connect(zoomSlider, &QAbstractSlider::valueChanged, this, &View::setupMatrix);
    connect(zoomInIcon, &QAbstractButton::clicked, this, &View::zoomIn);
    connect(zoomOutIcon, &QAbstractButton::clicked, this, &View::zoomOut);

    setupMatrix();
}

DisplayScene *View::displayScene() const
{
    return graphicsView;
}

void View::resetView()
{
    zoomSlider->setValue(SLIDER_MAXVALUE/2);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));
    setZoomLabel();
}

QLabel *View::getZoomLabel() const
{
    return zoomLabel;
}

void View::roomView()
{
    zoomSlider->setValue(550);
    setupMatrix();
}

void View::setupMatrix()
{
    zoomScale = std::pow(double(2), (zoomSlider->value() - SLIDER_MAXVALUE/2) / double(SLIDER_FORMAXEXPONENT));

    QTransform matrix;
    matrix.scale(zoomScale, zoomScale);
    graphicsView->setTransform(matrix);
    setZoomLabel();
}

void View::setZoomLabel()
{
    QString zoomValue=QString::number(zoomScale*100, 'f', 0);
    zoomLabel->setText(QString("%1 %").arg(zoomValue));
}

void View::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void View::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

double View::scale()const
{
    return zoomScale;
}

QSlider* View::slider()const
{
    return zoomSlider;
}

void View::setScale(const double& scale)
{
    int sliderIndex=View::SLIDER_MAXVALUE/2+View::SLIDER_FORMAXEXPONENT*std::log2f(scale);
    zoomSlider->setValue(sliderIndex);
}

void View::zoomWin()
{
    QPointF rubberBandRectTopLeft=graphicsView->mapToScene(graphicsView->getSelectionRect().topLeft());
    QPointF rubberBandRectBottomRight=graphicsView->mapToScene(graphicsView->getSelectionRect().bottomRight());
    QPointF viewportTopLeft=graphicsView->mapToScene(graphicsView->viewport()->rect().topLeft());
    QPointF viewportBottomRight=graphicsView->mapToScene(graphicsView->viewport()->rect().bottomRight());
    QSizeF viewportSize=QRectF(viewportTopLeft, viewportBottomRight).size();
    QSizeF rubberBandSize=QRectF(rubberBandRectTopLeft, rubberBandRectBottomRight).size();
    double scale=zoomScale*std::min(viewportSize.height()/rubberBandSize.height(),
                          viewportSize.width()/rubberBandSize.width());

    int sliderIndex=View::SLIDER_MAXVALUE/2+View::SLIDER_FORMAXEXPONENT*std::log2f(scale);

    zoomSlider->setValue(sliderIndex);
    graphicsView->centerOn(QRectF(rubberBandRectTopLeft, rubberBandRectBottomRight).center());
}
