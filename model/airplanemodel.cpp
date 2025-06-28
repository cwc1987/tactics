#include "airplanemodel.h"
#include <QColor>
#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

// AirPlaneItem 构造函数
AirPlaneItem::AirPlaneItem()
    : latitude(0.0)
    , longitude(0.0)
    , altitude(0.0)
    , heading(0.0)
    , speed(0.0)
    , verticalSpeed(0.0)
    , state(Normal)
    , flightDuration(0.0)
    , totalDistance(0.0)
    , remainingOil(100.0)
    , signalStrength(100.0)
    , cameraOnline(false)
    , gpsOnline(false)
    , radarOnline(false)
    , controlState(Manual)
    , communicationQuality(100.0)
    , isActive(false)
{
    // 初始化时间
    QDateTime now = QDateTime::currentDateTime();
    takeoffTime = now;
    landingTime = now;
    lastCommunication = now;
}

AirPlaneItem::AirPlaneItem(const QString& id, const QString& name)
    : id(id)
    , name(name)
    , latitude(0.0)
    , longitude(0.0)
    , altitude(0.0)
    , heading(0.0)
    , speed(0.0)
    , verticalSpeed(0.0)
    , state(Normal)
    , flightDuration(0.0)
    , totalDistance(0.0)
    , remainingOil(100.0)
    , signalStrength(100.0)
    , cameraOnline(false)
    , gpsOnline(false)
    , radarOnline(false)
    , controlState(Manual)
    , communicationQuality(100.0)
    , isActive(false)
{
    // 初始化时间
    QDateTime now = QDateTime::currentDateTime();
    takeoffTime = now;
    landingTime = now;
    lastCommunication = now;
}

void AirPlaneItem::updatePosition(double lat, double lon, double alt)
{
    latitude = lat;
    longitude = lon;
    altitude = alt;    
    // 更新最后通信时间
    lastCommunication = QDateTime::currentDateTime();
}

void AirPlaneItem::updateState(AirState newState)
{
    state = newState;    
    // 如果状态变为离线，更新相关状态
    if (newState == Offline) {
        cameraOnline = false;
        gpsOnline = false;
        radarOnline = false;
        controlState = Manual;
        signalStrength = 0.0;
        communicationQuality = 0.0;
    }
}

bool AirPlaneItem::isOnline() const
{
    return state != Offline;
}

bool AirPlaneItem::isControllable() const
{
    return isOnline() && (controlState == Manual || controlState == Auto);
}

// 添加缺失的方法实现
QString AirPlaneItem::getStateString() const
{
    switch (state) {
    case Normal: 
        return QObject::tr("Normal");
    case Damage: 
        return QObject::tr("Damage");
    case Emergency: 
        return QObject::tr("Emergency");
    case Maintenance: 
        return QObject::tr("Maintenance");
    case Offline: 
        return QObject::tr("Offline");
    default: 
        return QObject::tr("Unknown");
    }
}

QColor AirPlaneItem::getStateColor() const
{
    switch (state) {
    case Normal:
        return QColor(0, 255, 0);      // 绿色 - 正常
    case Damage:
        return QColor(255, 0, 0);      // 红色 - 损坏
    case Emergency:
        return QColor(255, 0, 255);    // 洋红色 - 紧急
    case Maintenance:
        return QColor(255, 165, 0);    // 橙色 - 维护
    case Offline:
        return QColor(128, 128, 128);  // 灰色 - 离线
    default:
        return QColor(128, 128, 128);  // 默认灰色
    }
}

// AirplaneModel 实现
AirplaneModel::AirplaneModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

// 表格模型必需的方法
int AirplaneModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_airplanes.size();
}

int AirplaneModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant AirplaneModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_airplanes.size())
        return QVariant();

    const AirPlaneItem &airplane = m_airplanes[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Id:
            return airplane.id;
        case Name:
            return airplane.name;
        case Type:
            return airplane.type;
        case Model:
            return airplane.model;
        case Latitude:
            return formatCoordinate(airplane.latitude);
        case Longitude:
            return formatCoordinate(airplane.longitude);
        case Altitude:
            return formatAltitude(airplane.altitude);
        case Heading:
            return QString("%1%2").arg(airplane.heading, 0, 'f', 1).arg(QObject::tr("°"));
        case Speed:
            return formatSpeed(airplane.speed);
        case State:
            return airplane.getStateString();
        case ControlState:
            return getControlStateString(airplane.controlState);
        case RemainingOil:
            return formatPercentage(airplane.remainingOil);
        case SignalStrength:
            return formatPercentage(airplane.signalStrength);
        case CameraOnline:
            return airplane.cameraOnline ? QObject::tr("Online") : QObject::tr("Offline");
        case GpsOnline:
            return airplane.gpsOnline ? QObject::tr("Online") : QObject::tr("Offline");
        case RadarOnline:
            return airplane.radarOnline ? QObject::tr("Online") : QObject::tr("Offline");
        case CommunicationQuality:
            return formatPercentage(airplane.communicationQuality);
        case LastCommunication:
            return formatDateTime(airplane.lastCommunication);
        default:
            return QVariant();
        }
    } else if (role == Qt::BackgroundRole) {
        // 根据状态设置背景色
        if (index.column() == State) {
            return airplane.getStateColor();
        } else if (index.column() == ControlState) {
            return getControlStateColor(airplane.controlState);
        }
    } else if (role == Qt::ForegroundRole) {
        // 根据背景色设置字体颜色
        if (index.column() == State) {
            return getTextColorForBackground(airplane.getStateColor());
        } else if (index.column() == ControlState) {
            return getTextColorForBackground(getControlStateColor(airplane.controlState));
        }
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

QVariant AirplaneModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case Id:
            return QObject::tr("ID");
        case Name:
            return QObject::tr("Name");
        case Type:
            return QObject::tr("Type");
        case Model:
            return QObject::tr("Model");
        case Latitude:
            return QObject::tr("Latitude");
        case Longitude:
            return QObject::tr("Longitude");
        case Altitude:
            return QObject::tr("Altitude");
        case Heading:
            return QObject::tr("Heading");
        case Speed:
            return QObject::tr("Speed");
        case State:
            return QObject::tr("State");
        case ControlState:
            return QObject::tr("Control State");
        case RemainingOil:
            return QObject::tr("Remaining Oil");
        case SignalStrength:
            return QObject::tr("Signal Strength");
        case CameraOnline:
            return QObject::tr("Camera Online");
        case GpsOnline:
            return QObject::tr("GPS Online");
        case RadarOnline:
            return QObject::tr("Radar Online");
        case CommunicationQuality:
            return QObject::tr("Communication Quality");
        case LastCommunication:
            return QObject::tr("Last Communication");
        default:
            return QVariant();
        }
    } else {
        return QString::number(section + 1);
    }
}

Qt::ItemFlags AirplaneModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return flags;
}

bool AirplaneModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //已经设置不可编辑，若需编辑可以根据该代码进行修改
    if (!index.isValid() || index.row() >= m_airplanes.size() || role != Qt::EditRole)
        return false;

    AirPlaneItem &airplane = m_airplanes[index.row()];

    switch (index.column()) {
    case Name:
        airplane.name = value.toString();
        break;
    case Type:
        airplane.type = value.toString();
        break;
    case Model:
        airplane.model = value.toString();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    emit airplaneUpdated(airplane);
    return true;
}

// 数据管理方法
void AirplaneModel::addAirplane(const AirPlaneItem &airplane)
{
    int newRow = m_airplanes.size();
    beginInsertRows(QModelIndex(), newRow, newRow);
    m_airplanes.append(airplane);
    endInsertRows();
    
    emit airplaneAdded(airplane);
}

void AirplaneModel::removeAirplane(int row)
{
    if (row < 0 || row >= m_airplanes.size())
        return;

    QString id = m_airplanes[row].id;
    beginRemoveRows(QModelIndex(), row, row);
    m_airplanes.removeAt(row);
    endRemoveRows();
    
    emit airplaneRemoved(id);
}

void AirplaneModel::removeAirplaneById(const QString &id)
{
    int row = findAirplaneById(id);
    if (row >= 0) {
        removeAirplane(row);
    }
}

void AirplaneModel::updateAirplane(int row, const AirPlaneItem &airplane)
{
    if (row < 0 || row >= m_airplanes.size())
        return;

    m_airplanes[row] = airplane;
    
    QModelIndex topLeft = index(row, 0);
    QModelIndex bottomRight = index(row, ColumnCount);
    emit dataChanged(topLeft, bottomRight);
    
    emit airplaneUpdated(airplane);
}

void AirplaneModel::updateAirplaneById(const QString &id, const AirPlaneItem &airplane)
{
    int row = findAirplaneById(id);
    if (row >= 0) {
        updateAirplane(row, airplane);
    }
}

AirPlaneItem AirplaneModel::getAirplane(int row) const
{
    if (row >= 0 && row < m_airplanes.size()) {
        return m_airplanes[row];
    }
    return AirPlaneItem();
}

AirPlaneItem AirplaneModel::getAirplaneById(const QString &id) const
{
    int row = findAirplaneById(id);
    return getAirplane(row);
}

