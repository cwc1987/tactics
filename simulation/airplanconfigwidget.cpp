#include "airplanconfigwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>

AirplanConfigWidget::AirplanConfigWidget(QWidget *parent)
    : QWidget{parent}
{
    // Initialize AirplaneSettings
    m_airplaneDatas = new AirplaneDatas(this);
    initUI();
}

void AirplanConfigWidget::initUI()
{
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Create tab widget for each airplane
    m_tabWidget = new QTabWidget(this);
    for (int i = 1; i <= 4; ++i) {
        QWidget *airplaneTab = createAirplaneConfigTab(i);
        // Load data for this airplane
        loadAirplaneData(i, airplaneTab);
        m_tabWidget->addTab(airplaneTab, QString(tr("Airplane %1")).arg(i));
    }
    
    mainLayout->addWidget(m_tabWidget);
    
    // Add bottom buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *saveButton = new QPushButton(tr("Save"), this);
    QPushButton *applyButton = new QPushButton(tr("Apply"), this);
    
    // Connect apply button to slot
    connect(applyButton, &QPushButton::clicked, this, &AirplanConfigWidget::onApplyButtonClicked);
    connect(saveButton, &QPushButton::clicked, this, &AirplanConfigWidget::onSaveButtonClicked);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

QWidget* AirplanConfigWidget::createAirplaneConfigTab(int airplaneIndex)
{
    QWidget *tab = new QWidget();
    QVBoxLayout *tabLayout = new QVBoxLayout(tab);
    
    // Basic information group
    QGroupBox *basicGroup = new QGroupBox(tr("Basic Information"), tab);
    QGridLayout *basicLayout = new QGridLayout(basicGroup);
    
    basicLayout->addWidget(new QLabel(tr("ID:")), 0, 0);
    QLineEdit *idEdit = new QLineEdit(basicGroup);
    idEdit->setObjectName(QString("airplane%1_id").arg(airplaneIndex));
    basicLayout->addWidget(idEdit, 0, 1);
    
    basicLayout->addWidget(new QLabel(tr("Name:")), 0, 2);
    QLineEdit *nameEdit = new QLineEdit(basicGroup);
    nameEdit->setObjectName(QString("airplane%1_name").arg(airplaneIndex));
    basicLayout->addWidget(nameEdit, 0, 3);
    
    basicLayout->addWidget(new QLabel(tr("Type:")), 1, 0);
    QComboBox *typeCombo = new QComboBox(basicGroup);
    typeCombo->setObjectName(QString("airplane%1_type").arg(airplaneIndex));
    typeCombo->addItems({tr("Reconnaissance"), tr("Attack"), tr("Transport"), tr("UAV")});
    basicLayout->addWidget(typeCombo, 1, 1);
    
    basicLayout->addWidget(new QLabel(tr("Model:")), 1, 2);
    QLineEdit *modelEdit = new QLineEdit(basicGroup);
    modelEdit->setObjectName(QString("airplane%1_model").arg(airplaneIndex));
    basicLayout->addWidget(modelEdit, 1, 3);
    
    tabLayout->addWidget(basicGroup);
    
    // Position information group
    QGroupBox *positionGroup = new QGroupBox(tr("Position Information"), tab);
    QGridLayout *positionLayout = new QGridLayout(positionGroup);
    
    positionLayout->addWidget(new QLabel(tr("Latitude:")), 0, 0);
    QDoubleSpinBox *latSpin = new QDoubleSpinBox(positionGroup);
    latSpin->setObjectName(QString("airplane%1_latitude").arg(airplaneIndex));
    latSpin->setRange(-90.0, 90.0);
    latSpin->setDecimals(6);
    positionLayout->addWidget(latSpin, 0, 1);
    
    positionLayout->addWidget(new QLabel(tr("Longitude:")), 0, 2);
    QDoubleSpinBox *lonSpin = new QDoubleSpinBox(positionGroup);
    lonSpin->setObjectName(QString("airplane%1_longitude").arg(airplaneIndex));
    lonSpin->setRange(-180.0, 180.0);
    lonSpin->setDecimals(6);
    positionLayout->addWidget(lonSpin, 0, 3);
    
    positionLayout->addWidget(new QLabel(tr("Altitude (m):")), 1, 0);
    QDoubleSpinBox *altSpin = new QDoubleSpinBox(positionGroup);
    altSpin->setObjectName(QString("airplane%1_altitude").arg(airplaneIndex));
    altSpin->setRange(0.0, 50000.0);
    altSpin->setDecimals(1);
    positionLayout->addWidget(altSpin, 1, 1);
    
    positionLayout->addWidget(new QLabel(tr("Heading (°):")), 1, 2);
    QDoubleSpinBox *headingSpin = new QDoubleSpinBox(positionGroup);
    headingSpin->setObjectName(QString("airplane%1_heading").arg(airplaneIndex));
    headingSpin->setRange(0.0, 360.0);
    headingSpin->setDecimals(1);
    positionLayout->addWidget(headingSpin, 1, 3);
    
    positionLayout->addWidget(new QLabel(tr("Speed (m/s):")), 2, 0);
    QDoubleSpinBox *speedSpin = new QDoubleSpinBox(positionGroup);
    speedSpin->setObjectName(QString("airplane%1_speed").arg(airplaneIndex));
    speedSpin->setRange(0.0, 1000.0);
    speedSpin->setDecimals(1);
    positionLayout->addWidget(speedSpin, 2, 1);
    
    positionLayout->addWidget(new QLabel(tr("Vertical Speed (m/s):")), 2, 2);
    QDoubleSpinBox *vSpeedSpin = new QDoubleSpinBox(positionGroup);
    vSpeedSpin->setObjectName(QString("airplane%1_vertical_speed").arg(airplaneIndex));
    vSpeedSpin->setRange(-100.0, 100.0);
    vSpeedSpin->setDecimals(1);
    positionLayout->addWidget(vSpeedSpin, 2, 3);
    
    tabLayout->addWidget(positionGroup);
    
    // Status information group
    QGroupBox *statusGroup = new QGroupBox(tr("Status Information"), tab);
    QGridLayout *statusLayout = new QGridLayout(statusGroup);
    
    statusLayout->addWidget(new QLabel(tr("State:")), 0, 0);
    QComboBox *stateCombo = new QComboBox(statusGroup);
    stateCombo->setObjectName(QString("airplane%1_state").arg(airplaneIndex));
    stateCombo->addItems({tr("Normal"), tr("Fault"), tr("Offline"), tr("Maintenance")});
    statusLayout->addWidget(stateCombo, 0, 1);
    
    statusLayout->addWidget(new QLabel(tr("Flight Duration (h):")), 0, 2);
    QDoubleSpinBox *durationSpin = new QDoubleSpinBox(statusGroup);
    durationSpin->setObjectName(QString("airplane%1_flight_duration").arg(airplaneIndex));
    durationSpin->setRange(0.0, 1000.0);
    durationSpin->setDecimals(1);
    statusLayout->addWidget(durationSpin, 0, 3);
    
    statusLayout->addWidget(new QLabel(tr("Total Distance (km):")), 1, 0);
    QDoubleSpinBox *distanceSpin = new QDoubleSpinBox(statusGroup);
    distanceSpin->setObjectName(QString("airplane%1_total_distance").arg(airplaneIndex));
    distanceSpin->setRange(0.0, 100000.0);
    distanceSpin->setDecimals(1);
    statusLayout->addWidget(distanceSpin, 1, 1);
    
    statusLayout->addWidget(new QLabel(tr("Remaining Oil (%):")), 1, 2);
    QDoubleSpinBox *oilSpin = new QDoubleSpinBox(statusGroup);
    oilSpin->setObjectName(QString("airplane%1_remaining_oil").arg(airplaneIndex));
    oilSpin->setRange(0.0, 100.0);
    oilSpin->setDecimals(1);
    statusLayout->addWidget(oilSpin, 1, 3);
    
    statusLayout->addWidget(new QLabel(tr("Signal Strength (%):")), 2, 0);
    QDoubleSpinBox *signalSpin = new QDoubleSpinBox(statusGroup);
    signalSpin->setObjectName(QString("airplane%1_signal_strength").arg(airplaneIndex));
    signalSpin->setRange(0.0, 100.0);
    signalSpin->setDecimals(1);
    statusLayout->addWidget(signalSpin, 2, 1);
    
    tabLayout->addWidget(statusGroup);
    
    // Sensor information group
    QGroupBox *sensorGroup = new QGroupBox(tr("Sensor Status"), tab);
    QGridLayout *sensorLayout = new QGridLayout(sensorGroup);
    
    QCheckBox *cameraCheck = new QCheckBox(tr("Camera Online"), sensorGroup);
    cameraCheck->setObjectName(QString("airplane%1_camera_online").arg(airplaneIndex));
    sensorLayout->addWidget(cameraCheck, 0, 0);
    
    QCheckBox *gpsCheck = new QCheckBox(tr("GPS Online"), sensorGroup);
    gpsCheck->setObjectName(QString("airplane%1_gps_online").arg(airplaneIndex));
    sensorLayout->addWidget(gpsCheck, 0, 1);
    
    QCheckBox *radarCheck = new QCheckBox(tr("Radar Online"), sensorGroup);
    radarCheck->setObjectName(QString("airplane%1_radar_online").arg(airplaneIndex));
    sensorLayout->addWidget(radarCheck, 0, 2);
    
    tabLayout->addWidget(sensorGroup);
    
    // Control information group
    QGroupBox *controlGroup = new QGroupBox(tr("Control Information"), tab);
    QGridLayout *controlLayout = new QGridLayout(controlGroup);
    
    controlLayout->addWidget(new QLabel(tr("Control State:")), 0, 0);
    QComboBox *controlCombo = new QComboBox(controlGroup);
    controlCombo->setObjectName(QString("airplane%1_control_state").arg(airplaneIndex));
    controlCombo->addItems({tr("Manual"), tr("Auto"), tr("Semi-Auto")});
    controlLayout->addWidget(controlCombo, 0, 1);
    
    controlLayout->addWidget(new QLabel(tr("Communication Channel:")), 0, 2);
    QLineEdit *channelEdit = new QLineEdit(controlGroup);
    channelEdit->setObjectName(QString("airplane%1_communication_channel").arg(airplaneIndex));
    controlLayout->addWidget(channelEdit, 0, 3);
    
    controlLayout->addWidget(new QLabel(tr("Communication Quality (%):")), 1, 0);
    QDoubleSpinBox *qualitySpin = new QDoubleSpinBox(controlGroup);
    qualitySpin->setObjectName(QString("airplane%1_communication_quality").arg(airplaneIndex));
    qualitySpin->setRange(0.0, 100.0);
    qualitySpin->setDecimals(1);
    controlLayout->addWidget(qualitySpin, 1, 1);
    
    tabLayout->addWidget(controlGroup);
    
    // Waypoint information group
    QGroupBox *waypointGroup = new QGroupBox(tr("Waypoint Configuration"), tab);
    QVBoxLayout *waypointLayout = new QVBoxLayout(waypointGroup);
    
    // Create waypoint table
    QTableWidget *waypointTable = new QTableWidget(10, 2, waypointGroup);
    waypointTable->setObjectName(QString("airplane%1_waypoint_table").arg(airplaneIndex));
    waypointTable->setHorizontalHeaderLabels({tr("Latitude"), tr("Longitude")});
    waypointTable->setVerticalHeaderLabels({
        tr("Point 1"), tr("Point 2"), tr("Point 3"), tr("Point 4"), tr("Point 5"),
        tr("Point 6"), tr("Point 7"), tr("Point 8"), tr("Point 9"), tr("Point 10")
    });
    
    // Set table properties
    waypointTable->horizontalHeader()->setStretchLastSection(true);
    waypointTable->setAlternatingRowColors(true);
    
    // Initialize table cells
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 2; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter);
            waypointTable->setItem(row, col, item);
        }
    }
    
    waypointLayout->addWidget(waypointTable);
    tabLayout->addWidget(waypointGroup);    
    return tab;
}

