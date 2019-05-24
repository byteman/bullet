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
    m_rt_wave_s(20*60)
{
    ui->setupUi(this);
    //ui->tbtAlarm->hide();
    //ui->lblChan->setText(tr("Address") + ":" + QString("%1").arg(addr));

    SetOnline(true);
    ui->lbl_weight->setText("");

    if(addr <=0 )addr=1;



}

void ChannelWidget::Show()
{
    if(m_zoom){
       ui->lbl_weight->hide();
    }else{
       ui->lbl_weight->setStyleSheet("font-size : 12pt");
       ui->lbl_weight->show();
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

}

int ChannelWidget::Addr()
{
    return m_addr;
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
    m_paused    = cfg.paused;
    disable(m_paused);
//    QString maxStr = QString(QStringLiteral("上超限:%1")).arg(m_max_value);
//    QString minStr = QString(QStringLiteral("下超限:%1")).arg(m_min_value);
//    ui->lbl_w_max->setText(maxStr);
//    ui->lbl_w_min->setText(minStr);
//    ui->tbtPlay->setChecked(m_paused);

}

//禁用通道的时候，更改通道的颜色.
void ChannelWidget::SetRecState(bool paused)
{

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
void ChannelWidget::AlarmCheck(double weight)
{
   if((weight >= m_max_value) || (weight <= m_min_value))
   {
        //报警
       ui->lbl_weight->setStyleSheet("background-color: rgb(255, 0, 0); ");
   }else{
       ui->lbl_weight->setStyleSheet("background-color: rgb(12, 235, 12); ");
   }
}

void ChannelWidget::DisplayWeight(int weight, quint16 state, quint16 dot)
{

    double wf = (double)weight;

    QString ws = utils::float2string(wf, dot);

    resetTimeout();

    ui->lbl_weight->setText(ws);


    AlarmCheck(utils::int2float(weight,dot));




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

}
void ChannelWidget::MinAlarm(bool alarm)
{

}
void ChannelWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    //qDebug() <<m_addr <<" double click";
//    m_zoom=!m_zoom;
//    if(m_zoom){
//       ui->lbl_weight->setStyleSheet("font-size : 128px");
//    }else{
//       ui->lbl_weight->setStyleSheet("font-size : 16pt");

//    }
//    emit onDoubleClick(m_addr,m_zoom);
}


void ChannelWidget::timerEvent(QTimerEvent *)
{

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
