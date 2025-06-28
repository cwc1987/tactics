#include "simulations.h"

Simulations *Simulations::m_instance = nullptr;
Simulations *Simulations::instance()
{
    if (m_instance == nullptr) {
        m_instance = new Simulations();
    }
    return m_instance;
}

void Simulations::initSimulations()
{
    m_airpleSimulation = new AirplaneSimulation(this);
}

AirplaneSimulation *Simulations::airplaneSimulation()
{
    return m_airpleSimulation;
}

Simulations::Simulations(QObject *parent)
    : QObject{parent}
{

}
