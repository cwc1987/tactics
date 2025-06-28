#include "areamodel.h"
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QColor>

// AreaItem constructor
AreaItem::AreaItem()
    : areaType(AreaType::TaskArea)
    , minAltitude(0.0)
    , maxAltitude(10000.0)
    , isActive(true)
    , color("#FF0000")
{
    createTime = QDateTime::currentDateTime();
    updateTime = QDateTime::currentDateTime();
}

// AreaItem helper methods
QString AreaItem::getAreaTypeString() const
{
    switch (areaType) {
        case AreaType::TaskArea: return QObject::tr("Task Area");
        case AreaType::NoFlyZone: return QObject::tr("No Fly Zone");
        case AreaType::ThreatZone: return QObject::tr("Threat Zone");
        default: return QObject::tr("Unknown");
    }
}

void AreaItem::addCoordinate(const QString &pointId, double lat, double lon, double alt)
{
    // Check if coordinate point with same ID already exists
    for (auto &coord : coordinates) {
        if (coord.id == pointId) {
            coord.latitude = lat;
            coord.longitude = lon;
            coord.altitude = alt;
            return;
        }
    }
    
    // Add new coordinate point
    coordinates.append(CoordinatePoint(pointId, lat, lon, alt));
}

void AreaItem::removeCoordinate(const QString &pointId)
{
    for (int i = 0; i < coordinates.size(); ++i) {
        if (coordinates[i].id == pointId) {
            coordinates.removeAt(i);
            break;
        }
    }
}

AreaItem::CoordinatePoint AreaItem::getCoordinate(const QString &pointId) const
{
    for (const auto &coord : coordinates) {
        if (coord.id == pointId) {
            return coord;
        }
    }
    return CoordinatePoint();
}

void AreaItem::clearCoordinates()
{
    coordinates.clear();
}

// AreaModel constructor
AreaModel::AreaModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

// QAbstractTableModel interface implementation
int AreaModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

int AreaModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant AreaModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return QVariant();

    const AreaItem &item = m_items[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case Id: return item.id;
            case Name: return item.name;
            case Type: return item.getAreaTypeString();
            case Description: return item.description;
            case CreateTime: return item.createTime.toString("yyyy-MM-dd hh:mm:ss");
            case UpdateTime: return item.updateTime.toString("yyyy-MM-dd hh:mm:ss");
            case CoordinateCount: return QString::number(item.coordinates.size());
            case MinAltitude: return QString::number(item.minAltitude);
            case MaxAltitude: return QString::number(item.maxAltitude);
            case Color: return item.color;
            default: return QVariant();
        }
    }
    
    // Handle background color for Color column
    if (role == Qt::BackgroundRole && index.column() == Color) {
        QColor bgColor(item.color);
        if (bgColor.isValid()) {
            return bgColor;
        }
        return QVariant();
    }
    
    // Handle foreground (text) color for Color column
    if (role == Qt::ForegroundRole && index.column() == Color) {
        QColor bgColor(item.color);
        if (bgColor.isValid()) {
            // Calculate luminance to determine text color
            // Use white text on dark backgrounds, black text on light backgrounds
            double luminance = 0.299 * bgColor.red() + 0.587 * bgColor.green() + 0.114 * bgColor.blue();
            return luminance > 128 ? QColor(Qt::black) : QColor(Qt::white);
        }
        return QVariant();
    }
    
    return QVariant();
}

QVariant AreaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case Id: return tr("ID");
            case Name: return tr("Name");
            case Type: return tr("Type");
            case Description: return tr("Description");
            case CreateTime: return tr("Create Time");
            case UpdateTime: return tr("Update Time");
            case CoordinateCount: return tr("Coordinate Count");
            case MinAltitude: return tr("Min Altitude");
            case MaxAltitude: return tr("Max Altitude");
            case Color: return tr("Color");
            default: return QVariant();
        }
    }
    
    return QVariant();
}

Qt::ItemFlags AreaModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(index.column() == Name || index.column() == Description)
        flags = flags | Qt::ItemIsEditable;
    return flags;
}

bool AreaModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_items.size() || role != Qt::EditRole)
        return false;

    AreaItem &area = m_items[index.row()];

    switch (index.column()) {
    case Name:
        area.name = value.toString();
        break;
    case Description:
        area.description = value.toString();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    emit areaUpdated(area);
    return true;
}

// Data operation methods implementation
void AreaModel::addArea(const AreaItem &area)
{
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(area);
    endInsertRows();
    emit areaAdded(area);
}

void AreaModel::removeArea(int row)
{
    if (row < 0 || row >= m_items.size())
        return;

    AreaItem area = m_items[row];
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
    emit areaRemoved(area);
}

