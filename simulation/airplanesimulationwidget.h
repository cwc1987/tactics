#ifndef AIRPLANESIMULATIONWIDGET_H
#define AIRPLANESIMULATIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QFormLayout>
#include "simulationsettings.h"

class AirplaneSimulationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AirplaneSimulationWidget(QWidget *parent = nullptr);
    void applySettings();

private:
    void initUI();
    void loadCurrentSettings();

private:
    // Speed settings
    QDoubleSpinBox *m_takeoffSpeedSpinBox;
    QDoubleSpinBox *m_cruiseSpeedSpinBox;
    QDoubleSpinBox *m_landingSpeedSpinBox;
    
    // Flight control settings
    QDoubleSpinBox *m_turnRateSpinBox;
    QDoubleSpinBox *m_climbRateSpinBox;
    QDoubleSpinBox *m_descentRateSpinBox;
    QDoubleSpinBox *m_waypointRadiusSpinBox;
    
    // Simulation settings
    QSpinBox *m_simulationIntervalSpinBox;
};

#endif // AIRPLANESIMULATIONWIDGET_H
