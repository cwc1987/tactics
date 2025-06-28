#include "customscale.h"
#include <QPen>
#include <QPainter>
#include "settings.h"

CustomScale::CustomScale(Qt::Orientation orientation)
    : QGVWidgetScale(orientation)
{
}

void CustomScale::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if (size().isEmpty()) {
        return;
    }
    QRect paintRect;
    if (getOrientation() == Qt::Horizontal) {
        paintRect = QRect(QPoint(0, 0), size());
    } else {
        paintRect = QRect(QPoint(0, 0), QSize(size().height(), size().width()));
    }
    paintRect.moveCenter(rect().center());

    QPainter painter(this);
    if (getOrientation() != Qt::Horizontal) {
        painter.setTransform(QGV::createTransfromAzimuth(rect().center(), -90.0));
    }

    painter.fillRect(rect(), QBrush(Settings::instance()->getColor(SettingsKey::ControlsBackGroundColor)));

    const int lnWidth = 2;
    QRect lineRect = paintRect;
    lineRect.adjust(lnWidth, lnWidth + lineRect.height() / 2, -lnWidth, -lnWidth);
    painter.setPen(QPen(QBrush(Qt::black), lnWidth));
    painter.drawLine(QLine(QPoint(lineRect.left(), lineRect.bottom()), QPoint(lineRect.right(), lineRect.bottom())));
    painter.drawLine(QLine(QPoint(lineRect.left(), lineRect.bottom()), QPoint(lineRect.left(), lineRect.top())));
    painter.drawLine(QLine(QPoint(lineRect.right(), lineRect.bottom()), QPoint(lineRect.right(), lineRect.top())));

    QRect textRect = paintRect;
    textRect.moveCenter(paintRect.center());
    painter.setPen(QPen(Settings::instance()->getColor(SettingsKey::ControlsTextColor)));
    painter.drawText(textRect, Qt::AlignCenter, getDistanceLabel(getScaleMeters()));
}