int AirplaneModel::findAirplaneById(const QString &id) const
{
    for (int i = 0; i < m_airplanes.size(); ++i) {
        if (m_airplanes[i].id == id) {
            return i;
        }
    }
    return -1;
}

void AirplaneModel::clearAll()
{
    if (m_airplanes.isEmpty())
        return;

    beginResetModel();
    m_airplanes.clear();
    endResetModel();
    emit airplaneReset();
}

// 数据查询方法
QList<AirPlaneItem> AirplaneModel::getOnlineAirplanes() const
{
    QList<AirPlaneItem> onlineAirplanes;
    for (const AirPlaneItem &airplane : m_airplanes) {
        if (airplane.isOnline()) {
            onlineAirplanes.append(airplane);
        }
    }
    return onlineAirplanes;
}

QList<AirPlaneItem> AirplaneModel::getAirplanesByState(AirPlaneItem::AirState state) const
{
    QList<AirPlaneItem> filteredAirplanes;
    for (const AirPlaneItem &airplane : m_airplanes) {
        if (airplane.state == state) {
            filteredAirplanes.append(airplane);
        }
    }
    return filteredAirplanes;
}

QList<AirPlaneItem> AirplaneModel::getControllableAirplanes() const
{
    QList<AirPlaneItem> controllableAirplanes;
    for (const AirPlaneItem &airplane : m_airplanes) {
        if (airplane.isControllable()) {
            controllableAirplanes.append(airplane);
        }
    }
    return controllableAirplanes;
}

// 批量操作方法
void AirplaneModel::updatePositions(const QMap<QString, QPair<double, double>> &positions)
{
    for (auto it = positions.begin(); it != positions.end(); ++it) {
        int row = findAirplaneById(it.key());
        if (row >= 0) {
            m_airplanes[row].updatePosition(it.value().first, it.value().second, m_airplanes[row].altitude);
        }
    }
    
    if (!positions.isEmpty()) {
        QModelIndex topLeft = index(0, Latitude);
        QModelIndex bottomRight = index(rowCount() - 1, Longitude);
        emit dataChanged(topLeft, bottomRight);
    }
}

void AirplaneModel::updateStates(const QMap<QString, AirPlaneItem::AirState> &states)
{
    for (auto it = states.begin(); it != states.end(); ++it) {
        int row = findAirplaneById(it.key());
        if (row >= 0) {
            m_airplanes[row].updateState(it.value());
        }
    }
    
    if (!states.isEmpty()) {
        QModelIndex topLeft = index(0, State);
        QModelIndex bottomRight = index(rowCount() - 1, State);
        emit dataChanged(topLeft, bottomRight);
    }
}

// 辅助方法
QString AirplaneModel::formatCoordinate(double coord) const
{
    return QString("%1%2").arg(coord, 0, 'f', 6).arg(QObject::tr("°"));
}

QString AirplaneModel::formatDateTime(const QDateTime &datetime) const
{
    if (!datetime.isValid())
        return QObject::tr("Unknown");
    return datetime.toString("yyyy-MM-dd hh:mm:ss");
}

QString AirplaneModel::formatPercentage(double value) const
{
    return QString("%1%2").arg(value, 0, 'f', 1).arg(QObject::tr("%"));
}

QString AirplaneModel::formatSpeed(double speed) const
{
    return QString("%1 %2").arg(speed, 0, 'f', 1).arg(QObject::tr("m/s"));
}

QString AirplaneModel::formatAltitude(double altitude) const
{
    return QString("%1 %2").arg(altitude, 0, 'f', 0).arg(QObject::tr("m"));
}

QString AirplaneModel::getControlStateString(AirPlaneItem::AirControlState state) const
{
    switch (state) {
    case AirPlaneItem::Manual:
        return QObject::tr("Manual");
    case AirPlaneItem::Auto:
        return QObject::tr("Auto");
    case AirPlaneItem::Emergency:
        return QObject::tr("Emergency");
    case AirPlaneItem::Unknown:
        return QObject::tr("Unknown");
    default:
        return QObject::tr("Unknown");
    }
}

QColor AirplaneModel::getControlStateColor(AirPlaneItem::AirControlState state) const
{
    switch (state) {
    case AirPlaneItem::Manual:
        return QColor(255, 255, 0);    // 黄色 - 手动
    case AirPlaneItem::Auto:
        return QColor(0, 255, 0);      // 绿色 - 自动
    case AirPlaneItem::Unknown:
        return QColor(128, 128, 128);  // 灰色 - 未知
    default:
        return QColor(128, 128, 128);  // 默认灰色
    }
}

QColor AirplaneModel::getTextColorForBackground(const QColor &background) const
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
