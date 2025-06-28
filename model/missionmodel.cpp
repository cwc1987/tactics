#include "missionmodel.h"
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

// MissionItem 构造函数
MissionItem::MissionItem()
    : tacticalType(TacticalType::OpticalPatrol)
    , sortieType(SortieType::Single)
    , formationMode(FormationMode::Horizontal)
    , leaderRole(LeaderRole::Leader)
    , formationDistance(0.0)
    , formationHeightDiff(0.0)
    , heightUpper(0.0)
    , heightLower(0.0)
{
}

// MissionItem 辅助方法实现
QString MissionItem::getTacticalTypeString() const
{
    switch (tacticalType) {
        case TacticalType::RadarOpticalPatrol: return QObject::tr("Radar Optical Patrol");
        case TacticalType::OpticalPatrol: return QObject::tr("Optical Patrol");
        case TacticalType::OpticalHorizontalFormation: return QObject::tr("Optical Horizontal Formation");
        case TacticalType::OpticalVerticalFormation: return QObject::tr("Optical Vertical Formation");
        case TacticalType::TargetOpticalDetail: return QObject::tr("Target Optical Detail");
        case TacticalType::TargetBeamDetail: return QObject::tr("Target Beam Detail");
        case TacticalType::CruiseStandby: return QObject::tr("Cruise Standby");
        case TacticalType::FormationCruiseStandby: return QObject::tr("Formation Cruise Standby");
        case TacticalType::SingleForward: return QObject::tr("Single Forward");
        case TacticalType::FormationForward: return QObject::tr("Formation Forward");
        case TacticalType::ReturnRecovery: return QObject::tr("Return Recovery");
        case TacticalType::FormationReturnRecovery: return QObject::tr("Formation Return Recovery");
        default: return QObject::tr("Unknown");
    }
}

QString MissionItem::getSortieTypeString() const
{
    switch (sortieType) {
        case SortieType::Single: return QObject::tr("Single");
        case SortieType::Formation: return QObject::tr("Formation");
        default: return QObject::tr("Unknown");
    }
}

QString MissionItem::getFormationModeString() const
{
    switch (formationMode) {
        case FormationMode::Horizontal: return QObject::tr("Horizontal");
        case FormationMode::Vertical: return QObject::tr("Vertical");
        case FormationMode::Diamond: return QObject::tr("Diamond");
        case FormationMode::VShape: return QObject::tr("V Shape");
        default: return QObject::tr("Unknown");
    }
}

QString MissionItem::getLeaderRoleString() const
{
    switch (leaderRole) {
        case LeaderRole::Leader: return QObject::tr("Leader");
        case LeaderRole::Wingman: return QObject::tr("Wingman");
        default: return QObject::tr("Unknown");
    }
}

// MissionModel 构造函数
MissionModel::MissionModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

// QAbstractTableModel 接口实现
int MissionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

int MissionModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant MissionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const MissionItem &item = m_items[index.row()];
        
        switch (index.column()) {
            case DroneId: return item.droneId;
            case MissionId: return item.missionId;
            case StartTime: return item.startTime.toString("yyyy-MM-dd hh:mm:ss");
            case EndTime: return item.endTime.toString("yyyy-MM-dd hh:mm:ss");
            case TacticalType: return item.getTacticalTypeString();
            case SortieType: return item.getSortieTypeString();
            case FormationMode: return item.getFormationModeString();
            case LeaderRole: return item.getLeaderRoleString();
            case CooperativeDroneId: return item.cooperativeDroneId;
            case FormationDistance: return QString::number(item.formationDistance);
            case FormationHeightDiff: return QString::number(item.formationHeightDiff);
            case TaskAreaId: return item.taskAreaId;
            case HeightUpper: return QString::number(item.heightUpper);
            case HeightLower: return QString::number(item.heightLower);
            case EntryPoint: return QString("(%1, %2)").arg(item.entryPoint.x()).arg(item.entryPoint.y());
            case ExitPoint: return QString("(%1, %2)").arg(item.exitPoint.x()).arg(item.exitPoint.y());
            default: return QVariant();
        }
    }
    
    return QVariant();
}

QVariant MissionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case DroneId: return tr("Drone ID");
            case MissionId: return tr("Mission ID");
            case StartTime: return tr("Start Time");
            case EndTime: return tr("End Time");
            case TacticalType: return tr("Tactical Type");
            case SortieType: return tr("Sortie Type");
            case FormationMode: return tr("Formation Mode");
            case LeaderRole: return tr("Leader Role");
            case CooperativeDroneId: return tr("Cooperative Drone ID");
            case FormationDistance: return tr("Formation Distance");
            case FormationHeightDiff: return tr("Formation Height Diff");
            case TaskAreaId: return tr("Task Area ID");
            case HeightUpper: return tr("Height Upper");
            case HeightLower: return tr("Height Lower");
            case EntryPoint: return tr("Entry Point");
            case ExitPoint: return tr("Exit Point");
            default: return QVariant();
        }
    }
    
    return QVariant();
}

// 数据操作方法实现
void MissionModel::addMission(const MissionItem &mission)
{
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(mission);
    endInsertRows();
}

