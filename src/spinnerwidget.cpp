/*
 * qconnman - Connman Applet
 * Copyright (C) 2011 O.S. Systems
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "spinnerwidget.h"

#include <QPen>
#include <QPointF>
#include <QPainter>

#include <cmath>

#define MATH_PI 3.14159265358979323846

SpinnerWidget::SpinnerWidget(QWidget * parent)
    : QWidget(parent),
      m_currentStep(0),
      m_linesCount(12),
      m_linesWidth(2),
      m_linesColor(QPalette::ButtonText),
      m_insetFactor(0.5),
      m_roundedCap(true),
      m_useFixedSize(false),
      m_fixedSize(QSize(22, 22))
{
    m_timerRotation.setInterval(70);
    QObject::connect(&m_timerRotation, SIGNAL(timeout()), this, SLOT(rotationUpdate()));
    startRotation();
}

void SpinnerWidget::startRotation()
{
    m_timerRotation.start();
}

void SpinnerWidget::stopRotation()
{
    m_timerRotation.stop();
}

void SpinnerWidget::toggleRotation()
{
    isRotationStarted() ? stopRotation() : startRotation();
}

void SpinnerWidget::paintEvent(QPaintEvent * event)
{
    renderSpinner();
    QWidget::paintEvent(event);
}

void SpinnerWidget::renderSpinner()
{
    qreal x = width() / 2;
    qreal y = height() / 2;

    qreal radius = qMin(spinnerSize().width() / 2 , spinnerSize().height() / 2);
    qreal half = m_linesCount / 2;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setWidth(m_linesWidth);
    pen.setCapStyle(capStyle());

    for (int i = 0 ; i < m_linesCount ; i++)
    {
        qreal inset = m_insetFactor * radius;
        qreal alpha = ((qreal)(((i + m_linesCount) - m_currentStep) % m_linesCount)) / ((qreal) m_linesCount);

        QColor color(m_linesColor);
        color.setAlpha(alpha * 255);

        pen.setColor(color);

        qreal angle = ((i * MATH_PI) / half);

        QPointF start(x + ((radius - inset) * cos(angle)), y + ((radius - inset) * sin(angle)));
        QPointF end(x + (radius * cos(angle)), y + (radius * sin(angle)));

        painter.setPen(pen);
        painter.drawLine(start, end);
    }
}

void SpinnerWidget::rotationUpdate()
{
    m_currentStep = (m_currentStep + 1) % m_linesCount;
    update();
}
