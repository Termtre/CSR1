#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QChartView>
#include <QWheelEvent>
#include <QtCharts>

class CustomView : public QChartView
{
    Q_OBJECT
public:
    explicit CustomView(QWidget* parent = nullptr);
    void zoomIt(bool);

protected:
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    using QChartView::QChartView;

private:
    bool itZoom = false;
    QPoint oldPoint;
    bool isPress;
    double dMinX, dMaxX, dMinY, dMaxY;

};

#endif // CUSTOMVIEW_H
