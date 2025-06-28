#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>
#include "airplanesimulation.h"

namespace Ui {
class MainView;
}

class QItemSelection;
class MainView : public QWidget
{
    Q_OBJECT

public:
    enum PageIndex {
        AirplaneManagement,
        AirplaneSimulationConfig,
        AreaManagement,
        SimulationControls,
        PageCount
    };

    explicit MainView(QWidget *parent = nullptr);
    ~MainView();

private slots:
    void on_airplane_button_clicked();
    void on_area_button_clicked();
    void on_simu_airplane_button_clicked();
    void on_simulation_button_clicked();
    void on_airplane_init_clicked();
    void on_start_simulation_clicked();
    void on_stop_simulation_clicked();
    void on_simulaotion_apply_button_clicked();
    void on_simulation_save_button_clicked();
    void on_import_area_button_clicked();
    void on_export_area_button_clicked();

    void onAreaSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onAirplaneSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);


private:
    void initViews();
    void initSimulaitons();
    void areaDataLoad();

private:
    Ui::MainView *ui;
};

#endif // MAINVIEW_H
