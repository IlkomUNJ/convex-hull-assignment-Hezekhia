#include "drawingcanvas.h"
#include <QPainter>
#include <QDebug>
#include <QtAlgorithms>
#include <cmath>
#include <QPainterPath>

bool lexicographicalCompare(const QPointF &a, const QPointF &b)
{
    if (qFuzzyCompare(a.x(), b.x())) {
        return a.y() < b.y();
    }
    return a.x() < b.x();
}

DrawingCanvas::DrawingCanvas(QWidget *parent)
    : QWidget(parent), m_statusMessage("Please add points.")
{
    setMouseTracking(true);
}

int DrawingCanvas::orientation(QPointF p, QPointF q, QPointF r)
{
    qreal val = (q.y() - p.y()) * (r.x() - q.x()) -
                (q.x() - p.x()) * (r.y() - q.y());

    if (qFuzzyIsNull(val)) return 0;
    return (val > 0) ? 1 : 2;
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_points.append(event->position());
        m_hull.clear();
        m_statusMessage = QString("Point added. Total: %1.").arg(m_points.size());
        update();
    }
}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(30, 30, 35));

    if (m_hull.size() > 1)
    {
        QPen hullPen(QColor(60, 255, 120), 3);
        painter.setPen(hullPen);

        QPainterPath hullPath;
        hullPath.moveTo(m_hull.first());
        for (int i = 1; i < m_hull.size(); ++i) {
            hullPath.lineTo(m_hull.at(i));
        }
        hullPath.lineTo(m_hull.first());

        painter.fillPath(hullPath, QColor(60, 255, 120, 50));
        painter.drawPath(hullPath);
    }

    QPen pointPen(QColor(255, 100, 100), 1);
    QBrush pointBrush(QColor(255, 100, 100));
    painter.setPen(pointPen);
    painter.setBrush(pointBrush);

    qreal pointRadius = 6.0;

    for (const QPointF &p : m_points)
    {
        painter.drawEllipse(p, pointRadius, pointRadius);
    }

    painter.setPen(QColor(200, 200, 200));
    painter.setFont(QFont("Inter", 10));

    painter.drawText(10, 20, m_statusMessage);
    painter.drawText(10, 40, QString("Total Points: %1. Click to add points.").arg(m_points.size()));
}

void DrawingCanvas::calculateSlowHull()
{
    m_hull.clear();
    int N = m_points.size();
    long long totalIterations = 0;

    if (N < 3)
    {
        m_statusMessage = QString("Failed: At least 3 points required.");
        m_hull = m_points;
        update();
        return;
    }

    QVector<QPair<QPointF, QPointF>> E;

    for (int i = 0; i < N; ++i)
    {
        QPointF p = m_points[i];
        for (int j = 0; j < N; ++j)
        {
            if (i == j) continue;
            QPointF q = m_points[j];

            bool valid = true;

            for (int k = 0; k < N; ++k)
            {
                if (k == i || k == j) continue;
                QPointF r = m_points[k];

                totalIterations++;

                int o = orientation(p, q, r);

                if (o == 2)
                {
                    valid = false;
                    break;
                }
            }

            if (valid)
            {
                E.append({p, q});
            }
        }
    }

    if (E.isEmpty()) {
        m_statusMessage = QString("SLOW Hull Failed: No edges found.");
        update();
        return;
    }

    QPointF startP = E[0].first;
    QPointF currentP = startP;
    QPointF nextP = E[0].second;

    m_hull.append(currentP);

    for (int count = 0; count < E.size() + 2 && nextP != startP; ++count)
    {
        currentP = nextP;
        m_hull.append(currentP);

        bool foundNext = false;

        for (const auto& edge : E)
        {
            if (edge.first == currentP)
            {
                nextP = edge.second;
                foundNext = true;
                break;
            }
        }

        if (!foundNext && currentP != startP) {
            break;
        }
    }

    m_statusMessage = QString("Slow Convex Hull finished. Comparison Iterations: %1").arg(totalIterations);
    qDebug() << m_statusMessage;

    if (m_hull.size() > 1 && m_hull.first() == m_hull.back()) {
        m_hull.removeLast();
    }

    update();
}

void DrawingCanvas::calculateFastHull()
{
    m_hull.clear();
    int N = m_points.size();

    int criticalOperations = 0;

    if (N < 3)
    {
        m_statusMessage = QString("Failed: At least 3 points required.");
        m_hull = m_points;
        update();
        return;
    }

    std::sort(m_points.begin(), m_points.end(), lexicographicalCompare);

    QVector<QPointF> upperHull;
    upperHull.append(m_points[0]);
    upperHull.append(m_points[1]);

    for (int i = 2; i < m_points.size(); ++i)
    {
        upperHull.append(m_points[i]);

        while (upperHull.size() > 2)
        {
            criticalOperations++;
            if (orientation(upperHull[upperHull.size() - 3], upperHull[upperHull.size() - 2], upperHull.back()) == 1) {
                break;
            } else {
                upperHull.remove(upperHull.size() - 2);
            }
        }
    }

    QVector<QPointF> lowerHull;
    int n = m_points.size();

    lowerHull.append(m_points[n - 1]);
    lowerHull.append(m_points[n - 2]);

    for (int i = n - 3; i >= 0; --i)
    {
        lowerHull.append(m_points[i]);

        while (lowerHull.size() > 2)
        {
            criticalOperations++;
            if (orientation(lowerHull[lowerHull.size() - 3], lowerHull[lowerHull.size() - 2], lowerHull.back()) == 1) {
                break;
            } else {
                lowerHull.remove(lowerHull.size() - 2);
            }
        }
    }

    if (lowerHull.size() > 1) {
        lowerHull.removeFirst();
    }
    if (lowerHull.size() > 0) {
        lowerHull.removeLast();
    }

    m_hull = upperHull;
    m_hull.append(lowerHull);

    m_statusMessage = QString("Fast Convex Hull finished. Rotation/Discard Operations: %1").arg(criticalOperations);
    qDebug() << m_statusMessage;

    update();
}


void DrawingCanvas::resetPoints()
{
    m_points.clear();
    m_hull.clear();
    m_statusMessage = "Canvas reset. Please add points.";
    update();
}
