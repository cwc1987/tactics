QT       += core gui widgets network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += QGeoView \
               QGeoView/Raster \
               situation \
               situation/mapcontrols \
               situation/tiles \
               situation/items \
               model \
               views \
               simulation

SOURCES += \
    QGeoView/QGVCamera.cpp \
    QGeoView/QGVDrawItem.cpp \
    QGeoView/QGVGlobal.cpp \
    QGeoView/QGVItem.cpp \
    QGeoView/QGVLayer.cpp \
    QGeoView/QGVLayerBDGEx.cpp \
    QGeoView/QGVLayerBing.cpp \
    QGeoView/QGVLayerGoogle.cpp \
    QGeoView/QGVLayerOSM.cpp \
    QGeoView/QGVLayerTiles.cpp \
    QGeoView/QGVLayerTilesOnline.cpp \
    QGeoView/QGVMap.cpp \
    QGeoView/QGVMapQGItem.cpp \
    QGeoView/QGVMapQGView.cpp \
    QGeoView/QGVMapRubberBand.cpp \
    QGeoView/QGVProjection.cpp \
    QGeoView/QGVProjectionEPSG3857.cpp \
    QGeoView/QGVUtils.cpp \
    QGeoView/QGVWidget.cpp \
    QGeoView/QGVWidgetCompass.cpp \
    QGeoView/QGVWidgetScale.cpp \
    QGeoView/QGVWidgetText.cpp \
    QGeoView/QGVWidgetZoom.cpp \
    QGeoView/Raster/QGVIcon.cpp \
    QGeoView/Raster/QGVImage.cpp \
    main.cpp \
    model/airplanemodel.cpp \
    model/areamodel.cpp \
    model/communitionmodel.cpp \
    model/missionmodel.cpp \
    model/modelconfig.cpp \
    model/targetmodel.cpp \
    settings.cpp \
    simulation/airplanconfigwidget.cpp \
    simulation/airplanedatas.cpp \
    simulation/airplanesimulation.cpp \
    simulation/airplanesimulationwidget.cpp \
    simulation/simulations.cpp \
    simulation/simulationsettings.cpp \
    situation/items/airplane.cpp \
    situation/items/airplanetrack.cpp \
    situation/items/area.cpp \
    situation/items/areaselectitem.cpp \
    situation/items/armoredcar.cpp \
    situation/items/rocket.cpp \
    situation/items/tank.cpp \
    situation/mapcontrols/areaselect.cpp \
    situation/mapcontrols/customscale.cpp \
    situation/situationwidget.cpp \
    situation/tiles/offlinetile.cpp \
    situation/tiles/offlinetiles.cpp \
    views/mainview.cpp

HEADERS += \
    QGeoView/QGVCamera.h \
    QGeoView/QGVDrawItem.h \
    QGeoView/QGVGlobal.h \
    QGeoView/QGVItem.h \
    QGeoView/QGVLayer.h \
    QGeoView/QGVLayerBDGEx.h \
    QGeoView/QGVLayerBing.h \
    QGeoView/QGVLayerGoogle.h \
    QGeoView/QGVLayerOSM.h \
    QGeoView/QGVLayerTiles.h \
    QGeoView/QGVLayerTilesOnline.h \
    QGeoView/QGVMap.h \
    QGeoView/QGVMapQGItem.h \
    QGeoView/QGVMapQGView.h \
    QGeoView/QGVMapRubberBand.h \
    QGeoView/QGVProjection.h \
    QGeoView/QGVProjectionEPSG3857.h \
    QGeoView/QGVUtils.h \
    QGeoView/QGVWidget.h \
    QGeoView/QGVWidgetCompass.h \
    QGeoView/QGVWidgetScale.h \
    QGeoView/QGVWidgetText.h \
    QGeoView/QGVWidgetZoom.h \
    QGeoView/Raster/QGVIcon.h \
    QGeoView/Raster/QGVImage.h \
    model/airplanemodel.h \
    model/areamodel.h \
    model/communitionmodel.h \
    model/missionmodel.h \
    model/modelconfig.h \
    model/targetmodel.h \
    settings.h \
    simulation/airplanconfigwidget.h \
    simulation/airplanedatas.h \
    simulation/airplanesimulation.h \
    simulation/airplanesimulationwidget.h \
    simulation/simulations.h \
    simulation/simulationsettings.h \
    situation/items/airplane.h \
    situation/items/airplanetrack.h \
    situation/items/area.h \
    situation/items/areaselectitem.h \
    situation/items/armoredcar.h \
    situation/items/rocket.h \
    situation/items/tank.h \
    situation/mapcontrols/areaselect.h \
    situation/mapcontrols/customscale.h \
    situation/situationwidget.h \
    situation/tiles/offlinetile.h \
    situation/tiles/offlinetiles.h \
    views/mainview.h

FORMS += \
    views/mainview.ui

TRANSLATIONS += \
    tactics_zh_CN.ts

RESOURCES += \
    res.qrc
