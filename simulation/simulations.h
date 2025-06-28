#ifndef SIMULATIONS_H
#define SIMULATIONS_H

#include <QObject>
#include "airplanesimulation.h"

class Simulations : public QObject
{
    Q_OBJECT
public:
    static Simulations* instance();
    void initSimulations();
    AirplaneSimulation *airplaneSimulation();

private:
    explicit Simulations(QObject *parent = nullptr);
    static Simulations *m_instance;
    AirplaneSimulation *m_airpleSimulation;
};

#endif // SIMULATIONS_H
