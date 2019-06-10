#include "channelwidget.h"
#include "ui_channelwidget.h"
#include "utils.h"
#include "command.h"
#include "dao.h"
#define MAX_TIMEOUT 3
ChannelWidget::ChannelWidget(int addr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelWidget),
    m_addr(addr),
    m_timeout(MAX_TIMEOUT),
    m_zoom(false),
    m_paused(false),
    m_waveWdg(NULL),
    m_rt_wave_s(20*60)
{
    ui->setupUi(this);
    ui->tbtAlarm->hide();
    //ui->lblChan->setText(tr("Address") + ":" + QString("%1").arg(addr));

    SetOnline(true);
    ui->lbl_weight->setText("");

    m_waveWdg = new WaveWidget(ui->plot2,1);
    if(addr <=0 )addr=1;

    m_waveWdg->SetChannel(addr-1,1);
    m_waveWdg->Hide();

}

void ChannelWidget::Show()
{
    if(m_zoom){

       //ui->lbl_weight->setStyleSheet("font-size : 128px");
       ui->lbl_weight->hide();
       m_waveWdg->Show();
    }else{
       ui->lbl_weight->setStyleSheet("font-size : 40pt");
       ui->lbl_weight->show();
       m_waveWdg->Hide();

    }
    this->show();
}
void ChannelWidget::Reset()
{
    m_zoom = false;
}

bool ChannelWidget::IsZoom()
{
    return m_zoom;
}

void ChannelWidget::ClearDisplay()
{
    ui->lbl_weight->setText("");
    if(m_waveWdg!=NULL)
        m_waveWdg->Clear();
    MinAlarm(false);
    MaxAlarm(false);
}

int ChannelWidget::Addr()
{
    return m_addr;
}

QQueue<SensorData> &ChannelWidget::GetHistoryData()
{
    return m_histData;
}
//重新加载一组波形数据.
void ChannelWidget::DisplayDataQueue(QQueue<SensorData> &dataQueue)
{
    m_waveWdg->Clear();
    double t = 0;
    for(int i = 0; i < dataQueue.size(); i++)
    {
        if(i == 0){
            t = dataQueue[i].time;
        }
        double df = double(dataQueue[i].time - t)/1000.0f;

        m_waveWdg->AppendTimeData(m_addr,df, dataQueue[i].weight);
    }

}
ChannelWidget::~ChannelWidget()
{
    delete ui;
}

void ChannelWidget::SetTitle(QString name)
{
    ui->lblChan->setText(name);

    m_name = name;

}

void ChannelWidget::SetChanSetting(DeviceChnConfig &cfg)
{
    m_min_value = cfg.minValue;
    m_max_value = cfg.maxValue;
    m_paused    = cfg.paused?true:false;
    disable(m_paused);
    QString maxStr = QString(QStringLiteral("上超限:%1")).arg(m_max_value);
    QString minStr = QString(QStringLiteral("下超限:%1")).arg(m_min_value);
    ui->lbl_w_max->setText(maxStr);
    ui->lbl_w_min->setText(minStr);
    ui->tbtPlay->setChecked(m_paused);
    m_waveWdg->AddLine("max",maxStr,false, 1000,m_max_value);
    m_waveWdg->AddLine("min",minStr,true,1000,m_min_value);
    if(m_paused)
        ui->lbl_weight->setText(QStringLiteral("暂停"));
}

//禁用通道的时候，更改通道的颜色.
void ChannelWidget::SetRecState(bool paused)
{
    m_paused = paused;

    disable(paused);

    ui->tbtPlay->setChecked(paused);
    if(paused)
    ui->lbl_weight->setText(QStringLiteral("暂停"));
}
//void ChannelWidget::SetSaveInt(int rangeS)
//{
//    m_waveWdg->SetRange(rangeS);
//}
void ChannelWidget::SetTimeRange(int rangeS)
{
    m_waveWdg->SetRange(rangeS);
}


void ChannelWidget::Timeout()
{
    if(m_timeout > 0)
    {
        m_timeout--;
        SetOnline(true);
    }
    if(m_timeout<=0){
        ui->lbl_weight->setText("");
        SetOnline(false);
    }
}

