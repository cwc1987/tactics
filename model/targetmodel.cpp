#include "targetmodel.h"
#include <QColor>
#include <QDateTime>
#include <QDebug>

// TargetItem 构造函数
TargetItem::TargetItem()
    : type(UnknownType)
    , state(Normal)
    , latitude(0.0)
    , longitude(0.0)
    , altitude(0.0)
    , heading(0.0)
    , speed(0.0)
    , verticalSpeed(0.0)
    , threatLevel(Low)
    , confidence(0.0)
    , isDetected(false)
    , isTracked(false)
    , detectionRange(0.0)
    , weaponRange(0.0)
    , missileCount(0)
    , communicationRange(0.0)
{
    // 初始化时间
    QDateTime now = QDateTime::currentDateTime();
    firstDetectionTime = now;
    lastDetectionTime = now;
}

TargetItem::TargetItem(const QString& id, const QString& name, TargetType type)
    : id(id)
    , name(name)
    , type(type)
    , state(Normal)
    , latitude(0.0)
    , longitude(0.0)
    , altitude(0.0)
    , heading(0.0)
    , speed(0.0)
    , verticalSpeed(0.0)
    , threatLevel(Low)
    , confidence(0.0)
    , isDetected(false)
    , isTracked(false)
    , detectionRange(0.0)
    , weaponRange(0.0)
    , missileCount(0)
    , communicationRange(0.0)
{
    // 初始化时间
    QDateTime now = QDateTime::currentDateTime();
    firstDetectionTime = now;
    lastDetectionTime = now;
}

// TargetItem 方法实现
QString TargetItem::getStateString() const
{
    switch (state) {
    case Normal:
        return QObject::tr("Normal");
    case Destoried:
        return QObject::tr("Destroyed");
    case Damage:
        return QObject::tr("Damage");
    case UnkonwnState:
        return QObject::tr("QObject");
    default:
        return QObject::tr("Unknown");
    }
}

QString TargetItem::getThreatLevelString() const
{
    switch (threatLevel) {
    case Low:
        return QObject::tr("Low");
    case Medium:
        return QObject::tr("Medium");
    case High:
        return QObject::tr("High");
    case Critical:
        return QObject::tr("Critical");
    default:
        return QObject::tr("Unknown");
    }
}

QString TargetItem::getTypeString() const
{
    switch (type) {
    case EnemyTank:
        return QObject::tr("Enemy Tank");
    case EnemyArmoredVehicle:
        return QObject::tr("Enemy Armored Vehicle");
    case FriendlyRocket:
        return QObject::tr("Friendly Rocket");
    case FriendlyGroundStation:
        return QObject::tr("Friendly Ground Station");
    case FriendlyProtectionVehicle:
        return QObject::tr("Friendly Protection Vehicle");
    case UnknownType:
        return QObject::tr("Unknown Type");
    default:
        return QObject::tr("Unknown Type");
    }
}

QColor TargetItem::getStateColor() const
{
    switch (state) {
    case Normal:
        return QColor(0, 255, 0);      // 绿色 - 正常
    case Destoried:
        return QColor(128, 128, 128);  // 灰色 - 已摧毁
    default:
        return QColor(128, 128, 128);  // 默认灰色
    }
}

QColor TargetItem::getThreatLevelColor() const
{
    switch (threatLevel) {
    case Low:
        return QColor(0, 255, 0);      // 绿色 - 低威胁
    case Medium:
        return QColor(255, 255, 0);    // 黄色 - 中等威胁
    case High:
        return QColor(255, 165, 0);    // 橙色 - 高威胁
    case Critical:
        return QColor(255, 0, 0);      // 红色 - 严重威胁
    default:
        return QColor(128, 128, 128);  // 默认灰色
    }
}

