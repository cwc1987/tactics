#include "situationwidget.h"
#include <QTimer>
#include <QLayout>
#include <QGVWidgetCompass.h>
#include <QGVMapQGView.h>
#include "customscale.h"
#include "settings.h"
#include "offlinetiles.h"
#include "airplane.h"
#include "airplanetrack.h"
#include "areaselect.h"
#include "areaselectitem.h"
#include "areamodel.h"
#include "airplanemodel.h"
// #include "targetmodel.h"
// #include "communitionmodel.h"
// #include "missionmodel.h"
#include "area.h"

SituationWidget::SituationWidget(QWidget *parent)
    : QGVMap(parent), m_mousePosText(nullptr), m_actionLayer(nullptr), m_equipLayer(nullptr),
    m_trackLayer(nullptr), m_connectionLayer(nullptr), m_selectItem(nullptr)
{
    setWindowTitle(tr("Situation Display"));
    initTiles();
    initControlsWidgets();
    initLayers();
    setupMapBoundary();
    initActions();
    initPostion();
}

SituationWidget::~SituationWidget()
{

}

void SituationWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (hasMouseTracking()) {
        QPointF projPos = mapToProj(event->pos());
        QGV::GeoPos geoPos = this->getProjection()->projToGeo(projPos);
        m_mousePosText->setText(QString(tr("Latitude:<b>%1, Longitude:%2</b>")).arg(geoPos.latToString()).arg(geoPos.lonToString()));
    }
    QGVMap::mouseMoveEvent(event);
}

void SituationWidget::onAreaAdded(const AreaItem &area)
{
    Area *areaItem = new Area();
    areaItem->setAreaData(area);
    m_areaLayer->addItem(areaItem);
}

void SituationWidget::onAreaRemoved(const AreaItem &area)
{
    for(int i = 0, iend = m_areaLayer->countItems(); i < iend; i++) {
        Area *areaItem = qobject_cast<Area*>(m_areaLayer->getItem(i));
        if(areaItem != nullptr && areaItem->getId() == area.id) {
            m_areaLayer->removeItem(areaItem);
            delete areaItem;
        }
    }
}

void SituationWidget::onAreaUpdated(const AreaItem &area)
{
    for(int i = 0, iend = m_areaLayer->countItems(); i < iend; i++) {
        Area *areaItem = qobject_cast<Area*>(m_areaLayer->getItem(i));
        if(areaItem != nullptr && areaItem->getId() == area.id) {
            areaItem->setAreaData(area);
            if(area.isActive) {
                QList<AreaItem::CoordinatePoint> coordinates = area.coordinates;
                AreaItem::CoordinatePoint centerPoint;
                if (!coordinates.isEmpty()) {
                    double sumLat = 0.0, sumLon = 0.0, sumAlt = 0.0;
                    for(int j = 0, jend = coordinates.count(); j < jend; j++) {
                        sumLat += coordinates[j].latitude;
                        sumLon += coordinates[j].longitude;
                        sumAlt += coordinates[j].altitude;
                    }
                    int count = coordinates.count();
                    centerPoint.latitude = sumLat / count;
                    centerPoint.longitude = sumLon / count;
                    centerPoint.altitude = sumAlt / count;
                    centerPoint.id = "center";
                }
                this->flyTo(QGVCameraActions(this).
                            moveTo(QGV::GeoPos(centerPoint.latitude,centerPoint.longitude)));
            }
        }
    }
}

void SituationWidget::onAreaReset()
{
    for(int i = 0, iend = m_areaLayer->countItems(); i < iend; i++) {
        Area *areaItem = qobject_cast<Area*>(m_areaLayer->getItem(i));
        if(areaItem != nullptr) {
            m_areaLayer->removeItem(areaItem);
            delete areaItem;
        }
    }

    QAbstractItemModel *model = ModelConfig::instance()
                                    ->getModel(ModelConfig::ModelType::Area);
    AreaModel *areaModel = qobject_cast<AreaModel*>(model);
    if(areaModel != nullptr) {
        QList<AreaItem> areas = areaModel->getAreas();
        for(int i = 0, iend = areas.count(); i < iend; i++) {
            Area *areaItem = new Area();
            areaItem->setAreaData(areas[i]);
            m_areaLayer->addItem(areaItem);
        }
    }
}