void ChannelWidget::SetUnit(QString unit)
{
    m_unit = unit;
    //ui->lbl_unit->setText(unit);
}

void ChannelWidget::resetTimeout()
{
    m_timeout = MAX_TIMEOUT;
}
void ChannelWidget::AlarmCheck(int weight)
{
    MinAlarm(weight<m_min_value);
    MaxAlarm(weight>m_max_value);
}
void ChannelWidget::WriteValues(int &value)
{
    SensorData data;
    data.time = QDateTime::currentMSecsSinceEpoch() / 1000;
    data.weight = value;
    data.addr = this->m_addr;
    qint32 span = m_rt_wave_s;

    m_histData.push_back(data);
    do{
        SensorData& first = m_histData.front();
        //如果超过20分钟*60秒，也抛弃.最多一秒一条数据.
        if( (data.time - first.time) < span && m_histData.size() < span){
            //如果当前数据的时间没有大于第一个数据20min种，就说明数据还没有满,跳过,否则
            break;
        }
        m_histData.pop_front();
    }while(1);

}
void ChannelWidget::DisplayWeight(int weight, quint16 state, quint16 dot)
{
    if(m_paused)
    {
        return;
    }
    if(weight==65535){
        ui->lbl_weight->setText(QStringLiteral("未连接"));
    }else{
        double wf = (double)weight;

        QString ws = utils::float2string(wf, dot);
        if(ws.length() >= 6){
            //qDebug() << "----------0";
            ui->lbl_weight->setText(QStringLiteral("数据异常"));
        }else{
           ui->lbl_weight->setText(ws+m_unit);
        }

        if(m_zoom){
            m_waveWdg->AppendTimeData(m_addr-1,utils::int2float(wf,dot));
            m_waveWdg->DisplayAllChannel(true);
        }
        if(!m_paused){
          AlarmCheck(weight);
        }
    }

    resetTimeout();






}
void ChannelWidget::disable(bool dis)
{
    if(dis){

        ui->lblChan->setStyleSheet("background-color: red;");
    }
    else{
        ui->lblChan->setStyleSheet("background-color: #26A65B;");
    }
}
void ChannelWidget::SetOnline(bool online)
{
    if(online){

        // this->setStyleSheet("background-color: #26A65B;");
        ui->lblChan->setStyleSheet("background-color: #26A65B;");

    }else{
        // this->setStyleSheet("background-color: red;");
        ui->lblChan->setStyleSheet("background-color: red;");
        ui->lbl_weight->setText(QStringLiteral("  通讯故障"));
    }
}

void ChannelWidget::MaxAlarm(bool alarm)
{
    if(alarm){
        ui->lbl_w_max->setStyleSheet("background-color: rgb(255, 0, 0);");
    }else{
        ui->lbl_w_max->setStyleSheet("background-color: rgb(0, 170, 0);");
    }
}
void ChannelWidget::MinAlarm(bool alarm)
{
    if(alarm){
        ui->lbl_w_min->setStyleSheet("background-color: rgb(255, 0, 0);");
    }else{
        ui->lbl_w_min->setStyleSheet("background-color: rgb(0, 170, 0);");
    }
}
void ChannelWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    //qDebug() <<m_addr <<" double click";
    m_zoom=!m_zoom;
    if(m_zoom){
       ui->lbl_weight->setStyleSheet("font-size : 128px");
    }else{
       ui->lbl_weight->setStyleSheet("font-size : 16pt");

    }
    emit onDoubleClick(m_addr,m_zoom);
}


void ChannelWidget::timerEvent(QTimerEvent *)
{
    static int i = 0;
    qDebug() << "time i=" << i;
    m_waveWdg->AppendData(0,i++);
    //m_waveWdg->DisplayChannel(0,true);
    //wave->AppendData(0,get_random_number());
    m_waveWdg->DisplayChannel(0,true);

    //DisplayWeight(i++,0,0);
}

void ChannelWidget::on_tbtSet_clicked()
{
    qDebug() << "addr " << m_addr << " config clicked";
    emit onConfigClick(this->m_addr);
}

void ChannelWidget::on_tbtPlay_clicked(bool checked)
{
    qDebug() << "play=" << checked;
    emit onPlayClick(m_addr,checked);
}
