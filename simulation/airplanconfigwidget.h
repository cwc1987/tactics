#ifndef AIRPLANCONFIGWIDGET_H
#define AIRPLANCONFIGWIDGET_H

#include <QWidget>
#include "airplanedatas.h"

class QTabWidget;

class AirplanConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AirplanConfigWidget(QWidget *parent = nullptr);

private slots:
    void onApplyButtonClicked();
    void onSaveButtonClicked();

private:
    void initUI();
    QWidget* createAirplaneConfigTab(int airplaneIndex);
    void loadAirplaneData(int airplaneIndex, QWidget *tab);
    void saveAirplaneData(int airplaneIndex, QWidget *tab);
    
private:
    AirplaneDatas *m_airplaneDatas;
    QTabWidget *m_tabWidget;
};

#endif // AIRPLANCONFIGWIDGET_H
