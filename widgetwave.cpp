#include "widgetwave.h"
#include "ui_widgetwave.h"

WidgetWave::WidgetWave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetWave)
{
    ui->setupUi(this);
    wave = new WaveWidget(ui->widget,4);
    wave->SetChannel(0,4);

}

WidgetWave::~WidgetWave()
{
    delete ui;
}
void WidgetWave::DisplayWeight(MsgSensorData &data)
{
    for(int i = 0; i < data.channels.size(); i++)
    {
        int dot = 0;
        //压强是1个小数点
        if(data.channels[i].addr != 4){
            dot = 1;
        }
        double wf = data.channels[i].weight;
        float v = utils::int2float(wf,dot);
        wave->AppendData(i,v);
    }

    wave->DisplayAllChannel(true);



}

void WidgetWave::ShowMe(bool en)
{
   if(en){
       this->show();
   }else{
       this->hide();
   }
}
