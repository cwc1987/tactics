#include "offlinetile.h"
#include <QtConcurrent>
#include "settings.h"

OfflineTile::OfflineTile(const QGV::GeoTilePos &tilePos)
    : m_imgImage(nullptr), m_ciaImage(nullptr), m_tilePos(tilePos), m_imagesLoaded(false)
{
    // 异步加载图像
    m_loadFuture = QtConcurrent::run([this, tilePos]() {
        loadImages(tilePos);
    });
}

void OfflineTile::onProjection(QGVMap *geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    m_projRect = geoMap->getProjection()->geoToProj(m_tilePos.toGeoRect());
}

QPainterPath OfflineTile::projShape() const
{
    QPainterPath path;
    path.addRect(m_projRect);
    return path;
}

void OfflineTile::projPaint(QPainter *painter)
{
    if(!m_loadFuture.isFinished())
        return;

    // 检查是否在视口范围内，避免绘制不可见的瓦片
    if (m_projRect.isEmpty() || m_imgImage == nullptr || m_ciaImage == nullptr || !m_imagesLoaded)
        return;

    // 启用图像平滑，但只在需要时
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    painter->drawImage(m_projRect, *m_imgImage);
    painter->drawImage(m_projRect, *m_ciaImage);
}

void OfflineTile::loadImages(const QGV::GeoTilePos &tilePos)
{
    QString mapPath = Settings::instance()->getValue(SettingsKey::OfflineTilePath).toString();

    QString imgImagesPath = mapPath + "/img";
    QString imgImagePath = imgImagesPath + "/" + QString::number(tilePos.zoom())
                           + "/" + QString::number(tilePos.pos().x())
                           + "/" + QString::number(tilePos.pos().y()) + ".jpg";
    if(QFile::exists(imgImagePath)) {
        m_imgImage = new QImage(imgImagePath);
    }

    QString ciaImagesPath = mapPath + "/cia";
    QString ciaImagePath = ciaImagesPath + "/" + QString::number(tilePos.zoom())
                           + "/" + QString::number(tilePos.pos().x())
                           + "/" + QString::number(tilePos.pos().y()) + ".jpg";
    if(QFile::exists(ciaImagePath)) {
        m_ciaImage = new QImage(ciaImagePath);
    }

    m_imagesLoaded = true;
}
