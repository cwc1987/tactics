#ifndef AREASELECT_H
#define AREASELECT_H

#include <QGVWidget.h>
#include <QPushButton>
#include <QVBoxLayout>

class AreaSelect : public QGVWidget
{
    Q_OBJECT
public:
    explicit AreaSelect();

signals:
    void taskAreaSelected();
    void noFlyAreaSelected();
    void threatAreaSelected();

private slots:
    void onTaskAreaClicked();
    void onNoFlyAreaClicked();
    void onThreatAreaClicked();

private:
    QPushButton* m_taskAreaButton;
    QPushButton* m_noFlyAreaButton;
    QPushButton* m_threatAreaButton;
    QVBoxLayout* m_layout;
};

#endif // AREASELECT_H
