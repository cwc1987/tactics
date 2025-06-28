#ifndef OFFLINETILE_H
#define OFFLINETILE_H

#include <QApplication>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QFuture>
#include <QGVDrawItem.h>

class OfflineTile : public QGVDrawItem
{
    Q_OBJECT
public:
    explicit OfflineTile(const QGV::GeoTilePos& tilePos);

private:
    void onProjection(QGVMap* geoMap) override;
    QPainterPath projShape() const override;
    void projPaint(QPainter* painter) override;
    void loadImages(const QGV::GeoTilePos &tilePos);

private:
    QImage *m_imgImage;
    QImage *m_ciaImage;
    QGV::GeoTilePos m_tilePos;
    QRectF m_projRect;

    // 添加异步加载支持
    bool m_imagesLoaded;
    QFuture<void> m_loadFuture;
};

#endif // OFFLINETILE_H
