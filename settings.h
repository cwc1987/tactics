#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QScopedPointer>
#include <QSettings>
#include <QList>
#include "QGeoView/QGVGlobal.h"

namespace SettingsKey {
    //map
    const QString MinScale = "min_scale";
    const QString MaxScale = "max_scale";
    const QString ViewWidth = "view_width";
    const QString ViewHeight = "view_height";
    const QString ViewMargin = "view_margin";
    const QString MaxZoomLevel = "max_zoom_level";
    const QString MinZoomLevel = "min_zoom_level";
    const QString ControlsBackGroundColor = "controls_backgroupd_color";
    const QString ControlsTextColor = "controls_text_color";
    const QString OfflineTilePath = "offline_tile_path";
    const QString InitLatitude = "init_latitude";//latitude
    const QString InitLongitude = "init_longitude";//longitude
    const QString InitScale = "init_scale";
    const QString ItemResourcesPath = "item_resources_path";//all images use PNG format
    const QString AirplaneSize = "airplane_size";
    const QString TankSize = "tank_size";
    const QString AirTrackSize = "air_track_size";
    const QString AirTrackColor = "air_track_color";
    const QString AirTrackWidth = "air_track_width";
    const QString AreaFontSize = "area_font_size";//area font size
    const QString AreaPenWidth = "area_pen_width";//area pen width
    const QString AreaSelectPointSize = "area_select_point_size";
    const QString AreaSelectMaxPoints = "area_select_max_points";
    const QString AreaSelectPointColor = "area_select_point_color";
    const QString AreaSelectPreviewLineWidth = "area_select_preview_line_width";
    const QString AreaSelectPreviewLineColor = "area_select_preview_line_color";
    const QString AreaSelectLineWidth = "area_select_line_width";
    const QString AreaSelectLineColor = "area_select_line_color";
    const QString RocketSize = "rocket_size";

    //model
    const QString TaskAreaColor = "task_area_color";
    const QString NoFlayAreaColor = "no_flay_area_color";
    const QString ThreatAreaColor = "threat_area_color";
    const QString AreaMax = "area_max";
};

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings* instance();

    void initSettings();
    QVariant getValue(const QString &key);
    QColor getColor(const QString &key);
private:
    explicit Settings(QObject *parent = nullptr);
    void initMapSetings();
    void initModelSettings();
    void cacheMapSettings();
    void cacheModelSettings();

    QColor transColor(const QString &value);
private:
    QSettings *m_settings;
    QHash<QString, QVariant> m_values;
    static Settings *m_instance;
};

#endif // SETTINGS_H