void MissionModel::removeMission(int row)
{
    if (row < 0 || row >= m_items.size())
        return;
        
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

void MissionModel::updateMission(int row, const MissionItem &mission)
{
    if (row < 0 || row >= m_items.size())
        return;
        
    m_items[row] = mission;
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
}

MissionItem MissionModel::getMission(int row) const
{
    if (row >= 0 && row < m_items.size())
        return m_items[row];
    return MissionItem();
}

void MissionModel::clearAll()
{
    if (m_items.isEmpty())
        return;
        
    beginResetModel();
    m_items.clear();
    endResetModel();
}

void MissionModel::setMissions(const QList<MissionItem> &missions)
{
    beginResetModel();
    m_items = missions;
    endResetModel();
}

// JSON import/export methods implementation
bool MissionModel::exportToJson(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    QString jsonString = exportToJsonString();
    QTextStream stream(&file);
    stream << jsonString;
    file.close();
    
    return true;
}

bool MissionModel::importFromJson(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QTextStream stream(&file);
    QString jsonString = stream.readAll();
    file.close();
    
    return importFromJsonString(jsonString);
}

QString MissionModel::exportToJsonString() const
{
    QJsonArray missionsArray;
    
    for (const MissionItem &mission : m_items) {
        QJsonObject missionObject;
        
        // Basic properties
        missionObject["droneId"] = mission.droneId;
        missionObject["missionId"] = mission.missionId;
        missionObject["startTime"] = mission.startTime.toString(Qt::ISODate);
        missionObject["endTime"] = mission.endTime.toString(Qt::ISODate);
        missionObject["tacticalType"] = static_cast<int>(mission.tacticalType);
        
        // Sortie and formation information
        missionObject["sortieType"] = static_cast<int>(mission.sortieType);
        missionObject["formationMode"] = static_cast<int>(mission.formationMode);
        missionObject["leaderRole"] = static_cast<int>(mission.leaderRole);
        missionObject["cooperativeDroneId"] = mission.cooperativeDroneId;
        missionObject["formationDistance"] = mission.formationDistance;
        missionObject["formationHeightDiff"] = mission.formationHeightDiff;
        
        // Task area information
        missionObject["taskAreaId"] = mission.taskAreaId;
        missionObject["heightUpper"] = mission.heightUpper;
        missionObject["heightLower"] = mission.heightLower;
        
        // Entry and exit points
        QJsonObject entryPoint;
        entryPoint["x"] = mission.entryPoint.x();
        entryPoint["y"] = mission.entryPoint.y();
        missionObject["entryPoint"] = entryPoint;
        
        QJsonObject exitPoint;
        exitPoint["x"] = mission.exitPoint.x();
        exitPoint["y"] = mission.exitPoint.y();
        missionObject["exitPoint"] = exitPoint;
        
        missionsArray.append(missionObject);
    }
    
    QJsonObject rootObject;
    rootObject["missions"] = missionsArray;
    rootObject["version"] = "1.0";
    rootObject["exportTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    QJsonDocument doc(rootObject);
    return doc.toJson(QJsonDocument::Indented);
}

bool MissionModel::importFromJsonString(const QString &jsonString)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    
    if (error.error != QJsonParseError::NoError) {
        return false;
    }
    
    QJsonObject rootObject = doc.object();
    QJsonArray missionsArray = rootObject["missions"].toArray();
    
    QList<MissionItem> newMissions;
    
    for (const QJsonValue &value : missionsArray) {
        QJsonObject missionObject = value.toObject();
        MissionItem mission;
        
        // Basic properties
        mission.droneId = missionObject["droneId"].toString();
        mission.missionId = missionObject["missionId"].toString();
        mission.startTime = QDateTime::fromString(missionObject["startTime"].toString(), Qt::ISODate);
        mission.endTime = QDateTime::fromString(missionObject["endTime"].toString(), Qt::ISODate);
        mission.tacticalType = static_cast<MissionItem::TacticalType>(missionObject["tacticalType"].toInt());
        
        // Sortie and formation information
        mission.sortieType = static_cast<MissionItem::SortieType>(missionObject["sortieType"].toInt());
        mission.formationMode = static_cast<MissionItem::FormationMode>(missionObject["formationMode"].toInt());
        mission.leaderRole = static_cast<MissionItem::LeaderRole>(missionObject["leaderRole"].toInt());
        mission.cooperativeDroneId = missionObject["cooperativeDroneId"].toString();
        mission.formationDistance = missionObject["formationDistance"].toDouble();
        mission.formationHeightDiff = missionObject["formationHeightDiff"].toDouble();
        
        // Task area information
        mission.taskAreaId = missionObject["taskAreaId"].toString();
        mission.heightUpper = missionObject["heightUpper"].toDouble();
        mission.heightLower = missionObject["heightLower"].toDouble();
        
        // Entry and exit points
        QJsonObject entryPoint = missionObject["entryPoint"].toObject();
        mission.entryPoint = QPointF(entryPoint["x"].toDouble(), entryPoint["y"].toDouble());
        
        QJsonObject exitPoint = missionObject["exitPoint"].toObject();
        mission.exitPoint = QPointF(exitPoint["x"].toDouble(), exitPoint["y"].toDouble());
        
        newMissions.append(mission);
    }
    
    setMissions(newMissions);
    return true;
}
