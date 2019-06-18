#ifndef MYDEVICES_H
#define MYDEVICES_H

#include <QObject>
#include <QVector>
#include <QGroupBox>
#include <QGridLayout>
#include <QMap>
#include "formgds.h"
#include "models.h"
class MyDevices:public QObject
{
    Q_OBJECT
public:
    MyDevices(QGroupBox* parent=NULL );

    void Timeout(int addr);
    void SetUnit(QString unit);

    void DisplayWeight(int addr,int weight, quint16 state,quint16 dot);

    void ClearDisplay();

    void Resize();

    void SetOnline(bool online);

private:

    void clearAll();

    QGroupBox*      m_container;
    QGridLayout*    qlayout;
    FormGds*        m_gds;
    QString         m_unit;

};

#endif // MYDEVICES_H
