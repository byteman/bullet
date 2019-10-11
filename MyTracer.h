#ifndef MYTRACER_H
#define MYTRACER_H
//#include "MStrTools.h"
#include <QObject>
#include "qcustomplot.h"


class MyTracer : public QObject
{
	Q_OBJECT

public:

	enum TracerType
	{
		XAxisTracer,
		YAxisTracer,
		DataTracer,
        CrossLine,
        YAxisStaticLineUp, //Y轴绝对线.
        YAxisStaticLineDown, //Y轴绝对线.
	};
    explicit MyTracer(QCustomPlot *_plot, TracerType _type, QObject *parent = NULL);
	~MyTracer();
	void setPen(const QPen &pen);
	void setBrush(const QBrush &brush);
	void setText(const QString &text);
	void setLabelPen(const QPen &pen);
    void setChan(const int ch);
	void updatePosition(double xValue, double yValue);
    void displayPosition(double xValue, double yValue);
    void setVisible(bool visible);
    void setXY(double x, double y);
    void getXY(double &x, double &y);
protected:

protected:
    QCustomPlot *plot;
    QCPItemTracer *tracer;// 跟踪的点
    QCPItemText *label;   // 显示的数值
    QCPItemLine *arrow ;   // 箭头
    QCPItemStraightLine *line ; //无限延长的直线
    double _x,_y;
    TracerType type;
    int m_chan;
    bool visible;
};

#endif // MYTRACER_H
