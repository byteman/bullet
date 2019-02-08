#include "dialogparams.h"
#include "ui_dialogparams.h"

DialogParams::DialogParams(DeviceManager* _dvm,QString id,QWidget *parent) :
    QDialog(parent),
    dvm(_dvm),
    currId(id),
    ui(new Ui::DialogParams)
{
    ui->setupUi(this);
    connect(dvm,SIGNAL(ReadParam(Device*,MsgDevicePara)),this,SLOT(onReadPara(Device*,MsgDevicePara)));
    connect(dvm,SIGNAL(WriteParam(Device*,bool)),this,SLOT(onWritePara(Device*,bool)));

    dvm->ReadParam(id);
    ui->edtDevId->setText(id);
}

DialogParams::~DialogParams()
{
    disconnect(dvm,SIGNAL(ReadParam(Device*,MsgDevicePara)),this,SLOT(onReadPara(Device*,MsgDevicePara)));
    disconnect(dvm,SIGNAL(WriteParam(Device*,bool)),this,SLOT(onWritePara(Device*,bool)));
    //disconnect(dvm,SIGNAL(ResetResult(Device*,bool)),this,SLOT(onResetResult(Device*,bool)));
    delete ui;
}
void DialogParams::onReadPara(Device* dev,MsgDevicePara para)
{
    //char buf[16]={0,};
    //memcpy(buf,para.SerialNo,12);

    ui->edtDevId->setText(dev->id());
    ui->edtTime->setText(para.mDateTime.toString());
    ui->edtDevIp->setText(para.Local_IP.ipaddr.toString());
    ui->edtNetmask->setText(para.Local_IP.SubnetMask.toString());
    ui->edtGateway->setText(para.Local_IP.GateWay.toString());
    ui->edtServerIp->setText(para.Server_ip.ipaddr.toString());
    ui->edtServerPort->setText(QString("%1").arg(para.Server_ip.port));
    ui->edtDelay->setText(QString("%1").arg(para.mReadInt));
    ui->edtTimeOut->setText(QString("%1").arg(para.mTimeout));
    char* p  = (char*)para.Version;
    ui->edtVer->setText(p);

}

void DialogParams::toIpAddr(QString ipstr, sIP_ADDR& addr)
{
    QHostAddress hostAddr(ipstr);
    quint32 ipaddr = hostAddr.toIPv4Address();
    addr.fromIp(ipaddr);

}
void DialogParams::toString(QString str, INT8U* dest,int size)
{
    std::string s = str.toStdString();
    for(int i = 0; i < s.size(); i++)
    {
        dest[i] = s[i];

    }
}

void DialogParams::toInt16U(QString str, INT16U &dest)
{
dest = str.toInt();
}

void DialogParams::toInt8U(QString str, INT8U &dest)
{
dest = str.toInt();
}

void DialogParams::toInt32U(QString str, quint32 &dest)
{
    dest = str.toInt();
}

void DialogParams::writeParam()
{
    MsgDevicePara para;
    toIpAddr(ui->edtGateway->text(), para.Local_IP.GateWay);
    toIpAddr(ui->edtNetmask->text(), para.Local_IP.SubnetMask);
    toIpAddr(ui->edtDevIp->text(),   para.Local_IP.ipaddr);
    toIpAddr(ui->edtServerIp->text(), para.Server_ip.ipaddr);
    toInt32U(ui->edtServerPort->text(),para.Server_ip.port);
    toInt16U(ui->edtDelay->text(),para.mReadInt);
    toInt16U(ui->edtTimeOut->text(),para.mTimeout);

    dvm->WriteParam(currId,para);
}
#include "myhelper.h"
void DialogParams::onWritePara(Device*,bool)
{
     myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("写入成功"));

}

void DialogParams::on_buttonBox_accepted()
{

}

void DialogParams::on_btnSave_clicked()
{
    writeParam();
}

void DialogParams::on_btnExit_clicked()
{
    this->reject();
}

void DialogParams::on_btnRead_clicked()
{
    dvm->ReadParam(currId);
}