void AreaModel::updateArea(int row, const AreaItem &area)
{
    if (row < 0 || row >= m_items.size())
        return;
        
    m_items[row] = area;
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
    emit areaUpdated(area);
}

AreaItem AreaModel::getArea(int row) const
{
    if (row >= 0 && row < m_items.size())
        return m_items[row];
    return AreaItem();
}

void AreaModel::clearAll()
{
    if (m_items.isEmpty())
        return;
        
    beginResetModel();
    m_items.clear();
    endResetModel();
    emit areaReset();
}

void AreaModel::setAreas(const QList<AreaItem> &areas)
{
    beginResetModel();
    m_items = areas;
    endResetModel();
    emit areaReset();
}

// Area search methods implementation
AreaItem AreaModel::findAreaById(const QString &id) const
{
    for (const auto &area : m_items) {
        if (area.id == id) {
            return area;
        }
    }
    return AreaItem();
}

QList<AreaItem> AreaModel::findAreasByType(AreaItem::AreaType type) const
{
    QList<AreaItem> result;
    for (const auto &area : m_items) {
        if (area.areaType == type) {
            result.append(area);
        }
    }
    return result;
}

// JSON import/export methods implementation
bool AreaModel::exportToJson(const QString &filePath) const
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

bool AreaModel::importFromJson(const QString &filePath)
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

QString AreaModel::exportToJsonString() const
{
    QJsonArray areasArray;
    
    for (const AreaItem &area : m_items) {
        QJsonObject areaObject;
        
        // Basic properties
        areaObject["id"] = area.id;
        areaObject["name"] = area.name;
        areaObject["areaType"] = static_cast<int>(area.areaType);
        areaObject["description"] = area.description;
        areaObject["createTime"] = area.createTime.toString(Qt::ISODate);
        areaObject["updateTime"] = area.updateTime.toString(Qt::ISODate);
        
        // Coordinates
        QJsonArray coordinatesArray;
        for (const AreaItem::CoordinatePoint &coord : area.coordinates) {
            QJsonObject coordObject;
            coordObject["id"] = coord.id;
            coordObject["latitude"] = coord.latitude;
            coordObject["longitude"] = coord.longitude;
            coordObject["altitude"] = coord.altitude;
            coordinatesArray.append(coordObject);
        }
        areaObject["coordinates"] = coordinatesArray;
        
        // Area properties
        areaObject["minAltitude"] = area.minAltitude;
        areaObject["maxAltitude"] = area.maxAltitude;
        areaObject["color"] = area.color;
        
        areasArray.append(areaObject);
    }
    
    QJsonObject rootObject;
    rootObject["areas"] = areasArray;
    rootObject["version"] = "1.0";
    rootObject["exportTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    QJsonDocument doc(rootObject);
    return doc.toJson(QJsonDocument::Indented);
}

bool AreaModel::importFromJsonString(const QString &jsonString)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    
    if (error.error != QJsonParseError::NoError) {
        return false;
    }
    
    QJsonObject rootObject = doc.object();
    QJsonArray areasArray = rootObject["areas"].toArray();
    
    QList<AreaItem> newAreas;
    
    for (const QJsonValue &value : areasArray) {
        QJsonObject areaObject = value.toObject();
        AreaItem area;
        
        // Basic properties
        area.id = areaObject["id"].toString();
        area.name = areaObject["name"].toString();
        area.areaType = static_cast<AreaItem::AreaType>(areaObject["areaType"].toInt());
        area.description = areaObject["description"].toString();
        area.createTime = QDateTime::fromString(areaObject["createTime"].toString(), Qt::ISODate);
        area.updateTime = QDateTime::fromString(areaObject["updateTime"].toString(), Qt::ISODate);
        
        // Coordinates
        QJsonArray coordinatesArray = areaObject["coordinates"].toArray();
        for (const QJsonValue &coordValue : coordinatesArray) {
            QJsonObject coordObject = coordValue.toObject();
            AreaItem::CoordinatePoint coord;
            coord.id = coordObject["id"].toString();
            coord.latitude = coordObject["latitude"].toDouble();
            coord.longitude = coordObject["longitude"].toDouble();
            coord.altitude = coordObject["altitude"].toDouble();
            area.coordinates.append(coord);
        }
        
        // Area properties
        area.minAltitude = areaObject["minAltitude"].toDouble();
        area.maxAltitude = areaObject["maxAltitude"].toDouble();
        area.isActive = false;
        area.color = areaObject["color"].toString();
        
        newAreas.append(area);
    }
    
    setAreas(newAreas);
    return true;
}
