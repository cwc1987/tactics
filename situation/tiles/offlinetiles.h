#ifndef OFFLINETILES_H
#define OFFLINETILES_H

#include <QGVLayerTiles.h>

class OfflineTiles : public QGVLayerTiles
{
    Q_OBJECT

public:
    OfflineTiles();

private:
    int minZoomlevel() const override final;
    int maxZoomlevel() const override final;
    void request(const QGV::GeoTilePos& tilePos) override final;
    void cancel(const QGV::GeoTilePos& tilePos) override;
};

#endif // OFFLINETILES_H
