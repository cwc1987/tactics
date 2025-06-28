#include "modelconfig.h"
#include "airplanemodel.h"
#include "communitionmodel.h"
#include "missionmodel.h"
#include "targetmodel.h"
#include "areamodel.h"
#include "settings.h"
#include <QString>
#include <QRandomGenerator>

ModelConfig *ModelConfig::m_instance = nullptr;
ModelConfig *ModelConfig::instance()
{
    if (m_instance == nullptr) {
        m_instance = new ModelConfig();
    }
    return m_instance;
}

void ModelConfig::initModels()
{
    m_airplaneModel = new AirplaneModel();
    m_areaModel = new AreaModel();
    m_missionModel = new MissionModel();
    m_communitionModel = new CommunitionModel();
    m_targetModel = new TargetModel();
    initModelSignals();
}

QAbstractItemModel *ModelConfig::getModel(const ModelType &type)
{
    if(type == ModelType::Airplane)
    {
        return m_airplaneModel;
    }
    else if(type == ModelType::Area)
    {
        return m_areaModel;
    }
    else if(type == ModelType::Mission)
    {
        return m_missionModel;
    }
    else if(type == ModelType::Communition)
    {
        return m_communitionModel;
    }
    else if(type == ModelType::Target)
    {
        return m_targetModel;
    }
    else
    {
        return nullptr;
    }
}

void ModelConfig::onNewSelectionArea(QVector<QGV::GeoPos> geoPosVec, int type)
{
    AreaItem areaItem;
    areaItem.id = QString::number(generateAreaItemId());
    if(areaItem.id == -1)
        return;
    areaItem.name = "Area" + areaItem.id;
    areaItem.areaType = static_cast<AreaItem::AreaType>(type);
    areaItem.description = "Area" + areaItem.id;
    areaItem.createTime = QDateTime::currentDateTime();
    areaItem.updateTime = QDateTime::currentDateTime();
    areaItem.minAltitude = 0;
    areaItem.maxAltitude = 0;
    areaItem.isActive = false;
    areaItem.color = getAreaColor(type);
    for(const auto& geoPos : geoPosVec)
    {
        AreaItem::CoordinatePoint coordinatePoint;
        coordinatePoint.latitude = geoPos.latitude();
        coordinatePoint.longitude = geoPos.longitude();
        coordinatePoint.altitude = 0;
        areaItem.coordinates.append(coordinatePoint);
    }
    m_areaModel->addArea(areaItem);
}

void ModelConfig::onAirplaneInit(const QList<AirPlaneItem> &items)
{
    m_airplaneModel->clearAll();
    for(int i = 0, iend = items.count(); i < iend; i++)
        m_airplaneModel->addAirplane(items[i]);
}

void ModelConfig::onAirplaneUpdated(const AirPlaneItem &airplane)
{
    int row = m_airplaneModel->findAirplaneById(airplane.id);
    if(row != -1)
        m_airplaneModel->updateAirplane(row, airplane);
}

ModelConfig::ModelConfig(QObject *parent)
    : QObject{parent}, m_airplaneModel(nullptr)
    , m_areaModel(nullptr), m_missionModel(nullptr)
    , m_communitionModel(nullptr), m_targetModel(nullptr)
{
    qRegisterMetaType<AirPlaneItem>("AirPlaneItem");
}

void ModelConfig::initModelSignals()
{
    //area
    connect(m_areaModel, &AreaModel::areaAdded, this, [this](const AreaItem &area) {
        emit areaAdded(area);
    });
    connect(m_areaModel, &AreaModel::areaRemoved, this, [this](const AreaItem &area) {
        emit areaRemoved(area);
    });
    connect(m_areaModel, &AreaModel::areaUpdated, this, [this](const AreaItem &area) {
        emit areaUpdated(area);
    });
    connect(m_areaModel, &AreaModel::areaReset, this, [this]() {
        emit areaReset();
    });

    connect(m_airplaneModel, &AirplaneModel::airplaneAdded, this, [this](const AirPlaneItem &airplane) {
        emit airplaneAdded(airplane);
    });
    connect(m_airplaneModel, &AirplaneModel::airplaneRemoved, this, [this](const QString &id) {
        emit airplaneRemoved(id);
    });
    connect(m_airplaneModel, &AirplaneModel::airplaneUpdated, this, [this](const AirPlaneItem &airplane) {
        emit airplaneUpdated(airplane);
    });
    connect(m_airplaneModel, &AirplaneModel::airplaneReset, this, [this]() {
        emit airplaneReset();
    });
}

int ModelConfig::generateAreaItemId() const
{
    // Check if AreaModel has more than 256 items
    if (m_areaModel->rowCount() >= 256) {
        return -1;
    }
    
    // Get all existing areas
    const QList<AreaItem>& areas = m_areaModel->getAreas();
    
    // Try to find an available ID from 0 to 255
    for (int id = 0; id <= 255; ++id) {
        QString idStr = QString::number(id);
        bool idExists = false;
        
        // Check if this ID already exists
        for (const auto& area : areas) {
            if (area.id == idStr) {
                idExists = true;
                break;
            }
        }
        
        // If ID doesn't exist, return it
        if (!idExists) {
            return id;
        }
    }
    
    // If all IDs from 0-255 are used, return -1
    return -1;
}

QString ModelConfig::getAreaColor(int type) const
{
    if(type == 0) {
        return Settings::instance()->getColor(SettingsKey::TaskAreaColor).name(QColor::HexArgb);
    } else if(type == 1) {
        return Settings::instance()->getColor(SettingsKey::NoFlayAreaColor).name(QColor::HexArgb);
    } else if(type == 2) {
        return Settings::instance()->getColor(SettingsKey::ThreatAreaColor).name(QColor::HexArgb);
    } else {
        return QString("#FFFFFFFF");
    }
}
