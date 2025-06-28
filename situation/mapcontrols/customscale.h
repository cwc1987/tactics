#ifndef CUSTOMSCALE_H
#define CUSTOMSCALE_H

#include <QObject>
#include <QGVWidgetScale.h>

class CustomScale : public QGVWidgetScale
{
    Q_OBJECT
public:
    explicit CustomScale(Qt::Orientation orientation = Qt::Horizontal);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // CUSTOMSCALE_H
