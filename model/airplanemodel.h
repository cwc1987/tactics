#ifndef AIRPLANEMODEL_H
#define AIRPLANEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QDateTime>
#include <QColor>

class AirPlaneItem {
public:
    // 基本信息
    QString id;                     // 无人机唯一标识
    QString name;                   // 无人机名称
    QString type;                   // 无人机类型
    QString model;                  // 无人机型号
    
    // 位置信息
    double latitude;                // 纬度
    double longitude;               // 经度
    double altitude;                // 高度(米)
    double heading;                 // 航向角(度)
    double speed;                   // 速度(米/秒)
    double verticalSpeed;           // 垂直速度(米/秒)
    
    // 状态信息
    enum AirState {
        Normal = 0,                 // 正常
        Damage = 1,                 // 损坏
        Emergency = 2,              // 紧急
        Maintenance = 3,            // 维护
        Offline = 4                 // 离线
    };
    AirState state;                 // 当前状态
    
    // 飞行信息
    QDateTime takeoffTime;          // 起飞时间
    QDateTime landingTime;          // 降落时间
    double flightDuration;          // 飞行时长(分钟)
    double totalDistance;           // 总飞行距离(米)
    double remainingOil;            // 剩余油量(%)
    double signalStrength;          // 信号强度(%)
    
    // 传感器信息
    bool cameraOnline;              // 光电在线状态
    bool gpsOnline;                 // GPS在线状态
    bool radarOnline;               // 雷达在线状态
    
    // 控制信息
    enum AirControlState {
        Manual = 0,                  // 手动
        Auto = 1,                    // 自动
        Unknown = 3,                 // 未知
    };
    AirControlState controlState;   // 当前控制状态
    
    // 通信信息
    QString communicationChannel;   // 通信频道
    double communicationQuality;    // 通信质量
    QDateTime lastCommunication;    // 最后通信时间

    bool isActive;
    
    // 构造函数
    AirPlaneItem();
    AirPlaneItem(const QString& id, const QString& name);
    
    // 获取状态字符串
    QString getStateString() const;
    // 获取状态颜色
    QColor getStateColor() const;
    // 更新位置信息
    void updatePosition(double lat, double lon, double alt);
    // 更新状态
    void updateState(AirState newState);
    // 检查是否在线
    bool isOnline() const;
    // 检查是否可控制
    bool isControllable() const;
};

class AirplaneModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit AirplaneModel(QObject *parent = nullptr);
    
    // 表格模型必需的方法
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    
    // 数据管理方法
    void addAirplane(const AirPlaneItem &airplane);
    void removeAirplane(int row);
    void removeAirplaneById(const QString &id);
    void updateAirplane(int row, const AirPlaneItem &airplane);
    void updateAirplaneById(const QString &id, const AirPlaneItem &airplane);
    AirPlaneItem getAirplane(int row) const;
    AirPlaneItem getAirplaneById(const QString &id) const;
    int findAirplaneById(const QString &id) const;
    void clearAll();
    
    // 数据查询方法
    QList<AirPlaneItem> getOnlineAirplanes() const;
    QList<AirPlaneItem> getAirplanesByState(AirPlaneItem::AirState state) const;
    QList<AirPlaneItem> getControllableAirplanes() const;
    
    // 批量操作方法
    void updatePositions(const QMap<QString, QPair<double, double>> &positions);
    void updateStates(const QMap<QString, AirPlaneItem::AirState> &states);
    
    // 获取所有数据
    QList<AirPlaneItem> getAllAirplanes() const { return m_airplanes; }
    
signals:
    void airplaneAdded(const AirPlaneItem &airplane);
    void airplaneRemoved(const QString &id);
    void airplaneUpdated(const AirPlaneItem &airplane);
    void airplaneReset();

private:
    QList<AirPlaneItem> m_airplanes;
    
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
        ControlState,
        RemainingOil,
        SignalStrength,
        CameraOnline,
        GpsOnline,
        RadarOnline,
        CommunicationQuality,
        LastCommunication,
        ColumnCount
    };
    
    // 辅助方法
    QString formatCoordinate(double coord) const;
    QString formatDateTime(const QDateTime &datetime) const;
    QString formatPercentage(double value) const;
    QString formatSpeed(double speed) const;
    QString formatAltitude(double altitude) const;
    QString getControlStateString(AirPlaneItem::AirControlState state) const;
    QColor getControlStateColor(AirPlaneItem::AirControlState state) const;
    QColor getTextColorForBackground(const QColor &background) const;
};

#endif // AIRPLANEMODEL_H
