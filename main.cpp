#include <QApplication>
#include <QTranslator>
#include "settings.h"
#include "situationwidget.h"
#include "mainview.h"
#include "modelconfig.h"
#include "simulations.h"
#include "simulationsettings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator qTranslator;
    qTranslator.load(":/tactics_zh_CN.qm");
    a.installTranslator(&qTranslator);

    Settings::instance()->initSettings();
    SimulationSettings::instance()->initSettings();
    ModelConfig::instance()->initModels();
    Simulations::instance()->initSimulations();

    //simulation to model
    QObject::connect(Simulations::instance()->airplaneSimulation(),
                     &AirplaneSimulation::airplaneInited,
                     ModelConfig::instance(), &ModelConfig::onAirplaneInit);
    QObject::connect(Simulations::instance()->airplaneSimulation(),
                     &AirplaneSimulation::airplaneUpdated,
                     ModelConfig::instance(), &ModelConfig::onAirplaneUpdated);

    SituationWidget situationview;
    //situation to model config
    QObject::connect(&situationview, &SituationWidget::newSelectionArea,
                     ModelConfig::instance(), &ModelConfig::onNewSelectionArea);
    //area model config to situation
    QObject::connect(ModelConfig::instance(), &ModelConfig::areaAdded,
                     &situationview, &SituationWidget::onAreaAdded);
    QObject::connect(ModelConfig::instance(), &ModelConfig::areaRemoved,
                     &situationview, &SituationWidget::onAreaRemoved);
    QObject::connect(ModelConfig::instance(), &ModelConfig::areaUpdated,
                     &situationview, &SituationWidget::onAreaUpdated);
    QObject::connect(ModelConfig::instance(), &ModelConfig::areaReset,
                     &situationview, &SituationWidget::onAreaReset);

    //airplane model config to situation
    QObject::connect(ModelConfig::instance(), &ModelConfig::airplaneAdded,
                     &situationview, &SituationWidget::onAirplaneAdded);
    QObject::connect(ModelConfig::instance(), &ModelConfig::airplaneRemoved,
                     &situationview, &SituationWidget::onAirplaneRemoved);
    QObject::connect(ModelConfig::instance(), &ModelConfig::airplaneUpdated,
                     &situationview, &SituationWidget::onAirplaneUpdated);
    QObject::connect(ModelConfig::instance(), &ModelConfig::airplaneReset,
                     &situationview, &SituationWidget::onAirplaneReset);

    MainView mainview;

    mainview.showMaximized();
    situationview.showMaximized();
    return a.exec();
}
