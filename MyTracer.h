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
		CrossLine
	};
	explicit MyTracer(QCustomPlot *_plot, TracerType _type, QObject *parent = nullptr);
	~MyTracer();
	void setPen(const QPen &pen);
	void setBrush(const QBrush &brush);
	void setText(const QString &text);
	void setLabelPen(const QPen &pen);
	void updatePosition(double xValue, double yValue);

protected:
	void setVisible(bool visible);
protected:
    QCustomPlot *plot;
    QCPItemTracer *tracer;// 跟踪的点
    QCPItemText *label;   // 显示的数值
    QCPItemLine *arrow ;   // 箭头
    QCPItemStraightLine *line ; //无限延长的直线

    TracerType type;
    bool visible;
};

#endif // MYTRACER_H
