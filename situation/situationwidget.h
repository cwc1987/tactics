#ifndef SITUATIONWIDGET_H
#define SITUATIONWIDGET_H

#include <QMouseEvent>
#include <QGVMap.h>
#include <QGVWidgetText.h>
#include <QGVLayer.h>
#include "modelconfig.h"

class AreaSelectItem;
class AreaItem;
class SituationWidget : public QGVMap
{
    Q_OBJECT
public:
    enum LayerType {
        ActionLayer = 200,
        EquieLayer = 100,
        ConnectionLayer = 95,
        TrackLayer = 90,
        AreaLayer = 80,
    };

    SituationWidget(QWidget *parent = nullptr);
    ~SituationWidget();

protected:
    void mouseMoveEvent(QMouseEvent* event) override;

public slots:
    //area signals
    void onAreaAdded(const AreaItem &area);
    void onAreaRemoved(const AreaItem &area);
    void onAreaUpdated(const AreaItem &area);
    void onAreaReset();
    
    //airplane signals
    void onAirplaneAdded(const AirPlaneItem &airplane);
    void onAirplaneRemoved(const QString &id);
    void onAirplaneUpdated(const AirPlaneItem &airplane);
    void onAirplaneReset();

private:
    void initControlsWidgets();
    void initLayers();
    void initTiles();
    void initPostion();
    void setupMapBoundary();
    void initActions();

signals:
    void newSelectionArea(QVector<QGV::GeoPos> geoPosVec, int type);

private slots:
    void onTaskAreaSelected();
    void onNoFlayAreaSelected();
    void onThreatAreaSelected();
    void onCompleteSelection(QVector<QPointF> projs, int type);

private:
    QGVWidgetText* m_mousePosText;
    QGVLayer *m_actionLayer;
    QGVLayer *m_equipLayer;
    QGVLayer *m_trackLayer;
    QGVLayer *m_connectionLayer;
    QGVLayer *m_areaLayer;

    AreaSelectItem *m_selectItem;
};
#endif // SITUATIONWIDGET_H
