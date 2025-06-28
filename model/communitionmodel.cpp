#include "communitionmodel.h"
#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QtMath>

// CommunitionItem 构造函数
CommunitionItem::CommunitionItem()
    : state(Unknown)
    , signalStrength(0.0)
    , communicationQuality(0.0)
    , bandwidth(0.0)
    , latency(0.0)
    , packetLoss(0.0)
    , duration(0.0)
    , sourceLatitude(0.0)
    , sourceLongitude(0.0)
    , sourceAltitude(0.0)
    , targetLatitude(0.0)
    , targetLongitude(0.0)
    , targetAltitude(0.0)
    , maxRange(0.0)
    , currentDistance(0.0)
{
    // 初始化时间
    QDateTime now = QDateTime::currentDateTime();
    establishTime = now;
    lastActiveTime = now;
}

CommunitionItem::CommunitionItem(const QString& id, const QString& sourceId, const QString& targetId)
    : id(id)
    , sourceId(sourceId)
    , targetId(targetId)
    , state(Unknown)
    , signalStrength(0.0)
    , communicationQuality(0.0)
    , bandwidth(0.0)
    , latency(0.0)
    , packetLoss(0.0)
    , duration(0.0)
    , sourceLatitude(0.0)
    , sourceLongitude(0.0)
    , sourceAltitude(0.0)
    , targetLatitude(0.0)
    , targetLongitude(0.0)
    , targetAltitude(0.0)
    , maxRange(0.0)
    , currentDistance(0.0)
{
    // 初始化时间
    QDateTime now = QDateTime::currentDateTime();
    establishTime = now;
    lastActiveTime = now;
    
    // 生成通信关系名称
    name = QString("%1-%2").arg(sourceId).arg(targetId);
}

// CommunitionItem 方法实现
QString CommunitionItem::getStateString() const
{
    switch (state) {
    case Connected:
        return QObject::tr("Connected");
    case Connecting:
        return QObject::tr("Connecting");
    case Disconnected:
        return QObject::tr("Disconnected");
    case Failed:
        return QObject::tr("Failed");
    case Unknown:
        return QObject::tr("Unknown");
    default:
        return QObject::tr("Unknown");
    }
}

QColor CommunitionItem::getStateColor() const
{
    switch (state) {
    case Connected:
        return QColor(0, 255, 0);      // 绿色 - 已连接
    case Connecting:
        return QColor(255, 255, 0);    // 黄色 - 连接中
    case Disconnected:
        return QColor(128, 128, 128);  // 灰色 - 已断开
    case Failed:
        return QColor(255, 0, 0);      // 红色 - 连接失败
    case Unknown:
        return QColor(128, 128, 128);  // 灰色 - 未知
    default:
        return QColor(128, 128, 128);  // 默认灰色
    }
}

QColor CommunitionItem::getQualityColor() const
{
    if (communicationQuality >= 80.0) {
        return QColor(0, 255, 0);      // 绿色 - 优秀
    } else if (communicationQuality >= 60.0) {
        return QColor(255, 255, 0);    // 黄色 - 良好
    } else if (communicationQuality >= 40.0) {
        return QColor(255, 165, 0);    // 橙色 - 一般
    } else {
        return QColor(255, 0, 0);      // 红色 - 差
    }
}

void CommunitionItem::updateState(CommunicationState newState)
{
    state = newState;
    lastActiveTime = QDateTime::currentDateTime();
    
    if (newState == Connected) {
        // 如果刚连接，更新建立时间
        if (establishTime.isNull()) {
            establishTime = QDateTime::currentDateTime();
        }
    }
}

void CommunitionItem::updateQuality(double quality, double signal, double lat, double packet)
{
    communicationQuality = quality;
    signalStrength = signal;
    latency = lat;
    packetLoss = packet;
    lastActiveTime = QDateTime::currentDateTime();
}

void CommunitionItem::updateSourcePosition(double lat, double lon, double alt)
{
    sourceLatitude = lat;
    sourceLongitude = lon;
    sourceAltitude = alt;
    calculateDistance();
}

