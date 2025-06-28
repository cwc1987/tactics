#ifndef COMMUNITIONMODEL_H
#define COMMUNITIONMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QDateTime>
#include <QColor>

class CommunitionItem {
public:
    // 基本信息
    QString id;                     // 通信关系唯一标识
    QString name;                   // 通信关系名称
    
    // 通信双方信息
    QString sourceId;               // 源设备ID（飞机或友方目标）
    QString sourceName;             // 源设备名称
    QString sourceType;             // 源设备类型（Airplane/FriendlyTarget）
    
    QString targetId;               // 目标设备ID
    QString targetName;             // 目标设备名称
    QString targetType;             // 目标设备类型（Airplane/FriendlyTarget）
    
    // 通信状态
    enum CommunicationState {
        Connected = 0,              // 已连接
        Connecting = 1,             // 连接中
        Disconnected = 2,           // 已断开
        Failed = 3,                 // 连接失败
        Unknown = 4                 // 未知状态
    };
    CommunicationState state;       // 当前通信状态
    
    // 通信质量信息
    double signalStrength;          // 信号强度(%)
    double communicationQuality;    // 通信质量(%)
    double bandwidth;               // 带宽(Mbps)
    double latency;                 // 延迟(ms)
    double packetLoss;              // 丢包率(%)
    
    // 时间信息
    QDateTime establishTime;        // 建立时间
    QDateTime lastActiveTime;       // 最后活跃时间
    double duration;                // 持续时间(分钟)

    // 位置信息（用于计算距离）
    double sourceLatitude;          // 源设备纬度
    double sourceLongitude;         // 源设备经度
    double sourceAltitude;          // 源设备高度
    
    double targetLatitude;          // 目标设备纬度
    double targetLongitude;         // 目标设备经度
    double targetAltitude;          // 目标设备高度
    
    // 通信范围
    double maxRange;                // 最大通信距离(米)
    double currentDistance;         // 当前距离(米)
    
    // 构造函数
    CommunitionItem();
    CommunitionItem(const QString& id, const QString& sourceId, const QString& targetId);
    
    // 获取状态字符串
    QString getStateString() const;
    // 获取通信类型字符串
    QString getTypeString() const;
    // 获取状态颜色
    QColor getStateColor() const;
    // 获取通信质量颜色
    QColor getQualityColor() const;
    // 更新通信状态
    void updateState(CommunicationState newState);
    // 更新通信质量
    void updateQuality(double quality, double signal, double latency, double packetLoss);
    // 更新位置信息
    void updateSourcePosition(double lat, double lon, double alt);
    void updateTargetPosition(double lat, double lon, double alt);
    // 计算当前距离
    void calculateDistance();
    // 检查是否在通信范围内
    bool isInRange() const;
    // 检查通信是否正常
    bool isHealthy() const;
    // 计算通信质量分数
    double calculateQualityScore() const;
    // 获取连接状态
    bool isConnected() const;
    // 获取连接持续时间
    double getConnectionDuration() const;
};

class CommunitionModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CommunitionModel(QObject *parent = nullptr);
    
    // 表格模型必需的方法
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    
    // 数据管理方法
    void addCommunication(const CommunitionItem &communication);
    void removeCommunication(int row);
    void removeCommunicationById(const QString &id);
    void updateCommunication(int row, const CommunitionItem &communication);
    void updateCommunicationById(const QString &id, const CommunitionItem &communication);
    CommunitionItem getCommunication(int row) const;
    CommunitionItem getCommunicationById(const QString &id) const;
    int findCommunicationById(const QString &id) const;
    void clearAll();
    
    // 数据查询方法
    QList<CommunitionItem> getConnectedCommunications() const;
    QList<CommunitionItem> getCommunicationsByState(CommunitionItem::CommunicationState state) const;
    QList<CommunitionItem> getCommunicationsBySource(const QString &sourceId) const;
    QList<CommunitionItem> getCommunicationsByTarget(const QString &targetId) const;
    QList<CommunitionItem> getHealthyCommunications() const;
    QList<CommunitionItem> getUnhealthyCommunications() const;
    QList<CommunitionItem> getInRangeCommunications() const;
    QList<CommunitionItem> getOutOfRangeCommunications() const;
    
    // 通过源ID和目标ID获取通信关系
    CommunitionItem getCommunicationBySourceAndTarget(const QString &sourceId, const QString &targetId) const;
    
    // 批量操作方法
    void updateQualities(const QMap<QString, QPair<double, double>> &qualities);
    void updateStates(const QMap<QString, CommunitionItem::CommunicationState> &states);
    
    // 批量更新通信关系的经纬度
    void updateCommunicationPositions(const QMap<QString, QPair<double, double>> &positions);

    // 获取所有数据
    QList<CommunitionItem> getAllCommunications() const { return m_communications; }
    
signals:
    void communicationAdded(const CommunitionItem &communication);
    void communicationRemoved(const QString &id);
    void communicationUpdated(const CommunitionItem &communication);

private:
    QList<CommunitionItem> m_communications;
    
    // 列定义
    enum Columns {
        Id = 0,
        Name,
        SourceId,
        SourceName,
        SourceType,
        TargetId,
        TargetName,
        TargetType,
        State,
        SignalStrength,
        CommunicationQuality,
        Bandwidth,
        Latency,
        PacketLoss,
        CurrentDistance,
        MaxRange,
        EstablishTime,
        LastActiveTime,
        Duration,
        ColumnCount
    };
    
    // 辅助方法
    QString formatDateTime(const QDateTime &datetime) const;
    QString formatPercentage(double value) const;
    QString formatDistance(double distance) const;
    QString formatBandwidth(double bandwidth) const;
    QString formatLatency(double latency) const;
    QString formatDuration(double duration) const;
    QColor getTextColorForBackground(const QColor &background) const;
};

#endif // COMMUNITIONMODEL_H