void AirplanConfigWidget::loadAirplaneData(int airplaneIndex, QWidget *tab)
{
    // Load basic information
    QLineEdit *idEdit = tab->findChild<QLineEdit*>(QString("airplane%1_id").arg(airplaneIndex));
    if (idEdit) {
        QString key = QString("airplane%1_id").arg(airplaneIndex);
        idEdit->setText(m_airplaneDatas->getValue(key).toString());
    }
    
    QLineEdit *nameEdit = tab->findChild<QLineEdit*>(QString("airplane%1_name").arg(airplaneIndex));
    if (nameEdit) {
        QString key = QString("airplane%1_name").arg(airplaneIndex);
        nameEdit->setText(m_airplaneDatas->getValue(key).toString());
    }
    
    QComboBox *typeCombo = tab->findChild<QComboBox*>(QString("airplane%1_type").arg(airplaneIndex));
    if (typeCombo) {
        QString key = QString("airplane%1_type").arg(airplaneIndex);
        QString type = m_airplaneDatas->getValue(key).toString();
        int index = typeCombo->findText(type);
        if (index >= 0) typeCombo->setCurrentIndex(index);
    }
    
    QLineEdit *modelEdit = tab->findChild<QLineEdit*>(QString("airplane%1_model").arg(airplaneIndex));
    if (modelEdit) {
        QString key = QString("airplane%1_model").arg(airplaneIndex);
        modelEdit->setText(m_airplaneDatas->getValue(key).toString());
    }
    
    // Load position information
    QDoubleSpinBox *latSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_latitude").arg(airplaneIndex));
    if (latSpin) {
        QString key = QString("airplane%1_latitude").arg(airplaneIndex);
        latSpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    QDoubleSpinBox *lonSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_longitude").arg(airplaneIndex));
    if (lonSpin) {
        QString key = QString("airplane%1_longitude").arg(airplaneIndex);
        lonSpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    QDoubleSpinBox *altSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_altitude").arg(airplaneIndex));
    if (altSpin) {
        QString key = QString("airplane%1_altitude").arg(airplaneIndex);
        altSpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    QDoubleSpinBox *headingSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_heading").arg(airplaneIndex));
    if (headingSpin) {
        QString key = QString("airplane%1_heading").arg(airplaneIndex);
        headingSpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    QDoubleSpinBox *speedSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_speed").arg(airplaneIndex));
    if (speedSpin) {
        QString key = QString("airplane%1_speed").arg(airplaneIndex);
        speedSpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    QDoubleSpinBox *vSpeedSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_vertical_speed").arg(airplaneIndex));
    if (vSpeedSpin) {
        QString key = QString("airplane%1_vertical_speed").arg(airplaneIndex);
        vSpeedSpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    // Load status information
    QComboBox *stateCombo = tab->findChild<QComboBox*>(QString("airplane%1_state").arg(airplaneIndex));
    if (stateCombo) {
        QString key = QString("airplane%1_state").arg(airplaneIndex);
        int state = m_airplaneDatas->getValue(key).toInt();
        if (state >= 0 && state < stateCombo->count()) {
            stateCombo->setCurrentIndex(state);
        }
    }
    
    QDoubleSpinBox *durationSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_flight_duration").arg(airplaneIndex));
    if (durationSpin) {
        QString key = QString("airplane%1_flight_duration").arg(airplaneIndex);
        durationSpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    QDoubleSpinBox *distanceSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_total_distance").arg(airplaneIndex));
    if (distanceSpin) {
        QString key = QString("airplane%1_total_distance").arg(airplaneIndex);
        distanceSpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    QDoubleSpinBox *oilSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_remaining_oil").arg(airplaneIndex));
    if (oilSpin) {
        QString key = QString("airplane%1_remaining_oil").arg(airplaneIndex);
        oilSpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    QDoubleSpinBox *signalSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_signal_strength").arg(airplaneIndex));
    if (signalSpin) {
        QString key = QString("airplane%1_signal_strength").arg(airplaneIndex);
        signalSpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    // Load sensor information
    QCheckBox *cameraCheck = tab->findChild<QCheckBox*>(QString("airplane%1_camera_online").arg(airplaneIndex));
    if (cameraCheck) {
        QString key = QString("airplane%1_camera_online").arg(airplaneIndex);
        cameraCheck->setChecked(m_airplaneDatas->getValue(key).toBool());
    }
    
    QCheckBox *gpsCheck = tab->findChild<QCheckBox*>(QString("airplane%1_gps_online").arg(airplaneIndex));
    if (gpsCheck) {
        QString key = QString("airplane%1_gps_online").arg(airplaneIndex);
        gpsCheck->setChecked(m_airplaneDatas->getValue(key).toBool());
    }
    
    QCheckBox *radarCheck = tab->findChild<QCheckBox*>(QString("airplane%1_radar_online").arg(airplaneIndex));
    if (radarCheck) {
        QString key = QString("airplane%1_radar_online").arg(airplaneIndex);
        radarCheck->setChecked(m_airplaneDatas->getValue(key).toBool());
    }
    
    // Load control information
    QComboBox *controlCombo = tab->findChild<QComboBox*>(QString("airplane%1_control_state").arg(airplaneIndex));
    if (controlCombo) {
        QString key = QString("airplane%1_control_state").arg(airplaneIndex);
        int controlState = m_airplaneDatas->getValue(key).toInt();
        if (controlState >= 0 && controlState < controlCombo->count()) {
            controlCombo->setCurrentIndex(controlState);
        }
    }
    
    QLineEdit *channelEdit = tab->findChild<QLineEdit*>(QString("airplane%1_communication_channel").arg(airplaneIndex));
    if (channelEdit) {
        QString key = QString("airplane%1_communication_channel").arg(airplaneIndex);
        channelEdit->setText(m_airplaneDatas->getValue(key).toString());
    }
    
    QDoubleSpinBox *qualitySpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_communication_quality").arg(airplaneIndex));
    if (qualitySpin) {
        QString key = QString("airplane%1_communication_quality").arg(airplaneIndex);
        qualitySpin->setValue(m_airplaneDatas->getValue(key).toDouble());
    }
    
    // Load waypoint data
    QTableWidget *waypointTable = tab->findChild<QTableWidget*>(QString("airplane%1_waypoint_table").arg(airplaneIndex));
    if (waypointTable) {
        QList<QGV::GeoPos> waypoints = m_airplaneDatas->getWayPoints(airplaneIndex);
        for (int i = 0; i < waypoints.size() && i < 10; ++i) {
            QTableWidgetItem *latItem = waypointTable->item(i, 0);
            QTableWidgetItem *lonItem = waypointTable->item(i, 1);
            if (latItem && lonItem) {
                latItem->setText(QString::number(waypoints[i].latitude(), 'f', 6));
                lonItem->setText(QString::number(waypoints[i].longitude(), 'f', 6));
            }
        }
    }
}

void AirplanConfigWidget::onApplyButtonClicked()
{
    // Save data for all airplanes
    for (int i = 1; i <= 4; ++i) {
        QWidget *tab = m_tabWidget->widget(i - 1);
        if (tab) {
            saveAirplaneData(i, tab);
        }
    }
}

void AirplanConfigWidget::onSaveButtonClicked()
{
    onApplyButtonClicked();
    m_airplaneDatas->saveSettings();
}

void AirplanConfigWidget::saveAirplaneData(int airplaneIndex, QWidget *tab)
{
    // Save basic information
    QLineEdit *idEdit = tab->findChild<QLineEdit*>(QString("airplane%1_id").arg(airplaneIndex));
    if (idEdit) {
        QString key = QString("airplane%1_id").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, idEdit->text());
    }
    
    QLineEdit *nameEdit = tab->findChild<QLineEdit*>(QString("airplane%1_name").arg(airplaneIndex));
    if (nameEdit) {
        QString key = QString("airplane%1_name").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, nameEdit->text());
    }
    
    QComboBox *typeCombo = tab->findChild<QComboBox*>(QString("airplane%1_type").arg(airplaneIndex));
    if (typeCombo) {
        QString key = QString("airplane%1_type").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, typeCombo->currentText());
    }
    
    QLineEdit *modelEdit = tab->findChild<QLineEdit*>(QString("airplane%1_model").arg(airplaneIndex));
    if (modelEdit) {
        QString key = QString("airplane%1_model").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, modelEdit->text());
    }
    
    // Save position information
    QDoubleSpinBox *latSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_latitude").arg(airplaneIndex));
    if (latSpin) {
        QString key = QString("airplane%1_latitude").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, latSpin->value());
    }
    
    QDoubleSpinBox *lonSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_longitude").arg(airplaneIndex));
    if (lonSpin) {
        QString key = QString("airplane%1_longitude").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, lonSpin->value());
    }
    
    QDoubleSpinBox *altSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_altitude").arg(airplaneIndex));
    if (altSpin) {
        QString key = QString("airplane%1_altitude").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, altSpin->value());
    }
    
    QDoubleSpinBox *headingSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_heading").arg(airplaneIndex));
    if (headingSpin) {
        QString key = QString("airplane%1_heading").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, headingSpin->value());
    }
    
    QDoubleSpinBox *speedSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_speed").arg(airplaneIndex));
    if (speedSpin) {
        QString key = QString("airplane%1_speed").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, speedSpin->value());
    }
    
    QDoubleSpinBox *vSpeedSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_vertical_speed").arg(airplaneIndex));
    if (vSpeedSpin) {
        QString key = QString("airplane%1_vertical_speed").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, vSpeedSpin->value());
    }
    
    // Save status information
    QComboBox *stateCombo = tab->findChild<QComboBox*>(QString("airplane%1_state").arg(airplaneIndex));
    if (stateCombo) {
        QString key = QString("airplane%1_state").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, stateCombo->currentIndex());
    }
    
    QDoubleSpinBox *durationSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_flight_duration").arg(airplaneIndex));
    if (durationSpin) {
        QString key = QString("airplane%1_flight_duration").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, durationSpin->value());
    }
    
    QDoubleSpinBox *distanceSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_total_distance").arg(airplaneIndex));
    if (distanceSpin) {
        QString key = QString("airplane%1_total_distance").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, distanceSpin->value());
    }
    
    QDoubleSpinBox *oilSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_remaining_oil").arg(airplaneIndex));
    if (oilSpin) {
        QString key = QString("airplane%1_remaining_oil").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, oilSpin->value());
    }
    
    QDoubleSpinBox *signalSpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_signal_strength").arg(airplaneIndex));
    if (signalSpin) {
        QString key = QString("airplane%1_signal_strength").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, signalSpin->value());
    }
    
    // Save sensor information
    QCheckBox *cameraCheck = tab->findChild<QCheckBox*>(QString("airplane%1_camera_online").arg(airplaneIndex));
    if (cameraCheck) {
        QString key = QString("airplane%1_camera_online").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, cameraCheck->isChecked());
    }
    
    QCheckBox *gpsCheck = tab->findChild<QCheckBox*>(QString("airplane%1_gps_online").arg(airplaneIndex));
    if (gpsCheck) {
        QString key = QString("airplane%1_gps_online").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, gpsCheck->isChecked());
    }
    
    QCheckBox *radarCheck = tab->findChild<QCheckBox*>(QString("airplane%1_radar_online").arg(airplaneIndex));
    if (radarCheck) {
        QString key = QString("airplane%1_radar_online").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, radarCheck->isChecked());
    }
    
    // Save control information
    QComboBox *controlCombo = tab->findChild<QComboBox*>(QString("airplane%1_control_state").arg(airplaneIndex));
    if (controlCombo) {
        QString key = QString("airplane%1_control_state").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, controlCombo->currentIndex());
    }
    
    QLineEdit *channelEdit = tab->findChild<QLineEdit*>(QString("airplane%1_communication_channel").arg(airplaneIndex));
    if (channelEdit) {
        QString key = QString("airplane%1_communication_channel").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, channelEdit->text());
    }
    
    QDoubleSpinBox *qualitySpin = tab->findChild<QDoubleSpinBox*>(QString("airplane%1_communication_quality").arg(airplaneIndex));
    if (qualitySpin) {
        QString key = QString("airplane%1_communication_quality").arg(airplaneIndex);
        m_airplaneDatas->setValue(key, qualitySpin->value());
    }
    
    // Save waypoint data
    QTableWidget *waypointTable = tab->findChild<QTableWidget*>(QString("airplane%1_waypoint_table").arg(airplaneIndex));
    if (waypointTable) {
        for (int i = 1; i <= 10; ++i) {
            QTableWidgetItem *latItem = waypointTable->item(i - 1, 0);
            QTableWidgetItem *lonItem = waypointTable->item(i - 1, 1);
            
            if (latItem && lonItem) {
                bool latOk, lonOk;
                double lat = latItem->text().toDouble(&latOk);
                double lon = lonItem->text().toDouble(&lonOk);
                
                if (latOk && lonOk && lat != 0.0 && lon != 0.0) {
                    QString latKey = QString("airplane%1_waypoint%2_lat").arg(airplaneIndex).arg(i);
                    QString lonKey = QString("airplane%1_waypoint%2_lon").arg(airplaneIndex).arg(i);
                    m_airplaneDatas->setValue(latKey, lat);
                    m_airplaneDatas->setValue(lonKey, lon);
                }
            }
        }
    }
}
