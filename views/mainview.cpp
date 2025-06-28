#include "mainview.h"
#include "ui_mainview.h"
#include <QFileDialog>
#include <QFile>
#include "modelconfig.h"
#include "areamodel.h"
#include "airplanemodel.h"
#include "simulations.h"

MainView::MainView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);
    setWindowTitle(tr("Data Views"));
    initViews();
    areaDataLoad();
    ui->stackedWidget->setCurrentIndex(AirplaneManagement);
}

MainView::~MainView()
{
    delete ui;
}

void MainView::on_airplane_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(AirplaneManagement);
}

void MainView::on_area_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(AreaManagement);
}

void MainView::onAreaSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    AreaModel *areaModel = qobject_cast<AreaModel*>(ModelConfig::instance()->getModel(ModelConfig::Area));
    QModelIndexList selectedList = selected.indexes();
    for(auto &selectedIndex : selectedList) {
        AreaItem area = areaModel->getArea(selectedIndex.row());
        area.isActive = true;
        areaModel->updateArea(selectedIndex.row(), area);
    }

    QModelIndexList deselectedList = deselected.indexes();
    for(auto &deselectedIndex : deselectedList) {
        AreaItem area = areaModel->getArea(deselectedIndex.row());
        area.isActive = false;
        areaModel->updateArea(deselectedIndex.row(), area);
    }
}

void MainView::onAirplaneSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    AirplaneModel *airplaneModel = qobject_cast<AirplaneModel*>(ModelConfig::instance()->getModel(ModelConfig::Airplane));
    QModelIndexList selectedList = selected.indexes();
    for(auto &selectedIndex : selectedList) {
        AirPlaneItem aiprplane = airplaneModel->getAirplane(selectedIndex.row());
        aiprplane.isActive = true;
        airplaneModel->updateAirplane(selectedIndex.row(), aiprplane);
    }

    QModelIndexList deselectedList = deselected.indexes();
    for(auto &deselectedIndex : deselectedList) {
        AirPlaneItem aiprplane = airplaneModel->getAirplane(deselectedIndex.row());
        aiprplane.isActive = true;
        airplaneModel->updateAirplane(deselectedIndex.row(), aiprplane);
    }
}

void MainView::initViews()
{
    auto *airplaneModel = ModelConfig::instance()->getModel(ModelConfig::Airplane);
    auto *areaModel = ModelConfig::instance()->getModel(ModelConfig::Area);

    ui->airplane_tableview->setModel(airplaneModel);
    ui->area_tableview->setModel(areaModel);

    connect(ui->area_tableview->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainView::onAreaSelectionChanged);
    connect(ui->airplane_tableview->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainView::onAirplaneSelectionChanged);
}

void MainView::areaDataLoad()
{
    AreaModel *areaModel = qobject_cast<AreaModel*>(ModelConfig::instance()->getModel(ModelConfig::Area));
    QString defaultAreaPath = QApplication::applicationDirPath() + "/default_area.json";
    if(QFile::exists(defaultAreaPath)) {
        areaModel->importFromJson(defaultAreaPath);
    }
}

void MainView::on_simu_airplane_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(AirplaneSimulationConfig);
}

void MainView::on_simulation_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(SimulationControls);
}

void MainView::on_airplane_init_clicked()
{
    Simulations::instance()->airplaneSimulation()->initAirplane();
}

void MainView::on_start_simulation_clicked()
{
    Simulations::instance()->airplaneSimulation()->startSimulation();
    ui->simulaotion_apply_button->setDisabled(true);
    ui->simulation_save_button->setDisabled(true);
}

void MainView::on_stop_simulation_clicked()
{
    Simulations::instance()->airplaneSimulation()->stopSimulation();
    ui->simulaotion_apply_button->setDisabled(false);
    ui->simulation_save_button->setDisabled(false);
}

void MainView::on_simulaotion_apply_button_clicked()
{
    ui->airplane_simulation_widget->applySettings();
    Simulations::instance()->airplaneSimulation()->reloadSettings();
}

void MainView::on_simulation_save_button_clicked()
{
    ui->airplane_simulation_widget->applySettings();
    SimulationSettings::instance()->saveSettings();
}


void MainView::on_import_area_button_clicked()
{
    AreaModel *areaModel = qobject_cast<AreaModel*>(ModelConfig::instance()->getModel(ModelConfig::Area));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Area File"),
                                                    QApplication::applicationDirPath(),
                                                    tr("JSON (*.json)"));
    areaModel->importFromJson(fileName);
}

void MainView::on_export_area_button_clicked()
{
    AreaModel *areaModel = qobject_cast<AreaModel*>(ModelConfig::instance()->getModel(ModelConfig::Area));
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Area File(deault_area.json auto open)"),
                                                   QApplication::applicationDirPath() + "/default_area.json",
                                                    tr("JSON (*.json)"));
    areaModel->exportToJson(fileName);
}

