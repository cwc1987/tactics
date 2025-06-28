#include "airplanesimulationwidget.h"
#include <QMessageBox>
#include <QApplication>

AirplaneSimulationWidget::AirplaneSimulationWidget(QWidget *parent)
    : QWidget{parent}
    , m_takeoffSpeedSpinBox(nullptr)
    , m_cruiseSpeedSpinBox(nullptr)
    , m_landingSpeedSpinBox(nullptr)
    , m_turnRateSpinBox(nullptr)
    , m_climbRateSpinBox(nullptr)
    , m_descentRateSpinBox(nullptr)
    , m_waypointRadiusSpinBox(nullptr)
    , m_simulationIntervalSpinBox(nullptr)
{
    initUI();
    loadCurrentSettings();
}

void AirplaneSimulationWidget::applySettings()
{
    SimulationSettings *settings = SimulationSettings::instance();
    // Save all settings using public methods
    settings->setDouble(SimulationSettingsKey::TakeoffSpeed, m_takeoffSpeedSpinBox->value());
    settings->setDouble(SimulationSettingsKey::CruiseSpeed, m_cruiseSpeedSpinBox->value());
    settings->setDouble(SimulationSettingsKey::LandingSpeed, m_landingSpeedSpinBox->value());
    settings->setDouble(SimulationSettingsKey::TurnRate, m_turnRateSpinBox->value());
    settings->setDouble(SimulationSettingsKey::ClimbRate, m_climbRateSpinBox->value());
    settings->setDouble(SimulationSettingsKey::DescentRate, m_descentRateSpinBox->value());
    settings->setDouble(SimulationSettingsKey::WaypointRadius, m_waypointRadiusSpinBox->value());
    settings->setValue(SimulationSettingsKey::SimulationInterval, m_simulationIntervalSpinBox->value());
}

void AirplaneSimulationWidget::initUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Speed settings group
    QGroupBox *speedGroup = new QGroupBox(tr("Speed Settings (m/s)"), this);
    QFormLayout *speedLayout = new QFormLayout(speedGroup);
    
    m_takeoffSpeedSpinBox = new QDoubleSpinBox(this);
    m_takeoffSpeedSpinBox->setRange(1.0, 1000.0);
    m_takeoffSpeedSpinBox->setSuffix(" m/s");
    m_takeoffSpeedSpinBox->setDecimals(1);
    speedLayout->addRow(tr("Takeoff Speed:"), m_takeoffSpeedSpinBox);
    
    m_cruiseSpeedSpinBox = new QDoubleSpinBox(this);
    m_cruiseSpeedSpinBox->setRange(0.0, 2000.0);
    m_cruiseSpeedSpinBox->setSuffix(" m/s");
    m_cruiseSpeedSpinBox->setDecimals(1);
    speedLayout->addRow(tr("Cruise Speed:"), m_cruiseSpeedSpinBox);
    
    m_landingSpeedSpinBox = new QDoubleSpinBox(this);
    m_landingSpeedSpinBox->setRange(0.0, 500.0);
    m_landingSpeedSpinBox->setSuffix(" m/s");
    m_landingSpeedSpinBox->setDecimals(1);
    speedLayout->addRow(tr("Landing Speed:"), m_landingSpeedSpinBox);
    
    mainLayout->addWidget(speedGroup);
    
    // Flight control settings group
    QGroupBox *flightControlGroup = new QGroupBox(tr("Flight Control Settings"), this);
    QFormLayout *flightControlLayout = new QFormLayout(flightControlGroup);
    
    m_turnRateSpinBox = new QDoubleSpinBox(this);
    m_turnRateSpinBox->setRange(1.0, 90.0);
    m_turnRateSpinBox->setSuffix(" °/s");
    m_turnRateSpinBox->setDecimals(1);
    flightControlLayout->addRow(tr("Turn Rate:"), m_turnRateSpinBox);
    
    m_climbRateSpinBox = new QDoubleSpinBox(this);
    m_climbRateSpinBox->setRange(1.0, 200.0);
    m_climbRateSpinBox->setSuffix(" m/s");
    m_climbRateSpinBox->setDecimals(1);
    flightControlLayout->addRow(tr("Climb Rate:"), m_climbRateSpinBox);
    
    m_descentRateSpinBox = new QDoubleSpinBox(this);
    m_descentRateSpinBox->setRange(1.0, 100.0);
    m_descentRateSpinBox->setSuffix(" m/s");
    m_descentRateSpinBox->setDecimals(1);
    flightControlLayout->addRow(tr("Descent Rate:"), m_descentRateSpinBox);
    
    m_waypointRadiusSpinBox = new QDoubleSpinBox(this);
    m_waypointRadiusSpinBox->setRange(10.0, 2000.0);
    m_waypointRadiusSpinBox->setSuffix(" m");
    m_waypointRadiusSpinBox->setDecimals(1);
    flightControlLayout->addRow(tr("Waypoint Radius:"), m_waypointRadiusSpinBox);
    
    mainLayout->addWidget(flightControlGroup);
    
    // Simulation settings group
    QGroupBox *simulationGroup = new QGroupBox(tr("Simulation Settings"), this);
    QFormLayout *simulationLayout = new QFormLayout(simulationGroup);
    
    m_simulationIntervalSpinBox = new QSpinBox(this);
    m_simulationIntervalSpinBox->setRange(10, 1000);
    m_simulationIntervalSpinBox->setSuffix(" ms");
    simulationLayout->addRow(tr("Simulation Interval:"), m_simulationIntervalSpinBox);
    
    mainLayout->addWidget(simulationGroup);
    setLayout(mainLayout);
}

void AirplaneSimulationWidget::loadCurrentSettings()
{
    SimulationSettings *settings = SimulationSettings::instance();
    
    // Load speed settings
    m_takeoffSpeedSpinBox->setValue(settings->getDouble(SimulationSettingsKey::TakeoffSpeed));
    m_cruiseSpeedSpinBox->setValue(settings->getDouble(SimulationSettingsKey::CruiseSpeed));
    m_landingSpeedSpinBox->setValue(settings->getDouble(SimulationSettingsKey::LandingSpeed));
    
    // Load flight control settings
    m_turnRateSpinBox->setValue(settings->getDouble(SimulationSettingsKey::TurnRate));
    m_climbRateSpinBox->setValue(settings->getDouble(SimulationSettingsKey::ClimbRate));
    m_descentRateSpinBox->setValue(settings->getDouble(SimulationSettingsKey::DescentRate));
    m_waypointRadiusSpinBox->setValue(settings->getDouble(SimulationSettingsKey::WaypointRadius));
    
    // Load simulation settings
    m_simulationIntervalSpinBox->setValue(settings->getValue(SimulationSettingsKey::SimulationInterval).toInt());
}
