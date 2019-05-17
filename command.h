#ifndef COMMAND_H
#define COMMAND_H
#include <QObject>
#include <QVector>
enum Command{

    CMD_CLEAR_ZERO = 0, //清零
    CMD_READ_WGT = 1, //查询单个或者全部称台的重量和状态
    CMD_QUERY_CHANGE_SENSOR, //查询重量有更改的称台
    CMD_QUERY_ERROR_SENSOR, //查询有故障的传感器.
    CMD_SET_WGT, //设置称台重量
    CMD_CUSTOM_READ_AD=105, //读取AD值
    CMD_CUSTOM_CALIB_ZERO, //标定零点
    CMD_CUSTOM_CALIB_WGT, //标定重量
    CMD_CUSTOM_WRITE_PARAM, //参数写 传感器个数/阀值/读取时间
    CMD_CUSTOM_READ_PARAM, //参数读 传感器个数/阀值/读取时间
    CMD_CUSTOM_WRITE_SENSOR_ADDR, //传感器地址设置，只写

};
union SensorState{
    struct {
        quint32 zero:1;
        quint32 still:1;
        quint32 overflow:1;
        quint32 power_zero_overflow:1;
        quint32 sensor_err:1;
        quint32 soc_err:1;
        quint32 bit6:1;
        quint32 bit7:1;

    }StateBit;
    quint8 StateValue;
};

struct SensorErrInfo{
    quint8 addr;
    SensorState state;

};
typedef QVector<SensorErrInfo> SensorErrInfoList;

struct SensorWgtInfo{
    quint8 addr;
    SensorState state;
    qint16 wgt;
};
typedef QVector<SensorWgtInfo> SensorWgtInfoList;

#endif // COMMAND_H