void SituationWidget::onAirplaneAdded(const AirPlaneItem &airplane) 
{
    Airplane *airplaneItem = new Airplane();
    airplaneItem->setAirplaneData(airplane);
    m_equipLayer->addItem(airplaneItem);

    AirplaneTrack *airplaneTrack = new AirplaneTrack(airplaneItem);
    m_trackLayer->addItem(airplaneTrack);
}

void SituationWidget::onAirplaneRemoved(const QString &id) 
{
    //delete air track
    for(int i = 0, iend = m_trackLayer->countItems(); i < iend; i++) {
        AirplaneTrack *airplaneTrack = qobject_cast<AirplaneTrack*>(m_trackLayer->getItem(i));
        if(airplaneTrack != nullptr && airplaneTrack->getAirplane()->getId() == id) {
            m_trackLayer->removeItem(airplaneTrack);
            delete airplaneTrack;
        }
    }

    //delete air plane
    for(int i = 0, iend = m_equipLayer->countItems(); i < iend; i++) {
        Airplane *airplaneItem = qobject_cast<Airplane*>(m_equipLayer->getItem(i));
        if(airplaneItem != nullptr && airplaneItem->getId() == id) {
            m_equipLayer->removeItem(airplaneItem);
            delete airplaneItem;
        }
    }
}

void SituationWidget::onAirplaneUpdated(const AirPlaneItem &airplane)
{
    for(int i = 0, iend = m_equipLayer->countItems(); i < iend; i++) {
        Airplane *airplaneItem = qobject_cast<Airplane*>(m_equipLayer->getItem(i));
        if(airplaneItem != nullptr && airplaneItem->getId() == airplane.id) {
            airplaneItem->setAirplaneData(airplane);
            if(airplaneItem->isActive()) {
                this->flyTo(QGVCameraActions(this). moveTo(airplaneItem->position()));
            }
        }
    }
}

void SituationWidget::onAirplaneReset()
{
    //delete track
    for(int i = 0, iend = m_trackLayer->countItems(); i < iend; i++) {
        AirplaneTrack *airplaneTrack = qobject_cast<AirplaneTrack*>(m_trackLayer->getItem(i));
        if(airplaneTrack != nullptr) {
            m_trackLayer->removeItem(airplaneTrack);
            delete airplaneTrack;
        }
    }

    //delete airplane
    for(int i = 0, iend = m_equipLayer->countItems(); i < iend; i++) {
        Airplane *airplaneItem = qobject_cast<Airplane*>(m_equipLayer->getItem(i));
        if(airplaneItem != nullptr) {
            m_equipLayer->removeItem(airplaneItem);
            delete airplaneItem;
        }
    }
}

void SituationWidget::initControlsWidgets()
{
    this->addWidget(new QGVWidgetCompass());
    this->addWidget(new CustomScale());
    AreaSelect *areaSelect = new AreaSelect();
    connect(areaSelect, &AreaSelect::taskAreaSelected, this, &SituationWidget::onTaskAreaSelected);
    connect(areaSelect, &AreaSelect::noFlyAreaSelected, this, &SituationWidget::onNoFlayAreaSelected);
    connect(areaSelect, &AreaSelect::threatAreaSelected, this, &SituationWidget::onThreatAreaSelected);
    this->addWidget(areaSelect);

    this->setMouseTracking(true);
    m_mousePosText = new QGVWidgetText();
    m_mousePosText->setAnchor(QPoint(0, 0), { Qt::TopEdge });
    this->addWidget(m_mousePosText);
    QColor textColor = Settings::instance()->getColor(SettingsKey::ControlsTextColor);
    QString textColorStyle = QString("color: rgb(%1,%2,%3);").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
    QColor backgroundColor = Settings::instance()->getColor(SettingsKey::ControlsBackGroundColor);
    QString backgroundColorStyle = QString("background-color: rgba(%1, %2, %3, %4);").
                                   arg(backgroundColor.red()).arg(backgroundColor.green()).
                                   arg(backgroundColor.blue()).arg(backgroundColor.alpha());
    m_mousePosText->label()->setStyleSheet(textColorStyle+backgroundColorStyle);
}