QColor TargetItem::getTypeColor() const
{
    switch (type) {
    case EnemyTank:
    case EnemyArmoredVehicle:
        return QColor(255, 0, 0);      // 红色 - 敌方
    case FriendlyRocket:
    case FriendlyGroundStation:
    case FriendlyProtectionVehicle:
        return QColor(0, 255, 0);      // 绿色 - 友方
    case UnknownType:
        return QColor(128, 128, 128);  // 灰色 - 未知
    default:
        return QColor(128, 128, 128);  // 默认灰色
    }
}

void TargetItem::updatePosition(double lat, double lon, double alt)
{
    latitude = lat;
    longitude = lon;
    altitude = alt;
    lastDetectionTime = QDateTime::currentDateTime();
}

void TargetItem::updateState(TargetState newState)
{
    state = newState;
    lastDetectionTime = QDateTime::currentDateTime();
}

void TargetItem::updateThreatLevel(ThreatLevel newLevel)
{
    threatLevel = newLevel;
    lastDetectionTime = QDateTime::currentDateTime();
}

bool TargetItem::isTrackable() const
{
    return isDetected && state == Normal;
}

double TargetItem::calculateThreatScore() const
{
    double score = 0.0;
    
    // 基础威胁分数
    switch (threatLevel) {
    case Low: score += 10; break;
    case Medium: score += 30; break;
    case High: score += 60; break;
    case Critical: score += 100; break;
    default: break;
    }
    
    // 武器系统分数
    if (missileCount > 0) {
        score += missileCount * 5;
    }
    
    // 探测状态分数
    if (isDetected) {
        score += 10;
    }
    
    return score;
}

bool TargetItem::isInWeaponRange(double distance) const
{
    return distance <= weaponRange;
}

bool TargetItem::isInDetectionRange(double distance) const
{
    return distance <= detectionRange;
}

bool TargetItem::isEnemyType() const
{
    return type == EnemyTank || type == EnemyArmoredVehicle;
}

bool TargetItem::isFriendlyType() const
{
    return type == FriendlyRocket || type == FriendlyGroundStation || type == FriendlyProtectionVehicle;
}

// TargetModel 实现
TargetModel::TargetModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

// 表格模型必需的方法
int TargetModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_targets.size();
}

int TargetModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant TargetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_targets.size())
        return QVariant();

    const TargetItem &target = m_targets[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Id:
            return target.id;
        case Name:
            return target.name;
        case Type:
            return target.getTypeString();
        case Model:
            return target.model;
        case Latitude:
            return formatCoordinate(target.latitude);
        case Longitude:
            return formatCoordinate(target.longitude);
        case Altitude:
            return formatAltitude(target.altitude);
        case Heading:
            return QString("%1%2").arg(target.heading, 0, 'f', 1).arg(QObject::tr("°"));
        case Speed:
            return formatSpeed(target.speed);
        case State:
            return target.getStateString();
        case ThreatLevel:
            return target.getThreatLevelString();
        case Confidence:
            return formatPercentage(target.confidence);
        case IsDetected:
            return target.isDetected ? QObject::tr("Yes") : QObject::tr("No");
        case IsTracked:
            return target.isTracked ? QObject::tr("Yes") : QObject::tr("No");
        case DetectionRange:
            return formatRange(target.detectionRange);
        case WeaponRange:
            return formatRange(target.weaponRange);
        case MissileCount:
            return QString::number(target.missileCount);
        case LastDetection:
            return formatDateTime(target.lastDetectionTime);
        default:
            return QVariant();
        }
    } else if (role == Qt::BackgroundRole) {
        // 根据状态设置背景色
        if (index.column() == State) {
            return target.getStateColor();
        } else if (index.column() == ThreatLevel) {
            return target.getThreatLevelColor();
        } else if (index.column() == Type) {
            return target.getTypeColor();
        }
    } else if (role == Qt::ForegroundRole) {
        // 根据背景色设置字体颜色
        if (index.column() == State) {
            return getTextColorForBackground(target.getStateColor());
        } else if (index.column() == ThreatLevel) {
            return getTextColorForBackground(target.getThreatLevelColor());
        } else if (index.column() == Type) {
            return getTextColorForBackground(target.getTypeColor());
        }
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

QVariant TargetModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        case ThreatLevel:
            return QObject::tr("Threat Level");
        case Confidence:
            return QObject::tr("Confidence");
        case IsDetected:
            return QObject::tr("Detected");
        case IsTracked:
            return QObject::tr("Tracked");
        case DetectionRange:
            return QObject::tr("Detection Range");
        case WeaponRange:
            return QObject::tr("Weapon Range");
        case MissileCount:
            return QObject::tr("Missile Count");
        case PlatformType:
            return QObject::tr("Platform Type");
        case LastDetection:
            return QObject::tr("Last Detection");
        default:
            return QVariant();
        }
    } else {
        return QString::number(section + 1);
    }
}

