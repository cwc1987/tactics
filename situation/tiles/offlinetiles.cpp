#include "offlinetiles.h"
#include "offlinetile.h"
#include "settings.h"

OfflineTiles::OfflineTiles()
{
}

int OfflineTiles::minZoomlevel() const
{
    return Settings::instance()->getValue(SettingsKey::MinZoomLevel).toInt();
}

int OfflineTiles::maxZoomlevel() const
{
    return Settings::instance()->getValue(SettingsKey::MaxZoomLevel).toInt();
}

void OfflineTiles::request(const QGV::GeoTilePos &tilePos)
{
    QGVDrawItem* tile = new OfflineTile(tilePos);
    tile->setSelectable(false);
    onTile(tilePos, tile);
}

void OfflineTiles::cancel(const QGV::GeoTilePos &tilePos)
{
    Q_UNUSED(tilePos)
    qt_noop();
}
