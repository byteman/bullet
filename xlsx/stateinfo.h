#ifndef STATEINFO_H
#define STATEINFO_H

#include <QObject>
#include <QMap>
/**
 * @brief 电芯测试状态信息
 */
struct CellState
{
    CellState();
    QString Temp; //温度
    QString TestNo ; //测试单号
    QString TestUser; //测试人员
    QString ProjGroup; //项目组
    QString PressDev; //压力设备名称
    QString PressDevId; 	//压力设备编号
    qint32 PressDevChan; 	//压力设备通道编号
    QString ChargeDev; //控制柜通道数据
    QString CellNo; //电芯编号
    QString TestState; //测试状态 下柜 在柜
};
//订单对应电芯
typedef QMap<QString,QVector<CellState>> CellTestOrderList;


typedef QMap<QString,CellTestOrderList> CellTestHost;

#endif // STATEINFO_H