void CommunitionItem::updateTargetPosition(double lat, double lon, double alt)
{
    targetLatitude = lat;
    targetLongitude = lon;
    targetAltitude = alt;
    calculateDistance();
}

void CommunitionItem::calculateDistance()
{
    // 使用Haversine公式计算两点间距离
    const double R = 6371000.0; // 地球半径(米)
    
    double lat1 = qDegreesToRadians(sourceLatitude);
    double lon1 = qDegreesToRadians(sourceLongitude);
    double lat2 = qDegreesToRadians(targetLatitude);
    double lon2 = qDegreesToRadians(targetLongitude);
    
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    
    double a = qSin(dlat/2) * qSin(dlat/2) + qCos(lat1) * qCos(lat2) * qSin(dlon/2) * qSin(dlon/2);
    double c = 2 * qAtan2(qSqrt(a), qSqrt(1-a));
    
    double horizontalDistance = R * c;
    
    // 计算高度差
    double altitudeDiff = qAbs(targetAltitude - sourceAltitude);
    
    // 计算三维距离
    currentDistance = qSqrt(horizontalDistance * horizontalDistance + altitudeDiff * altitudeDiff);
}

bool CommunitionItem::isInRange() const
{
    return currentDistance <= maxRange;
}

bool CommunitionItem::isHealthy() const
{
    return state == Connected && 
           communicationQuality >= 60.0 && 
           signalStrength >= 50.0 && 
           latency <= 100.0 && 
           packetLoss <= 5.0 &&
           isInRange();
}

double CommunitionItem::calculateQualityScore() const
{
    double score = 0.0;
    
    // 基础分数
    score += communicationQuality * 0.4;  // 通信质量权重40%
    score += signalStrength * 0.3;        // 信号强度权重30%
    
    // 延迟分数（延迟越低分数越高）
    if (latency <= 10.0) {
        score += 20.0;
    } else if (latency <= 50.0) {
        score += 15.0;
    } else if (latency <= 100.0) {
        score += 10.0;
    } else if (latency <= 200.0) {
        score += 5.0;
    }
    
    // 丢包率分数（丢包率越低分数越高）
    if (packetLoss <= 1.0) {
        score += 10.0;
    } else if (packetLoss <= 3.0) {
        score += 7.0;
    } else if (packetLoss <= 5.0) {
        score += 5.0;
    } else if (packetLoss <= 10.0) {
        score += 2.0;
    }
    
    return score;
}

bool CommunitionItem::isConnected() const
{
    return state == Connected;
}

double CommunitionItem::getConnectionDuration() const
{
    if (!isConnected() || establishTime.isNull()) {
        return 0.0;
    }
    
    QDateTime endTime = lastActiveTime.isValid() ? lastActiveTime : QDateTime::currentDateTime();
    return establishTime.secsTo(endTime) / 60.0; // 返回分钟数
}

// CommunitionModel 实现
CommunitionModel::CommunitionModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

// 表格模型必需的方法
int CommunitionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_communications.size();
}

int CommunitionModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant CommunitionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_communications.size())
        return QVariant();

    const CommunitionItem &communication = m_communications[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Id:
            return communication.id;
        case Name:
            return communication.name;
        case SourceId:
            return communication.sourceId;
        case SourceName:
            return communication.sourceName;
        case SourceType:
            return communication.sourceType;
        case TargetId:
            return communication.targetId;
        case TargetName:
            return communication.targetName;
        case TargetType:
            return communication.targetType;
        case State:
            return communication.getStateString();
        case SignalStrength:
            return formatPercentage(communication.signalStrength);
        case CommunicationQuality:
            return formatPercentage(communication.communicationQuality);
        case Bandwidth:
            return formatBandwidth(communication.bandwidth);
        case Latency:
            return formatLatency(communication.latency);
        case PacketLoss:
            return formatPercentage(communication.packetLoss);
        case CurrentDistance:
            return formatDistance(communication.currentDistance);
        case MaxRange:
            return formatDistance(communication.maxRange);
        case EstablishTime:
            return formatDateTime(communication.establishTime);
        case LastActiveTime:
            return formatDateTime(communication.lastActiveTime);
        case Duration:
            return formatDuration(communication.getConnectionDuration());
        default:
            return QVariant();
        }
    } else if (role == Qt::BackgroundRole) {
        // 根据状态设置背景色
        if (index.column() == State) {
            return communication.getStateColor();
        } else if (index.column() == CommunicationQuality) {
            return communication.getQualityColor();
        }
    } else if (role == Qt::ForegroundRole) {
        // 根据背景色设置字体颜色
        if (index.column() == State) {
            return getTextColorForBackground(communication.getStateColor());
        } else if (index.column() == CommunicationQuality) {
            return getTextColorForBackground(communication.getQualityColor());
        }
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

QVariant CommunitionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case Id:
            return QObject::tr("ID");
        case Name:
            return QObject::tr("Name");
        case SourceId:
            return QObject::tr("Source ID");
        case SourceName:
            return QObject::tr("Source Name");
        case SourceType:
            return QObject::tr("Source Type");
        case TargetId:
            return QObject::tr("Target ID");
        case TargetName:
            return QObject::tr("Target Name");
        case TargetType:
            return QObject::tr("Target Type");
        case State:
            return QObject::tr("State");
        case SignalStrength:
            return QObject::tr("Signal Strength");
        case CommunicationQuality:
            return QObject::tr("Quality");
        case Bandwidth:
            return QObject::tr("Bandwidth");
        case Latency:
            return QObject::tr("Latency");
        case PacketLoss:
            return QObject::tr("Packet Loss");
        case CurrentDistance:
            return QObject::tr("Current Distance");
        case MaxRange:
            return QObject::tr("Max Range");
        case EstablishTime:
            return QObject::tr("Establish Time");
        case LastActiveTime:
            return QObject::tr("Last Active");
        case Duration:
            return QObject::tr("Duration");
        default:
            return QVariant();
        }
    } else {
        return QString::number(section + 1);
    }
}

Qt::ItemFlags CommunitionModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool CommunitionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // 数据不可编辑，保留可用
    if (!index.isValid() || index.row() >= m_communications.size() || role != Qt::EditRole)
        return false;

    return false;
}

// 数据管理方法
void CommunitionModel::addCommunication(const CommunitionItem &communication)
{
    int newRow = m_communications.size();
    beginInsertRows(QModelIndex(), newRow, newRow);
    m_communications.append(communication);
    endInsertRows();
    
    emit communicationAdded(communication);
}

void CommunitionModel::removeCommunication(int row)
{
    if (row < 0 || row >= m_communications.size())
        return;

    QString id = m_communications[row].id;
    beginRemoveRows(QModelIndex(), row, row);
    m_communications.removeAt(row);
    endRemoveRows();
    
    emit communicationRemoved(id);
}

void CommunitionModel::removeCommunicationById(const QString &id)
{
    int row = findCommunicationById(id);
    if (row >= 0) {
        removeCommunication(row);
    }
}

