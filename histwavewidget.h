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
    void updatePosition(double xValue, double yValue);
    int  GetKeyIndex(qint64 key);
    void SetTitle(QString title);
    void Clear();
private:
    QMap<qint64,int> m_keys;
    QWidget* m_parent;
    QCustomPlot *m_plot;
    QColor m_color;
    //QCustomPlot *plot;
    QCPItemTracer *tracer;// 跟踪的点
    QCPItemText *label;   // 显示的数值
    QCPItemLine *arrow ;   // 箭头
    bool visible;
    TracerType m_type;

    void Init();
    void AddChannel();
    void InitTracker(TracerType _type);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setLabelPen(const QPen &pen);
    void setText(const QString &text);
    void setVisible(bool visible);

};

#endif // HISTWAVEWIDGET_H
