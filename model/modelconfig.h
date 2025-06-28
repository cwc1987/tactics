#ifndef MODELCONFIG_H
#define MODELCONFIG_H

#include <QObject>
#include <QAbstractItemModel>
#include <QGVGlobal.h>
//胶水层

class AirplaneModel;
class AirPlaneItem;
class AreaModel;
class AreaItem;
class MissionModel;
class MissionItem;
class CommunitionModel;
class CommunitionItem;
class TargetModel;
class TargetItem;
class ModelConfig : public QObject
{
    Q_OBJECT
public:
    enum ModelType {
        Airplane,
        Area,
        Mission,
        Communition,
        Target
    };

    static ModelConfig* instance();
    void initModels();

    QAbstractItemModel *getModel(const ModelType &type);
signals:
    void areaAdded(const AreaItem &area);
    void areaRemoved(const AreaItem &area);
    void areaUpdated(const AreaItem &area);
    void areaReset();

    void airplaneAdded(const AirPlaneItem &airplane);
    void airplaneRemoved(const QString &id);
    void airplaneUpdated(const AirPlaneItem &airplane);
    void airplaneReset();

    void missionAdded(const MissionItem &mission);
    void missionRemoved(const MissionItem &mission);
    void missionUpdated(const MissionItem &mission);
    void missionReset();

    void communitionAdded(const CommunitionItem &communition);
    void communitionRemoved(const CommunitionItem &communition);
    void communitionUpdated(const CommunitionItem &communition);
    void communitionReset();    

    void targetAdded(const TargetItem &target);
    void targetRemoved(const TargetItem &target);
    void targetUpdated(const TargetItem &target);
    void targetReset();
public slots:
    void onNewSelectionArea(QVector<QGV::GeoPos> geoPosVec, int type);
    void onAirplaneInit(const QList<AirPlaneItem> &items);
    void onAirplaneUpdated(const AirPlaneItem &airplane);
    
private:
    explicit ModelConfig(QObject *parent = nullptr);
    void initModelSignals();

    // id generate
    // Generate unique ID for AreaItem (0-255)
    int generateAreaItemId() const;

    // Generate Colors
    QString getAreaColor(int type) const;


    static ModelConfig *m_instance;

    AirplaneModel *m_airplaneModel;
    AreaModel *m_areaModel;
    MissionModel *m_missionModel;
    CommunitionModel *m_communitionModel;
    TargetModel *m_targetModel;
};

#endif // MODELCONFIG_H
