#include "channelwidget.h"
#include "ui_channelwidget.h"
#include "utils.h"
#include "command.h"

#define MAX_TIMEOUT 3
ChannelWidget::ChannelWidget(int addr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelWidget),
    m_addr(addr),
    m_timeout(MAX_TIMEOUT),
    m_last_still(false)
{
    ui->setupUi(this);
    ui->lblChan->setText(tr("Address") + ":" + QString("%1").arg(addr));
    clearState();
    SetOnline(true);
    ui->lbl_weight->setText("");

    m_waveWdg = new WaveWidget(ui->plot2,1);

    m_waveWdg->Hide();
}

void ChannelWidget::Show()
{
    if(m_zoom){

       ui->lbl_weight->setStyleSheet("font-size : 128px");
       ui->lbl_weight->hide();
       m_waveWdg->Show();
    }else{
       ui->lbl_weight->setStyleSheet("font-size : 16pt");
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

void ChannelWidget::Timeout()
{
    if(m_timeout > 0)
    {
        m_timeout--;
        SetOnline(true);
    }
    if(m_timeout<=0){
        clearState();
        ui->lbl_weight->setText("");
        SetOnline(false);
    }
}

void ChannelWidget::SetUnit(QString unit)
{
    m_unit = unit;
    //ui->lbl_unit->setText(unit);
}

void ChannelWidget::clearState()
{

}
void ChannelWidget::resetTimeout()
{
    m_timeout = MAX_TIMEOUT;
}

QString ChannelWidget::DisplayWeight(int weight, quint16 state, quint16 dot)
{
    double wf = (double)weight;

    QString ws = utils::float2string(wf, dot);
    QString wt = ws;
    resetTimeout();

    ui->lbl_weight->setText(ws);
    m_waveWdg->AppendData(0,utils::int2float(wf,dot));
    return wt;

}

void ChannelWidget::SetOnline(bool online)
{
    if(online){
        ui->lblChan->setStyleSheet("background-color: green;");
    }else{
        ui->lblChan->setStyleSheet("background-color: red;");
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
