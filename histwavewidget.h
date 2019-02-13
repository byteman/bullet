#ifndef HISTWAVEWIDGET_H
#define HISTWAVEWIDGET_H

#include <QObject>
#include "qcustomplot.h"
#include "models.h"
#include <QSqlError>
#include "dao.h"


class HistWaveWidget:public QObject
{
public:
    enum TracerType
        {
            XAxisTracer,
            YAxisTracer,
            DataTracer
        };
    HistWaveWidget(QWidget *parent);
    //加载波形数据.
    void DisplayData(int chan,DeviceDataList &ddl);
    int  GetKeyIndex(qint64 key);
    void SetTitle(QString title);
    void Clear();
private:
    QMap<qint64,int> m_keys;
    QWidget* m_parent;
    QCustomPlot *m_plot;
    QColor m_color;


    void Init();
    void AddChannel();


};

#endif // HISTWAVEWIDGET_H
