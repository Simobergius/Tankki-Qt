#ifndef TOUCHAREAWIDGET_H
#define TOUCHAREAWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

class TouchAreaWidget : public QWidget {
    Q_OBJECT
public:
    TouchAreaWidget(QWidget *parent = nullptr);
    ~TouchAreaWidget();

    QPoint getPointerLocation();
protected:
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
private:
    QPoint m_pointerLocation;
};

#endif // TOUCHAREAWIDGET_H