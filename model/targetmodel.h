#ifndef TARGETMODEL_H
#define TARGETMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QDateTime>
#include <QColor>

class TargetItem {
public:
    // 基本信息
    QString id;                     // 目标唯一标识
    QString name;                   // 目标名称
    QString model;                  // 目标型号
    
    // 目标类型枚举
    enum TargetType {
        // 敌方目标类型
        EnemyTank = 0,                   // 敌方坦克
        EnemyArmoredVehicle = 1,         // 敌方装甲车
        // 我方目标类型
        FriendlyRocket = 2,              // 我方火箭弹
        FriendlyGroundStation = 3,      // 我方地面站
        FriendlyProtectionVehicle = 4, // 我方目保车
        UnknownType = 5                // 未知类型
    };
    TargetType type;                // 目标类型
    
    // 目标状态
    enum TargetState {
        Normal = 0,                 //正常
        Damage = 1,                 //损坏
        Destoried = 2,               //摧毁
        UnkonwnState = 3            //未知状态
    };
    TargetState state;             // 当前状态

    // 位置信息
    double latitude;                // 纬度
    double longitude;               // 经度
    double altitude;                // 高度(米)
    double heading;                 // 航向角(度)
    double speed;                   // 速度(米/秒)
    double verticalSpeed;           // 垂直速度(米/秒)
    
    // 威胁信息
    enum ThreatLevel {
        Low = 0,                    // 低威胁
        Medium = 1,                 // 中等威胁
        High = 2,                   // 高威胁
        Critical = 3                // 严重威胁
    };
    ThreatLevel threatLevel;        // 威胁等级
    double confidence;              // 置信度
    
    // 探测信息
    bool isDetected;                // 是否被探测到
    bool isTracked;                 // 是否被跟踪
    double detectionRange;          // 探测范围(米)
    double weaponRange;             // 武器射程(米)
    QDateTime firstDetectionTime;   // 首次探测时间
    QDateTime lastDetectionTime;    // 最后探测时间
    
    // 武器系统
    QStringList weaponSystems;      // 武器系统列表
    int missileCount;               // 导弹数量
    
    // 通信信息
    QString communicationType;      // 通信类型
    double communicationRange;      // 通信范围
    
    // 构造函数
    TargetItem();
    TargetItem(const QString& id, const QString& name, TargetType type);
    
    // 获取状态字符串
    QString getStateString() const;
    // 获取威胁等级字符串
    QString getThreatLevelString() const;
    // 获取目标类型字符串
    QString getTypeString() const;
    // 获取状态颜色
    QColor getStateColor() const;
    // 获取威胁等级颜色
    QColor getThreatLevelColor() const;
    // 获取目标类型颜色
    QColor getTypeColor() const;
    // 更新位置信息
    void updatePosition(double lat, double lon, double alt);
    // 更新状态
    void updateState(TargetState newState);
    // 更新威胁等级
    void updateThreatLevel(ThreatLevel newLevel);
    // 检查是否可跟踪
    bool isTrackable() const;
    // 计算威胁分数
    double calculateThreatScore() const;
    // 检查是否在武器射程内
    bool isInWeaponRange(double distance) const;
    // 检查是否在探测范围内
    bool isInDetectionRange(double distance) const;
    // 检查目标类型是否为敌方
    bool isEnemyType() const;
    // 检查目标类型是否为友方
    bool isFriendlyType() const;
};

class TargetModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TargetModel(QObject *parent = nullptr);
    
    // 表格模型必需的方法
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    
    // 数据管理方法
    void addTarget(const TargetItem &target);
    void removeTarget(int row);
    void removeTargetById(const QString &id);
    void updateTarget(int row, const TargetItem &target);
    void updateTargetById(const QString &id, const TargetItem &target);
    TargetItem getTarget(int row) const;
    TargetItem getTargetById(const QString &id) const;
    int findTargetById(const QString &id) const;
    void clearAll();
    
    // 数据查询方法
    QList<TargetItem> getDetectedTargets() const;
    QList<TargetItem> getTrackedTargets() const;
    QList<TargetItem> getTargetsByState(TargetItem::TargetState state) const;
    QList<TargetItem> getTargetsByType(TargetItem::TargetType type) const;
    QList<TargetItem> getTargetsByThreatLevel(TargetItem::ThreatLevel level) const;
    QList<TargetItem> getEnemyTypeTargets() const;
    QList<TargetItem> getFriendlyTypeTargets() const;
    
    // 批量操作方法
    void updatePositions(const QMap<QString, QPair<double, double>> &positions);
    void updateStates(const QMap<QString, TargetItem::TargetState> &states);
    void updateThreatLevels(const QMap<QString, TargetItem::ThreatLevel> &levels);
    
    // 获取所有数据
    QList<TargetItem> getAllTargets() const { return m_targets; }
    
signals:
    void targetAdded(const TargetItem &target);
    void targetRemoved(const QString &id);
    void targetUpdated(const TargetItem &target);

private:
    QList<TargetItem> m_targets;
    
    // 列定义
    enum Columns {
        Id = 0,
        Name,
        Type,
        Model,
        Latitude,
        Longitude,
        Altitude,
        Heading,
        Speed,
        State,
        ThreatLevel,
        Confidence,
        IsDetected,
        IsTracked,
        DetectionRange,
        WeaponRange,
        MissileCount,
        PlatformType,
        LastDetection,
        ColumnCount
    };
    
    // 辅助方法
    QString formatCoordinate(double coord) const;
    QString formatDateTime(const QDateTime &datetime) const;
    QString formatPercentage(double value) const;
    QString formatSpeed(double speed) const;
    QString formatAltitude(double altitude) const;
    QString formatRange(double range) const;
    QColor getTextColorForBackground(const QColor &background) const;
};

#endif // TARGETMODEL_H 
