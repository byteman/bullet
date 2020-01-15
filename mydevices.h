#ifndef MYDEVICES_H
#define MYDEVICES_H

#include <QObject>
#include <QVector>
#include <QGroupBox>
#include <QGridLayout>
#include <QMap>
#include "channelwidget.h"
#include "models.h"
class MyDevices:public QObject
{
    Q_OBJECT
public:
    MyDevices(int max=32,QGroupBox* parent=NULL );
    void SetTimeRange(int rangeS);
    void SetDeviceNum(int  start, int num);
    void SetMaxSampleNum(int max);
    void SetCenterAddr(int addr);
    void Timeout(int addr);
    void SetUnit(QString unit);
    void SetTitle(int addr, QString title);
    void SetChanSetting(int addr,DeviceChnConfig& cfg);
    void SetRecState(int addr, bool paused);
    void DisplayWeight(int addr,int weight, quint16 state,quint16 dot);
    void SaveWave();
    void ClearDisplay();
    //void LoadWave(QString file,ChannelsData& datas);
    void GetNum(int &start,int &num);
    int  GetEndAddr();
    void listWaveFiles(QStringList &files);
    void Resize();
    void DisplayDataQueue(int addr,QQueue<SensorData> &dataQueue);
    int  GetZoomWidget();
    void SetSaveInt(int rangeS);
    void SetOnline(bool online);
    void SetSelectRecState( bool paused);
    void SetSelectAll(bool sel);
    QVector<int> GetSelectChan();
public slots:
    void onDoubleClick(int addr,bool zoom);
    void onChannelConfigEvent(int addr);
    void onPlayClickEvent(int addr, bool played);
signals:
    void onWaveShow(int addr,bool zoom);
    void WaveFull();
    void onChannelConfig(int addr);
    void onPlayClick(int addr, bool played);
private:
    void AppendWave(int addr,float value);
    QVector<ChannelWidget*> widgets;
    void clearAll();
    int m_row,m_col;
    int m_max;
    int m_max_sample;
    QGroupBox* m_container;
    QGridLayout* qlayout;
    QMap<int,QByteArray > m_values;
    bool m_zoom;
    int  m_addr;
    //CSVFile m_csv;
    int m_num;
    int m_start;
    int m_end;
    int m_index;
    QString m_unit;
    QString CreateDir(QString type);
    QString GetFileName();
    void AppendItem(QString &output, QString item);

    void GetFileName(QString &csvFile, QString &waveFile);
    void zoomDevice(int addr);
};

#endif // MYDEVICES_H
