#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../protomessage.h"
#include <QThread>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serial_id(0),
    session_id(0)
{
    ui->setupUi(this);
    this->startTimer(5000);
    socket.bind(9999);
    connect(&socket,SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));
    timer.setInterval(50);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));
    qDebug() << sizeof(MsgDevicePara);

}
void MainWindow::addLog(QString str)
{
    ui->txtLog->append(str);
}
void MainWindow::parseMsg(QByteArray& msg,QHostAddress sender,quint16 senderPort)
{
    ProtoMessage input_msg;

    if(parser.parseData(msg, input_msg))
    {
        if(input_msg.head.cmd_id == MSG_START_WAVE)
        {
            on_btnStart_clicked();
        }
        else if(input_msg.head.cmd_id == MSG_HEART)
        {
            sDateTime sdt;
            input_msg.getDateTime(sdt);
            addLog(sdt.toString());

        }
        else if(input_msg.head.cmd_id == MSG_READ_PARAM)
        {
            MsgDevicePara para;

            QByteArray data;
            para.toByteArray(data);
            addLog("------send param");
            sendPacket(0x80|MSG_READ_PARAM,session_id,data);
            //socket.writeDatagram(msg.toByteArray(),sender,senderPort);

        }
        else if(input_msg.head.cmd_id == MSG_WRITE_PARAM)
        {
            MsgDevicePara para;
            if(input_msg.getData(&para,sizeof(MsgDevicePara)))
            {

                addLog(QString("workMode=%1").arg(para.mWorkMode));
                addLog(QString("mDateTime=%1").arg(para.mDateTime.toString()));
                addLog(QString("mWetDown=%1").arg(para.mWetDown));
                addLog(QString("mWetUp=%1").arg(para.mWetUp));
                addLog(QString("GateWay=%1").arg(para.Local_IP.GateWay.toString()));
                addLog(QString("ipaddr=%1").arg(para.Local_IP.ipaddr.toString()));
                addLog(QString("SubnetMask=%1").arg(para.Local_IP.SubnetMask.toString()));
                addLog(QString("mWifiPass=%1").arg((char*)para.mWifiPass));
                addLog(QString("mWifiSSID=%1").arg((char*)para.mWifiSSID));
                addLog(QString("Server_ip.ipaddr=%1").arg(para.Server_ip.ipaddr.toString()));
                addLog(QString("Server_ip.port=%1").arg(para.Server_ip.port));


            }
            else
            {
                addLog("write param failed!");
            }

        }
        else if(input_msg.head.cmd_id == MSG_ENUM_FILES)
        {
            enumFiles();
        }
    }
}
#include <QDir>
void MainWindow::enumFiles()
{

    QStringList filist2 = QDir::current().entryList();
    QString files;
    files = filist2.join(",");

    quint16 num = filist2.size();
    QByteArray data;
    data.append((const char*)&num,2);
    for(int i = 0; i < num; i++)
    {
        data.append(1);
    }
    data.append(files);
    sendPacket(0x80|MSG_ENUM_FILES,session_id,data);

}
void MainWindow::readPendingDatagrams()
{
    while (socket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket.readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        parseMsg(datagram, sender, senderPort);

    }
}
#include <QMessageBox>
void MainWindow::timeout()
{
    if(m_index < m_count)
    {
        sendOnePacket(m_sess_id,m_index++,m_value++);
    }else{
        timer.stop();
        QMessageBox::information(this,"title","send ok");
    }

}
MainWindow::~MainWindow()
{
    delete ui;
}
/*
    quint8 samplebits;
    quint8 nchannel;
    quint32 totalSamples;
    quint32 startSample;
    quint16 nSample;
*/
void MainWindow::sendOnePacket(quint16 ss,int index,qint16 value)
{
    WaveDataHead wvd;

    wvd.samplebits = 16;
    wvd.nchannel = 8;
    wvd.totalSamples = m_num*m_count;
    wvd.startSample = 0;
    wvd.nSample = 1;


    //n次发送

    QByteArray data;
    wvd.startSample = index*m_num;
    wvd.nSample = m_num;
    data.append(wvd.samplebits);
    data.append(wvd.nchannel);
    data.append((char*)&wvd.totalSamples,4);
    data.append((char*)&wvd.startSample,4);
    data.append((char*)&wvd.nSample,2);
    //n个采样
    for(int j = 0; j < m_num; j++)
    {
        //一次采样n个通道
        for(int k =0; k < wvd.nchannel; k++)
        {
            data.append((char*)&value,2);
        }


    }
    sendPacket(MSG_WAVE_DATA,ss,data);



}
void MainWindow::on_btnStart_clicked()
{
    m_num = ui->edtSampleNum->text().toInt();
    m_count = ui->edtCount->text().toInt();
    m_value = 0;
    m_index = 0;
    m_sess_id = session_id++;
    timer.start();

}

void MainWindow::sendPacket(quint8 cmd,quint16 sess,QByteArray& data)
{
    ProtoMessage msg;
    msg.head.cmd_id = cmd;
    msg.head.device_id = ui->edtDevID->text().toInt();
    msg.head.serial_id = serial_id++;
    msg.head.sesson_id = sess;
    msg.data.append(data);
    QByteArray buf;
    msg.toByteArray(buf);
    qDebug() << "send size="  << buf.size();
    socket.writeDatagram(buf,QHostAddress(ui->edtIp->text()),ui->edtPort->text().toInt());
}
void MainWindow::timerEvent(QTimerEvent *)
{
    QByteArray data;
    sendPacket(MSG_HEART,session_id++,data);
}

void MainWindow::on_btnStop_clicked()
{
    qDebug() << "stop";
}