Qt::ItemFlags TargetModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return flags;
}

bool TargetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //数据不可编辑，保留可用
    if (!index.isValid() || index.row() >= m_targets.size() || role != Qt::EditRole)
        return false;

    TargetItem &target = m_targets[index.row()];

    switch (index.column()) {
    case Name:
        target.name = value.toString();
        break;
    case Model:
        target.model = value.toString();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    emit targetUpdated(target);
    return true;
}

// 数据管理方法
void TargetModel::addTarget(const TargetItem &target)
{
    int newRow = m_targets.size();
    beginInsertRows(QModelIndex(), newRow, newRow);
    m_targets.append(target);
    endInsertRows();
    
    emit targetAdded(target);
}

void TargetModel::removeTarget(int row)
{
    if (row < 0 || row >= m_targets.size())
        return;

    QString id = m_targets[row].id;
    beginRemoveRows(QModelIndex(), row, row);
    m_targets.removeAt(row);
    endRemoveRows();
    
    emit targetRemoved(id);
}

void TargetModel::removeTargetById(const QString &id)
{
    int row = findTargetById(id);
    if (row >= 0) {
        removeTarget(row);
    }
}

void TargetModel::updateTarget(int row, const TargetItem &target)
{
    if (row < 0 || row >= m_targets.size())
        return;

    m_targets[row] = target;
    
    QModelIndex topLeft = index(row, 0);
    QModelIndex bottomRight = index(row, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
    emit targetUpdated(target);
}

void TargetModel::updateTargetById(const QString &id, const TargetItem &target)
{
    int row = findTargetById(id);
    if (row >= 0) {
        updateTarget(row, target);
    }
}

TargetItem TargetModel::getTarget(int row) const
{
    if (row >= 0 && row < m_targets.size()) {
        return m_targets[row];
    }
    return TargetItem();
}

TargetItem TargetModel::getTargetById(const QString &id) const
{
    int row = findTargetById(id);
    return getTarget(row);
}

int TargetModel::findTargetById(const QString &id) const
{
    for (int i = 0; i < m_targets.size(); ++i) {
        if (m_targets[i].id == id) {
            return i;
        }
    }
    return -1;
}

void TargetModel::clearAll()
{
    if (m_targets.isEmpty())
        return;

    beginResetModel();
    m_targets.clear();
    endResetModel();
}

// 数据查询方法
QList<TargetItem> TargetModel::getDetectedTargets() const
{
    QList<TargetItem> detectedTargets;
    for (const TargetItem &target : m_targets) {
        if (target.isDetected) {
            detectedTargets.append(target);
        }
    }
    return detectedTargets;
}

QList<TargetItem> TargetModel::getTrackedTargets() const
{
    QList<TargetItem> trackedTargets;
    for (const TargetItem &target : m_targets) {
        if (target.isTracked) {
            trackedTargets.append(target);
        }
    }
    return trackedTargets;
}

QList<TargetItem> TargetModel::getTargetsByState(TargetItem::TargetState state) const
{
    QList<TargetItem> filteredTargets;
    for (const TargetItem &target : m_targets) {
        if (target.state == state) {
            filteredTargets.append(target);
        }
    }
    return filteredTargets;
}

QList<TargetItem> TargetModel::getTargetsByType(TargetItem::TargetType type) const
{
    QList<TargetItem> filteredTargets;
    for (const TargetItem &target : m_targets) {
        if (target.type == type) {
            filteredTargets.append(target);
        }
    }
    return filteredTargets;
}

QList<TargetItem> TargetModel::getTargetsByThreatLevel(TargetItem::ThreatLevel level) const
{
    QList<TargetItem> filteredTargets;
    for (const TargetItem &target : m_targets) {
        if (target.threatLevel == level) {
            filteredTargets.append(target);
        }
    }
    return filteredTargets;
}

QList<TargetItem> TargetModel::getEnemyTypeTargets() const
{
    QList<TargetItem> enemyTypeTargets;
    for (const TargetItem &target : m_targets) {
        if (target.isEnemyType()) {
            enemyTypeTargets.append(target);
        }
    }
    return enemyTypeTargets;
}

QList<TargetItem> TargetModel::getFriendlyTypeTargets() const
{
    QList<TargetItem> friendlyTypeTargets;
    for (const TargetItem &target : m_targets) {
        if (target.isFriendlyType()) {
            friendlyTypeTargets.append(target);
        }
    }
    return friendlyTypeTargets;
}

// 批量操作方法
void TargetModel::updatePositions(const QMap<QString, QPair<double, double>> &positions)
{
    for (auto it = positions.begin(); it != positions.end(); ++it) {
        int row = findTargetById(it.key());
        if (row >= 0) {
            m_targets[row].updatePosition(it.value().first, it.value().second, m_targets[row].altitude);
        }
    }
    
    if (!positions.isEmpty()) {
        QModelIndex topLeft = index(0, Latitude);
        QModelIndex bottomRight = index(rowCount() - 1, Longitude);
        emit dataChanged(topLeft, bottomRight);
    }
}

void TargetModel::updateStates(const QMap<QString, TargetItem::TargetState> &states)
{
    for (auto it = states.begin(); it != states.end(); ++it) {
        int row = findTargetById(it.key());
        if (row >= 0) {
            m_targets[row].updateState(it.value());
        }
    }
    
    if (!states.isEmpty()) {
        QModelIndex topLeft = index(0, State);
        QModelIndex bottomRight = index(rowCount() - 1, State);
        emit dataChanged(topLeft, bottomRight);
    }
}

void TargetModel::updateThreatLevels(const QMap<QString, TargetItem::ThreatLevel> &levels)
{
    for (auto it = levels.begin(); it != levels.end(); ++it) {
        int row = findTargetById(it.key());
        if (row >= 0) {
            m_targets[row].updateThreatLevel(it.value());
        }
    }
    
    if (!levels.isEmpty()) {
        QModelIndex topLeft = index(0, ThreatLevel);
        QModelIndex bottomRight = index(rowCount() - 1, ThreatLevel);
        emit dataChanged(topLeft, bottomRight);
    }
}

// 辅助方法
QString TargetModel::formatCoordinate(double coord) const
{
    return QString("%1%2").arg(coord, 0, 'f', 6).arg(QObject::tr("°"));
}

QString TargetModel::formatDateTime(const QDateTime &datetime) const
{
    if (!datetime.isValid())
        return QObject::tr("Unknown");
    return datetime.toString("yyyy-MM-dd hh:mm:ss");
}

QString TargetModel::formatPercentage(double value) const
{
    return QString("%1%2").arg(value, 0, 'f', 1).arg(QObject::tr("%"));
}

QString TargetModel::formatSpeed(double speed) const
{
    return QString("%1 %2").arg(speed, 0, 'f', 1).arg(QObject::tr("m/s"));
}

QString TargetModel::formatAltitude(double altitude) const
{
    return QString("%1 %2").arg(altitude, 0, 'f', 0).arg(QObject::tr("m"));
}

QString TargetModel::formatRange(double range) const
{
    return QString("%1 %2").arg(range, 0, 'f', 0).arg(QObject::tr("m"));
}

QColor TargetModel::getTextColorForBackground(const QColor &background) const
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
