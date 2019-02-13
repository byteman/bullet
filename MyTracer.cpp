#include "MyTracer.h"
#include "utils.h"
MyTracer::MyTracer(QCustomPlot *_plot, TracerType _type, QObject *parent) : plot(_plot),
type(_type),visible(false), QObject(parent)
{


    tracer = NULL;// 跟踪的点
    label = NULL;   // 显示的数值
    arrow = NULL ;   // 箭头
    line = NULL; //无限延长的直线
	if (plot)
	{
		tracer = new QCPItemTracer(plot);
		tracer->setStyle(QCPItemTracer::tsCircle);
        tracer->setPen(QPen(QColor(235, 100, 0),5));
		tracer->setBrush(QColor(235, 100, 0));


		label = new QCPItemText(plot);
		label->setLayer("overlay");
		label->setClipToAxisRect(false);
		label->setPadding(QMargins(5, 5, 5, 5));
		label->setBrush(QBrush(QColor(0, 0, 0, 200)));
        label->setPen(QPen(Qt::green));
		label->position->setParentAnchor(tracer->position);
		label->setFont(QFont("宋体", 10));
		label->setColor(Qt::white);

		arrow = new QCPItemLine(plot);
        QPen  arrowPen(Qt::yellow, 1);
		arrow->setPen(arrowPen);
		arrow->setLayer("overlay");
		arrow->setClipToAxisRect(false);
		arrow->setHead(QCPLineEnding::esSpikeArrow);//设置头部为箭头形状

		line = new QCPItemStraightLine(plot);//直线
		line->setLayer("overlay");
        QPen linesPen(QColor(255, 71, 90), 1, Qt::DashLine);
		label->setPen(linesPen);
		line->setClipToAxisRect(true);
        line->setPen(QPen(Qt::red));


		switch (type) {
		case XAxisTracer:
		{
			tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
			tracer->position->setTypeY(QCPItemPosition::ptAxisRectRatio);
			tracer->setSize(7);
			label->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);

			arrow->end->setParentAnchor(tracer->position);
			arrow->start->setParentAnchor(arrow->end);
			arrow->start->setCoords(20, 0);//偏移量
			break;
		}
		case YAxisTracer:
		{
			tracer->position->setTypeX(QCPItemPosition::ptAxisRectRatio);
			tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);
			tracer->setSize(7);
			label->setPositionAlignment(Qt::AlignRight | Qt::AlignHCenter);

			arrow->end->setParentAnchor(tracer->position);
			arrow->start->setParentAnchor(label->position);
			arrow->start->setCoords(-20, 0);//偏移量
			break;
		}
		case DataTracer:
		{
			tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
			tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);
			tracer->setSize(5);

			label->setPositionAlignment(Qt::AlignLeft | Qt::AlignVCenter);

			arrow->end->setParentAnchor(tracer->position);
			arrow->start->setParentAnchor(arrow->end);
			arrow->start->setCoords(25, 0);
			break;
		}
		case CrossLine:
		{

			line->point1->setTypeX(QCPItemPosition::ptPlotCoords);
			line->point1->setTypeY(QCPItemPosition::ptPlotCoords);
			line->point2->setTypeX(QCPItemPosition::ptPlotCoords);
			line->point2->setTypeY(QCPItemPosition::ptPlotCoords);


			break;
		}
        case YAxisStaticLineUp:
        {

            line->setPen(QPen(Qt::red,1, Qt::DashLine));
            tracer->position->setTypeX(QCPItemPosition::ptAxisRectRatio);
            tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);
            tracer->setSize(7);
            label->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);

            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(arrow->end);
            arrow->start->setCoords(20, 0);//偏移量


            break;
        }
        case YAxisStaticLineDown:
        {

            line->setPen(QPen(Qt::red,1, Qt::DashLine));
            tracer->position->setTypeX(QCPItemPosition::ptAxisRectRatio);
            tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);
            tracer->setSize(7);
            label->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);

            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(arrow->end);
            arrow->start->setCoords(20, 0);//偏移量



            break;
        }
		default:
			break;
		}

		setVisible(false);
	}
}

MyTracer::~MyTracer()
{
	if (tracer)
		plot->removeItem(tracer);
	if (label)
		plot->removeItem(label);
}

void MyTracer::setPen(const QPen &pen)
{
	tracer->setPen(pen);
	arrow->setPen(pen);
}

void MyTracer::setBrush(const QBrush &brush)
{
	tracer->setBrush(brush);
}

void MyTracer::setLabelPen(const QPen &pen)
{
	label->setPen(pen);
}

void MyTracer::setText(const QString &text)
{
	label->setText(text);
}

void MyTracer::setVisible(bool visible)
{
	tracer->setVisible(visible);
	label->setVisible(visible);
	arrow->setVisible(visible);
    line->setVisible(visible);
}

void MyTracer::setXY(double x, double y)
{
   this->_x = x;
    this->_y = y;
}

void MyTracer::getXY(double &x, double &y)
{
    x = this->_x;
    y = this->_y;
}

void MyTracer::updatePosition(double xValue, double yValue)
{
	if (!visible)
	{
		setVisible(true);
		visible = true;
	}
	if (yValue > plot->yAxis->range().upper)
		yValue = plot->yAxis->range().upper;
	switch (type) {
	case XAxisTracer:
	{
		tracer->position->setCoords(xValue, 1);
		label->position->setCoords(0, 15);
		arrow->start->setCoords(0, 15);
		arrow->end->setCoords(0, 0);
        setText(utils::Time2String(xValue));
		break;
	}
	case YAxisTracer:
	{
		tracer->position->setCoords(0, yValue);
		label->position->setCoords(-60, 0);
		//arrow->start->setCoords(-20, 0);
		//arrow->end->setCoords(0, 0);
		setText(QString::number(yValue, 'f', 1));
		break;
	}
	case DataTracer:
	{
		tracer->position->setCoords(xValue, yValue);
		label->position->setCoords(25, 0);
		//setText(toQString(MStrTools::Time2String(xValue)) + "\n "+ QString::number(yValue, 'f', 1));
		setText( QString::number(yValue, 'f', 1));
		break;
	}
	case CrossLine:
	{
        line->point1->setCoords(xValue, yValue);
        line->point2->setCoords(xValue, yValue - 100.0);

		break;
	}
    case YAxisStaticLineUp:
    {
        tracer->position->setCoords(0.1, yValue);
        label->position->setCoords(0, 15);
        arrow->start->setCoords(0, 15);
        arrow->end->setCoords(0, 0);
        line->point1->setCoords(0, yValue);
        line->point2->setCoords(xValue, yValue);
        break;
    }
    case YAxisStaticLineDown:
    {
        tracer->position->setCoords(0.9, yValue);
        label->position->setCoords(0, -35);
        arrow->start->setCoords(0, -15);
        arrow->end->setCoords(0, 0);
        line->point1->setCoords(0, yValue);
        line->point2->setCoords(xValue, yValue);

        break;
    }

	default:
		break;
    }
}

void MyTracer::displayPosition(double xValue, double yValue)
{
//    if (!visible)
//    {
//        line->setVisible(visible);
//        visible = true;
//    }
//    switch (type) {
//        case YAxisStaticLine:
//        {
//            line->point1->setCoords(0, yValue);
//            line->point2->setCoords(xValue, yValue);

//            break;
//        }
//    }
}