void CommunitionModel::updateCommunication(int row, const CommunitionItem &communication)
{
    if (row < 0 || row >= m_communications.size())
        return;

    m_communications[row] = communication;
    
    QModelIndex topLeft = index(row, 0);
    QModelIndex bottomRight = index(row, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
    
    emit communicationUpdated(communication);
}

void CommunitionModel::updateCommunicationById(const QString &id, const CommunitionItem &communication)
{
    int row = findCommunicationById(id);
    if (row >= 0) {
        updateCommunication(row, communication);
    }
}

CommunitionItem CommunitionModel::getCommunication(int row) const
{
    if (row >= 0 && row < m_communications.size()) {
        return m_communications[row];
    }
    return CommunitionItem();
}

CommunitionItem CommunitionModel::getCommunicationById(const QString &id) const
{
    int row = findCommunicationById(id);
    return getCommunication(row);
}

int CommunitionModel::findCommunicationById(const QString &id) const
{
    for (int i = 0; i < m_communications.size(); ++i) {
        if (m_communications[i].id == id) {
            return i;
        }
    }
    return -1;
}

void CommunitionModel::clearAll()
{
    if (m_communications.isEmpty())
        return;

    beginResetModel();
    m_communications.clear();
    endResetModel();
}

// 数据查询方法
QList<CommunitionItem> CommunitionModel::getConnectedCommunications() const
{
    QList<CommunitionItem> connectedCommunications;
    for (const CommunitionItem &communication : m_communications) {
        if (communication.isConnected()) {
            connectedCommunications.append(communication);
        }
    }
    return connectedCommunications;
}

QList<CommunitionItem> CommunitionModel::getCommunicationsByState(CommunitionItem::CommunicationState state) const
{
    QList<CommunitionItem> filteredCommunications;
    for (const CommunitionItem &communication : m_communications) {
        if (communication.state == state) {
            filteredCommunications.append(communication);
        }
    }
    return filteredCommunications;
}

QList<CommunitionItem> CommunitionModel::getCommunicationsBySource(const QString &sourceId) const
{
    QList<CommunitionItem> filteredCommunications;
    for (const CommunitionItem &communication : m_communications) {
        if (communication.sourceId == sourceId) {
            filteredCommunications.append(communication);
        }
    }
    return filteredCommunications;
}

QList<CommunitionItem> CommunitionModel::getCommunicationsByTarget(const QString &targetId) const
{
    QList<CommunitionItem> filteredCommunications;
    for (const CommunitionItem &communication : m_communications) {
        if (communication.targetId == targetId) {
            filteredCommunications.append(communication);
        }
    }
    return filteredCommunications;
}

QList<CommunitionItem> CommunitionModel::getHealthyCommunications() const
{
    QList<CommunitionItem> healthyCommunications;
    for (const CommunitionItem &communication : m_communications) {
        if (communication.isHealthy()) {
            healthyCommunications.append(communication);
        }
    }
    return healthyCommunications;
}

QList<CommunitionItem> CommunitionModel::getUnhealthyCommunications() const
{
    QList<CommunitionItem> unhealthyCommunications;
    for (const CommunitionItem &communication : m_communications) {
        if (!communication.isHealthy()) {
            unhealthyCommunications.append(communication);
        }
    }
    return unhealthyCommunications;
}

QList<CommunitionItem> CommunitionModel::getInRangeCommunications() const
{
    QList<CommunitionItem> inRangeCommunications;
    for (const CommunitionItem &communication : m_communications) {
        if (communication.isInRange()) {
            inRangeCommunications.append(communication);
        }
    }
    return inRangeCommunications;
}

QList<CommunitionItem> CommunitionModel::getOutOfRangeCommunications() const
{
    QList<CommunitionItem> outOfRangeCommunications;
    for (const CommunitionItem &communication : m_communications) {
        if (!communication.isInRange()) {
            outOfRangeCommunications.append(communication);
        }
    }
    return outOfRangeCommunications;
}

// 通过源ID和目标ID获取通信关系
CommunitionItem CommunitionModel::getCommunicationBySourceAndTarget(const QString &sourceId, const QString &targetId) const
{
    for (const CommunitionItem &communication : m_communications) {
        if (communication.sourceId == sourceId && communication.targetId == targetId) {
            return communication;
        }
    }
    return CommunitionItem(); // 返回空的通信项
}

// 批量操作方法
void CommunitionModel::updateQualities(const QMap<QString, QPair<double, double>> &qualities)
{
    for (auto it = qualities.begin(); it != qualities.end(); ++it) {
        int row = findCommunicationById(it.key());
        if (row >= 0) {
            m_communications[row].updateQuality(it.value().first, it.value().second, 
                                              m_communications[row].latency, 
                                              m_communications[row].packetLoss);
        }
    }
    
    if (!qualities.isEmpty()) {
        QModelIndex topLeft = index(0, CommunicationQuality);
        QModelIndex bottomRight = index(rowCount() - 1, SignalStrength);
        emit dataChanged(topLeft, bottomRight);
    }
}

void CommunitionModel::updateStates(const QMap<QString, CommunitionItem::CommunicationState> &states)
{
    for (auto it = states.begin(); it != states.end(); ++it) {
        int row = findCommunicationById(it.key());
        if (row >= 0) {
            m_communications[row].updateState(it.value());
        }
    }
    
    if (!states.isEmpty()) {
        QModelIndex topLeft = index(0, State);
        QModelIndex bottomRight = index(rowCount() - 1, State);
        emit dataChanged(topLeft, bottomRight);
    }
}

// 批量更新通信关系的经纬度
void CommunitionModel::updateCommunicationPositions(const QMap<QString, QPair<double, double>> &positions)
{
    for (auto it = positions.begin(); it != positions.end(); ++it) {
        QString deviceId = it.key();
        double latitude = it.value().first;
        double longitude = it.value().second;
        
        // 遍历所有通信关系，检查源设备或目标设备是否匹配
        for (int i = 0; i < m_communications.size(); ++i) {
            CommunitionItem &communication = m_communications[i];
            
            // 检查源设备是否匹配
            if (communication.sourceId == deviceId) {
                communication.updateSourcePosition(latitude, longitude, communication.sourceAltitude);
            }
            
            // 检查目标设备是否匹配
            if (communication.targetId == deviceId) {
                communication.updateTargetPosition(latitude, longitude, communication.targetAltitude);
            }
        }
    }
    
    if (!positions.isEmpty()) {
        // 通知视图数据已变化，更新距离相关的列
        QModelIndex topLeft = index(0, CurrentDistance);
        QModelIndex bottomRight = index(rowCount() - 1, CurrentDistance);
        emit dataChanged(topLeft, bottomRight);
    }
}

// 辅助方法
QString CommunitionModel::formatDateTime(const QDateTime &datetime) const
{
    if (!datetime.isValid())
        return QObject::tr("Unknown");
    return datetime.toString("yyyy-MM-dd hh:mm:ss");
}

QString CommunitionModel::formatPercentage(double value) const
{
    return QString("%1%2").arg(value, 0, 'f', 1).arg(QObject::tr("%"));
}

QString CommunitionModel::formatDistance(double distance) const
{
    if (distance >= 1000.0) {
        return QString("%1 %2").arg(distance / 1000.0, 0, 'f', 2).arg(QObject::tr("km"));
    } else {
        return QString("%1 %2").arg(distance, 0, 'f', 0).arg(QObject::tr("m"));
    }
}

QString CommunitionModel::formatBandwidth(double bandwidth) const
{
    if (bandwidth >= 1000.0) {
        return QString("%1 %2").arg(bandwidth / 1000.0, 0, 'f', 2).arg(QObject::tr("Gbps"));
    } else {
        return QString("%1 %2").arg(bandwidth, 0, 'f', 2).arg(QObject::tr("Mbps"));
    }
}

QString CommunitionModel::formatLatency(double latency) const
{
    return QString("%1 %2").arg(latency, 0, 'f', 1).arg(QObject::tr("ms"));
}

QString CommunitionModel::formatDuration(double duration) const
{
    if (duration >= 60.0) {
        int hours = static_cast<int>(duration / 60.0);
        int minutes = static_cast<int>(duration) % 60;
        return QString("%1:%2").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0'));
    } else {
        return QString("%1 %2").arg(duration, 0, 'f', 0).arg(QObject::tr("min"));
    }
}

QColor CommunitionModel::getTextColorForBackground(const QColor &background) const
{
    // 计算背景色的亮度 (YIQ公式)
    double brightness = (background.red() * 299 + background.green() * 587 + background.blue() * 114) / 1000.0;
    
    // 如果背景色较亮，使用黑色字体；如果背景色较暗，使用白色字体
    if (brightness > 128) {
        return QColor(0, 0, 0);  // 黑色字体
    } else {
        return QColor(255, 255, 255);  // 白色字体
    }
}