void SituationWidget::initLayers()
{
    m_equipLayer = new QGVLayer();
    m_equipLayer->setName(tr("Equipment Layer"));
    m_equipLayer->setZValue(EquieLayer);
    this->addItem(m_equipLayer);

    m_connectionLayer = new QGVLayer();
    m_connectionLayer->setName(tr("Communication Link Layer"));
    m_connectionLayer->setZValue(ConnectionLayer);
    this->addItem(m_connectionLayer);

    m_trackLayer = new QGVLayer();
    m_trackLayer->setName(tr("Track Layer"));
    m_trackLayer->setZValue(TrackLayer);
    this->addItem(m_trackLayer);

    m_actionLayer = new QGVLayer();
    m_actionLayer->setName(tr("Action Layer"));
    m_actionLayer->setZValue(ActionLayer);
    this->addItem(m_actionLayer);

    m_areaLayer = new QGVLayer();
    m_areaLayer->setName(tr("Area Layer"));
    m_areaLayer->setZValue(AreaLayer);
    this->addItem(m_areaLayer);
}

void SituationWidget::initTiles()
{
    auto offTiles = new OfflineTiles();
    this->addItem(offTiles);
}

void SituationWidget::initPostion()
{
    QTimer::singleShot(100, this, [this]() {
        double initLat = Settings::instance()->getValue(SettingsKey::InitLatitude).toDouble();
        double initLon = Settings::instance()->getValue(SettingsKey::InitLongitude).toDouble();
        float initScale = Settings::instance()->getValue(SettingsKey::InitScale).toFloat();
        QGV::GeoPos beijingPos(initLat, initLon);
        this->flyTo(QGVCameraActions(this).scaleTo(initScale).moveTo(beijingPos));
    });
}

void SituationWidget::setupMapBoundary()
{
    // Get world boundary
    QGV::GeoRect worldBoundary = this->getProjection()->boundaryGeoRect();
    QRectF worldProjRect = this->getProjection()->geoToProj(worldBoundary);

    // Set scale limits
    const double minScale = Settings::instance()->getValue(SettingsKey::MinScale).toDouble();  // 最小缩放比
    const double maxScale = Settings::instance()->getValue(SettingsKey::MaxScale).toDouble();    // 最大缩放比
    this->geoView()->setScaleLimits(minScale, maxScale);

    // Calculate draggable area based on minimum scale
    // At minimum scale, the view should be able to show the entire world
    const double viewWidth = Settings::instance()->getValue(SettingsKey::ViewWidth).toDouble();   // 视图宽度
    const double viewHeight = Settings::instance()->getValue(SettingsKey::ViewHeight).toDouble();  // 视图高度

    // Calculate the maximum draggable area at minimum scale
    // This ensures that at minimum zoom, the entire world is visible
    double maxDraggableWidth = worldProjRect.width() - (viewWidth / minScale);
    double maxDraggableHeight = worldProjRect.height() - (viewHeight / minScale);

    // Ensure draggable area is not negative
    maxDraggableWidth = qMax(0.0, maxDraggableWidth);
    maxDraggableHeight = qMax(0.0, maxDraggableHeight);

    // Calculate scene rect with proper margins
    QRectF sceneRect = worldProjRect;

    // Add margins to allow some dragging beyond world boundary
    const double marginX = maxDraggableWidth * Settings::instance()->getValue(SettingsKey::ViewMargin).toDouble();
    const double marginY = maxDraggableHeight * Settings::instance()->getValue(SettingsKey::ViewMargin).toDouble();

    sceneRect.adjust(-marginX, -marginY, +marginX, +marginY);

    // Set the scene rect
    this->geoView()->scene()->setSceneRect(sceneRect);
}

void SituationWidget::initActions()
{
    m_selectItem = new AreaSelectItem();
    connect(m_selectItem, &AreaSelectItem::selectionCompleted, this, &SituationWidget::onCompleteSelection);
    m_actionLayer->addItem(m_selectItem);
}

void SituationWidget::onTaskAreaSelected()
{
    m_selectItem->clearSelection();
    m_selectItem->startSelection(static_cast<int>(AreaItem::TaskArea));
}

void SituationWidget::onNoFlayAreaSelected()
{
    m_selectItem->clearSelection();
    m_selectItem->startSelection(static_cast<int>(AreaItem::NoFlyZone));
}

void SituationWidget::onThreatAreaSelected()
{
    m_selectItem->clearSelection();
    m_selectItem->startSelection(static_cast<int>(AreaItem::ThreatZone));
}

void SituationWidget::onCompleteSelection(QVector<QPointF> projs, int type)
{
    QVector<QGV::GeoPos> geoPosVec;
    for(int i = 0, iend = projs.count(); i < iend; i++) {
        QGV::GeoPos geoPos = this->getProjection()->projToGeo(projs[i]);
        geoPosVec.append(geoPos);
    }
    emit newSelectionArea(geoPosVec, type);
    m_selectItem->clearSelection();
}
