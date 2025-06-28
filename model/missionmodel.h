#ifndef MISSIONMODEL_H
#define MISSIONMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QPointF>

class MissionItem {
public:
    // 战术类型枚举
    enum class TacticalType {
        RadarOpticalPatrol,      // Radar Optical Patrol
        OpticalPatrol,           // Optical Patrol
        OpticalHorizontalFormation, // Optical Horizontal Formation
        OpticalVerticalFormation,   // Optical Vertical Formation
        TargetOpticalDetail,     // Target Optical Detail
        TargetBeamDetail,        // Target Beam Detail
        CruiseStandby,           // Cruise Standby
        FormationCruiseStandby,  // Formation Cruise Standby
        SingleForward,           // Single Forward
        FormationForward,        // Formation Forward
        ReturnRecovery,          // Return Recovery
        FormationReturnRecovery  // Formation Return Recovery
    };

    // 出动形式枚举
    enum class SortieType {
        Single,      // Single
        Formation    // Formation
    };

    // 编队模式枚举
    enum class FormationMode {
        Horizontal,  // Horizontal
        Vertical,    // Vertical
        Diamond,     // Diamond
        VShape       // V Shape
    };

    // 长僚身份枚举
    enum class LeaderRole {
        Leader,      // Leader
        Wingman      // Wingman
    };

    MissionItem();
    
    // 基本属性
    QString droneId;             // 无人机编号
    QString missionId;           // 任务ID
    QDateTime startTime;         // 开始时刻
    QDateTime endTime;           // 结束时刻
    TacticalType tacticalType;   // 战术类型
    
    // 出动和编队信息
    SortieType sortieType;       // 出动形式
    FormationMode formationMode; // 编队模式
    LeaderRole leaderRole;       // 长僚身份
    QString cooperativeDroneId;  // 协同机号
    double formationDistance;    // 编队间距
    double formationHeightDiff;  // 编队高差
    
    // 任务区域信息
    QString taskAreaId;          // 任务区坐标（id）
    double heightUpper;          // 高度范围上
    double heightLower;          // 高度范围下
    QPointF entryPoint;          // 任务区进入点
    QPointF exitPoint;           // 任务区飞出点
    
    // 辅助方法
    QString getTacticalTypeString() const;
    QString getSortieTypeString() const;
    QString getFormationModeString() const;
    QString getLeaderRoleString() const;
};

class MissionModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MissionModel(QObject *parent = nullptr);
    
    // 列定义
    enum Columns {
        DroneId = 0,
        MissionId,
        StartTime,
        EndTime,
        TacticalType,
        SortieType,
        FormationMode,
        LeaderRole,
        CooperativeDroneId,
        FormationDistance,
        FormationHeightDiff,
        TaskAreaId,
        HeightUpper,
        HeightLower,
        EntryPoint,
        ExitPoint,
        ColumnCount
    };
    
    // QAbstractTableModel 接口
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    
    // 数据操作方法
    void addMission(const MissionItem &mission);
    void removeMission(int row);
    void updateMission(int row, const MissionItem &mission);
    MissionItem getMission(int row) const;
    void clearAll();
    
    // 数据访问
    const QList<MissionItem>& getMissions() const { return m_items; }
    void setMissions(const QList<MissionItem> &missions);
    
    // JSON import/export methods
    bool exportToJson(const QString &filePath) const;
    bool importFromJson(const QString &filePath);
    QString exportToJsonString() const;
    bool importFromJsonString(const QString &jsonString);

private:
    QList<MissionItem> m_items;
};

#endif // MISSIONMODEL_H
