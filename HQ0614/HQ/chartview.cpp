/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "chartview.h"
#include <QtGui/QMouseEvent>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCore/QDebug>
#include <QtCharts/QPolarChart>
#include "mainwindow.h"
ChartView::ChartView(QChart *chart, QWidget *parent) :
    QChartView(chart, parent),
    m_isTouching(false)
{
    setRubberBand(QChartView::RectangleRubberBand);
}

bool ChartView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin) {
        // By default touch events are converted to mouse events. So
        // after this event we will get a mouse event also but we want
        // to handle touch events as gestures only. So we need this safeguard
        // to block mouse events that are actually generated from touch.
        m_isTouching = true;

        // Turn off animations when handling gestures they
        // will only slow us down.
        chart()->setAnimationOptions(QChart::NoAnimation);
    }
    return QChartView::viewportEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    QChartView::mouseMoveEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching)
        m_isTouching = false;

    // Because we disabled animations when touch event was detected
    // we must put them back on.
    chart()->setAnimationOptions(QChart::SeriesAnimations);

    QChartView::mouseReleaseEvent(event);
}

//![1]
void ChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
//![1]
    case Qt::Key_Left:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -10);
        break;
    case Qt::Key_Home:
        chart() -> zoomReset();
        chart() -> zoom(0.8);
        break;
    case Qt::Key_Space:
        switchChartType();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}
void ChartView::switchChartType()
{
    QChart *newChart;
    QChart *oldChart = chart();

    if (oldChart->chartType() == QChart::ChartTypeCartesian)
        newChart = new QPolarChart();
    else
        newChart = new QChart();

    // Move series and axes from old chart to new one
    const QList<QAbstractSeries *> seriesList = oldChart->series();
    const QList<QAbstractAxis *> axisList = oldChart->axes();
    QList<QPair<qreal, qreal> > axisRanges;
    for (QAbstractAxis *axis : axisList) {
        QValueAxis *valueAxis = static_cast<QValueAxis *>(axis);
        axisRanges.append(QPair<qreal, qreal>(valueAxis->min(), valueAxis->max()));
    }

    for (QAbstractSeries *series : seriesList)
        oldChart->removeSeries(series);

    for (QAbstractAxis *axis : axisList) {
        oldChart->removeAxis(axis);
        newChart->addAxis(axis, axis->alignment());
    }

    for (QAbstractSeries *series : seriesList) {
        newChart->addSeries(series);
        for (QAbstractAxis *axis : axisList)
            series->attachAxis(axis);
    }

    int count = 0;
    for (QAbstractAxis *axis : axisList) {
        axis->setRange(axisRanges[count].first, axisRanges[count].second);
        count++;
    }

    newChart->setTitle(oldChart->title());
    delete oldChart;
}
