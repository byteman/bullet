#include "mydevices.h"
#include <QGridLayout>
#include "utils.h"
#include <QDir>
#include <QDateTime>
#include <QDebug>
MyDevices::MyDevices(QGroupBox* parent):
    m_container(parent)

{
    m_gds = new FormGds(parent);
    QVBoxLayout* lay = new QVBoxLayout();
    //m_container->setLayout();
    lay->addWidget(m_gds);
    parent->setLayout(lay);

}

void MyDevices::SetOnline(bool online)
{

}

void MyDevices::clearAll()
{

}
#define COL_S 5
#define ROW_S 20

#define MAR_L 5
#define MAR_T 50
#define MAR_R 5
#define MAR_B 20

void MyDevices::Resize()
{

//    int width  = m_container->width()  - (m_col-1)*COL_S - (MAR_L+MAR_R);
//    int height = m_container->height() - (m_row-1)*ROW_S - (MAR_T+MAR_B);

//    int w  = width / m_col;
//    int h  = height / m_row;

    //m_gds->setGeometry(m_container->geometry());

}



void MyDevices::Timeout(int addr)
{

}

void MyDevices::SetUnit(QString unit)
{
    m_unit = unit;

}

void MyDevices::DisplayWeight(int addr, int weight, quint16 state, quint16 dot)
{

}

void MyDevices::ClearDisplay()
{

}
