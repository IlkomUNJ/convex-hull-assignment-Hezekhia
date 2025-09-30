#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QMouseEvent>
#include <QString>

class DrawingCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingCanvas(QWidget *parent = nullptr);

public slots:
    void calculateFastHull();
    void calculateSlowHull();
    void resetPoints();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    int orientation(QPointF p, QPointF q, QPointF r);

private:
    QVector<QPointF> m_points;
    QVector<QPointF> m_hull;
    QString m_statusMessage;
};

#endif
