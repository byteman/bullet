#include "ilinechart.h"

void ILineChart::DisplayAllChannel(bool show)
{

    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Display(show);

    }

}


void ILineChart::AppendDataArray(QVector<QVector<double>> &samples)
{
    for(int i = 0; i < channels.size();i++)
    {

        channels[i]->AddDataArray(samples[i]);
    }
}

void ILineChart::AppendData(int chan,float value)
{
    if(chan >= channels.size()) return;
    channels[chan]->AddData(0,value);
}
#if 0
void ILineChart::SetDataArray(QVector<QVector<double>> &samples)
{
    QVector<double>  keys;
    keys.reserve(samples[0].size());
    for(int j =0; j < samples[0].size();j++)
    {
        keys.push_back(keys.size()+1);
    }
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Clear();
        channels[i]->SetDataArray(keys,samples[i]);
    }

}
#else
void ILineChart::SetDataArray(QVector<QVector<double>> &samples)
{
    for(int i = 0; i < channels.size();i++)
    {
        double min,max;
        channels[i]->SetDataArray(samples[i]);
        channels[i]->GetValueRange(min,max);
        if(i == 0){
            m_max = max;
            m_min = min;
        }
        if(min < m_min){
            m_min = min;
        }
        if(max > m_max){
            m_max = max;
        }

    }

}

void ILineChart::SetChanDataArray(int chan, QVector<double> &samples)
{
    if(chan >= channels.size()) return;
    channels[chan]->SetDataArray(samples);
}
#endif
void ILineChart::InitChannel(QWidget *parent, int num)
{
    //Qt::white	3	White (#ffffff)
    //Qt::black	2	Black (#000000)
    //Qt::red	7	Red (#ff0000)
    //Qt::darkRed	13	Dark red (#800000)
    //Qt::green	8	Green (#00ff00)
    //Qt::darkGreen	14	Dark green (#008000)
    //Qt::blue	9	Blue (#0000ff)
    //Qt::darkBlue	15	Dark blue (#000080)
    //Qt::cyan	10	Cyan (#00ffff)
    //Qt::darkCyan	16	Dark cyan (#008080)

}



void ILineChart::DisplayChannel(int chan,bool bshow)
{
    int index = (chan >= channels.size())?0:chan;


    channels[index]->Display(bshow);


}
void ILineChart::CloseAll()
{
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Display(false);
    }
}

void ILineChart::Clear()
{
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Clear();
    }
}

void ILineChart::SetChannel(int start, int num)
{

}

void ILineChart::GetValueRange(double &min, double &max)
{
    min = m_min;
    max = m_max;
}

void ILineChart::SetFilter(IValueFilter *filter)
{
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->SetFilter(filter);
    }
}

double ILineChart::filter(double value)
{
    return value;
}
