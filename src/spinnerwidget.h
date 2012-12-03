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

#ifndef _SPINNERWIDGET_H
#define _SPINNERWIDGET_H

#include <QColor>
#include <QWidget>
#include <QPaintEvent>

#include <QTimer>

class SpinnerWidget: public QWidget
{
    Q_OBJECT

public:
    SpinnerWidget(QWidget *parent = NULL);

public:
    Q_PROPERTY(int linesCount READ linesCount WRITE setLinesCount);
    Q_PROPERTY(int linesWidth READ linesWidth WRITE setLinesWidth);
    Q_PROPERTY(QColor linesColor READ linesColor WRITE setLinesColor);
    Q_PROPERTY(qreal insetFactor READ insetFactor WRITE setInsetFactor);
    Q_PROPERTY(bool useFixedSize READ useFixedSize WRITE setUseFixedSize);
    Q_PROPERTY(QSize fixedSize READ fixedSize WRITE setFixedSize);
    Q_PROPERTY(bool roundedCap READ roundedCap WRITE setRoundedCap);
    Q_PROPERTY(int rotationInterval READ rotationInterval WRITE setRotationInterval);
    Q_PROPERTY(QSize spinnerSize READ spinnerSize);
    Q_PROPERTY(bool rotationStarted READ isRotationStarted);

    inline int linesCount() const { return m_linesCount; }
    void setLinesCount(int val)
    {
        m_linesCount = val;
        update();
    }

    inline int linesWidth() const { return m_linesWidth; }
    void setLinesWidth(int val)
    {
        m_linesWidth = val;
        update();
    }

    inline QColor linesColor() const { return m_linesColor; }
    void setLinesColor(QColor val)
    {
        m_linesColor = val;
        update();
    }

    inline qreal insetFactor() const { return m_insetFactor; }
    void setInsetFactor(qreal val)
    {
        m_insetFactor = val;
        update();
    }

    inline bool useFixedSize() const { return m_useFixedSize; }
    void setUseFixedSize(bool val)
    {
        m_useFixedSize = val;
        update();
    }

    inline QSize fixedSize() const { return m_fixedSize; }
    void setFixedSize(QSize val)
    {
        m_fixedSize = val;
        update();
    }

    inline bool roundedCap() const { return m_roundedCap; }
    void setRoundedCap(bool val)
    {
        m_roundedCap = val;
        update();
    }

    inline int rotationInterval() const { return m_timerRotation.interval(); }
    inline void setRotationInterval(int val) { m_timerRotation.setInterval(val); }

    QSize spinnerSize() const
    {
        if (m_useFixedSize)
            return fixedSize();
        else
            return size() - QSize(m_linesWidth, m_linesWidth);
    }

    inline Qt::PenCapStyle capStyle() const { return m_roundedCap ? Qt::RoundCap : Qt::SquareCap; }

    bool isRotationStarted() const { return m_timerRotation.isActive(); }

public slots:
        void startRotation();
        void stopRotation();
        void toggleRotation();

protected:
        virtual void paintEvent(QPaintEvent * event);
        virtual void renderSpinner();

private slots:
        void rotationUpdate();

private:
        int m_currentStep;
        int m_linesCount;
        int m_linesWidth;
        QColor m_linesColor;
        qreal m_insetFactor;
        bool m_useFixedSize;
        bool m_roundedCap;
        QTimer m_timerRotation;
        QSize m_fixedSize;
};

#endif
