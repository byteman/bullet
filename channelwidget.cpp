﻿#include "channelwidget.h"
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
    m_zoom(false)
{
    ui->setupUi(this);
    ui->lblChan->setText(tr("Address") + ":" + QString("%1").arg(addr));

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
       ui->lbl_weight->setStyleSheet("font-size : 48pt");
       ui->lbl_weight->show();
       m_waveWdg->Hide();

    }
    this->show();
}
void ChannelWidget::Reset()
{
    m_zoom = false;
}
ChannelWidget::~ChannelWidget()
{
    delete ui;
}

void ChannelWidget::SetChanSetting(DeviceChnConfig &cfg)
{
    m_min_value = cfg.minValue;
    m_max_value = cfg.maxValue;
    m_paused    = cfg.paused;
    ui->lbl_w_max->setText(QString(QStringLiteral("上超限:%1")).arg(m_min_value));
    ui->lbl_w_min->setText(QString(QStringLiteral("下超限:%1")).arg(m_max_value));
    ui->tbtPlay->setChecked(m_paused);
}


void ChannelWidget::SetRecState(bool paused)
{
    m_paused = paused;
    ui->tbtPlay->setChecked(paused);
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
void ChannelWidget::DisplayWeight(int weight, quint16 state, quint16 dot)
{
    double wf = (double)weight;

    QString ws = utils::float2string(wf, dot);
    QString wt = ws;
    resetTimeout();

    ui->lbl_weight->setText(ws);
    if(m_zoom){
        m_waveWdg->AppendTimeData(m_addr-1,utils::int2float(wf,dot));
        m_waveWdg->DisplayAllChannel(true);
    }
    AlarmCheck(weight);


}

void ChannelWidget::SetOnline(bool online)
{
    if(online){
        ui->lblChan->setStyleSheet("background-color: #26A65B;");
    }else{
        ui->lblChan->setStyleSheet("background-color: red;");
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
