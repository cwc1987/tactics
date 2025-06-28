#ifndef AREAMODEL_H
#define AREAMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QList>
#include <QString>
#include <QPointF>
#include <QDateTime>

class AreaItem {
public:
    // Area type enumeration
    enum AreaType {
        TaskArea = 0,       // Task Area
        NoFlyZone = 1,      // No Fly Zone
        ThreatZone = 2      // Threat Zone
    };

    // Coordinate point structure
    struct CoordinatePoint {
        QString id;         // Coordinate point ID
        double latitude;    // Latitude
        double longitude;   // Longitude
        double altitude;    // Altitude (optional)
        
        CoordinatePoint() : latitude(0.0), longitude(0.0), altitude(0.0) {}
        CoordinatePoint(const QString &pointId, double lat, double lon, double alt = 0.0)
            : id(pointId), latitude(lat), longitude(lon), altitude(alt) {}
    };

    AreaItem();
    
    QString id;                     // Unique identifier
    QString name;                   // Name
    AreaType areaType;              // Type
    QString description;            // Description
    QDateTime createTime;           // Create time
    QDateTime updateTime;           // Update time
    
    QList<CoordinatePoint> coordinates;
    
    double minAltitude;             // Minimum altitude
    double maxAltitude;             // Maximum altitude
    bool isActive;                  // Is active
    QString color;                  // Display color
    
    // Helper methods
    QString getAreaTypeString() const;
    void addCoordinate(const QString &pointId, double lat, double lon, double alt = 0.0);
    void removeCoordinate(const QString &pointId);
    CoordinatePoint getCoordinate(const QString &pointId) const;
    void clearCoordinates();

    bool operator==(const AreaItem &other) const {
        return id == other.id;
    }
};

class AreaModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit AreaModel(QObject *parent = nullptr);
    
    // QAbstractTableModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Data operation methods
    void addArea(const AreaItem &area);
    void removeArea(int row);
    void updateArea(int row, const AreaItem &area);
    AreaItem getArea(int row) const;
    void clearAll();
    
    // Data access
    const QList<AreaItem>& getAreas() const { return m_items; }
    void setAreas(const QList<AreaItem> &areas);
    
    // Area search methods
    AreaItem findAreaById(const QString &id) const;
    QList<AreaItem> findAreasByType(AreaItem::AreaType type) const;
    
    // JSON import/export methods
    bool exportToJson(const QString &filePath) const;
    bool importFromJson(const QString &filePath);
    QString exportToJsonString() const;
    bool importFromJsonString(const QString &jsonString);
signals:
    void areaAdded(const AreaItem &area);
    void areaRemoved(const AreaItem &area);
    void areaUpdated(const AreaItem &area);
    void areaReset();
    
private:
    enum Columns {
        Id = 0,
        Name,
        Type,
        Description,
        CreateTime,
        UpdateTime,
        CoordinateCount,
        MinAltitude,
        MaxAltitude,
        Color,
        ColumnCount
    };

    QList<AreaItem> m_items;
};

#endif // AREAMODEL_H
