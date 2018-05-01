#include "gpserver.h"

#include "qcustomplot.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>


void MainWindow::loadDeviceUI()
{

    pause = false;
    QList<Device*> devices;
    ui->treeWidget->clear();
    ui->treeWidget->setIconSize(QSize(48,48));
    dvm.ListDevice(devices);
    for(int i = 0; i < devices.size();i++)
    {
        QTreeWidgetItem* item = NULL;
        item = new QTreeWidgetItem(QStringList(devices[i]->name()));

        item->setText(1,"100%");
        ui->treeWidget->addTopLevelItem(item);

        item->setIcon(0,icon_device[1]);

        item->setData(0,Qt::UserRole,devices[i]->id());
        if(i == 0){
            ui->treeWidget->setCurrentItem(item);
            m_cur_dev_id = devices[i]->id();
        }
    }

}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_cur_dev_id = 0;
    qRegisterMetaType<SessMessage>("SessMessage");
    ui->setupUi(this);

    QSettings config("bullet.ini", QSettings::IniFormat);


    m_debug_bytes = config.value("/device/debug",20).toInt();
    m_refresh_count = config.value("/device/refresh",20).toInt();

    icon_device[0].addFile(":image/online.png");
    icon_device[1].addFile(":image/offline.png");
    icon_channel.addFile(":image/channel.png");
    icon_dir.addFile(":image/dir.png");
    icon_file.addFile(":image/channel.png");

    ui->btnStop->setEnabled(false);


    checkAll(true);
    ui->rball->setChecked(true);
    //srv->setParent(this);
//    connect(&srv,SIGNAL(Message(SessMessage)),this,SLOT(Message(SessMessage)));
//    connect(&srv,SIGNAL(Message(SessMessage)),&dvm
//            ,SLOT(Message(SessMessage)));

    srv = new GPServer();
    connect(srv,SIGNAL(Message(SessMessage)),this,SLOT(Message(SessMessage)));
    connect(srv,SIGNAL(Message(SessMessage)),&dvm
            ,SLOT(Message(SessMessage)));

    connect(&dvm,SIGNAL(Notify(QString)),this,SLOT(onNotify(QString)));
    connect(&dvm,SIGNAL(DevOffline(Device*)),this,SLOT(DevOffline(Device*)));
    connect(&dvm,SIGNAL(DevOnline(Device*)),this,SLOT(DevOnline(Device*)));
    connect(&dvm,SIGNAL(ReadParam(Device*,MsgDevicePara)),this,SLOT(onReadPara(Device*,MsgDevicePara)));
    connect(&dvm,SIGNAL(WriteParam(Device*,bool)),this,SLOT(onWritePara(Device*,bool)));
    connect(&dvm,SIGNAL(EnumFiles(Device*,MsgFileList)),this,SLOT(onEnumFiles(Device*,MsgFileList)));
    connect(&dvm,SIGNAL(Progress(Device*,QString)),this,SLOT(onWaveProgress(Device*,QString)));
    connect(&dvm,SIGNAL(WaveMsg(Device*,MsgWaveData)),this,SLOT(onWaveMsg(Device*,MsgWaveData)));
    connect(&dvm,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCalibResult(Device*,int,int,int)));
    connect(&dvm,SIGNAL(RealTimeResult(Device*,RT_AD_RESULT)),this,SLOT(onRealTimeResult(Device*,RT_AD_RESULT)));

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(on_mytime_out()));
    ui->treeWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    menu=new QMenu(this);
    QAction* action = new QAction("读取参数",this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_menu_click(bool)));

    action = new QAction("写入参数",this);
    menu->addAction(action);

    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_write_menu_click(bool)));

    action = new QAction("复位设备",this);
    menu->addAction(action);

    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_reset_menu_click(bool)));
    action = new QAction("枚举设备文件",this);
    menu->addAction(action);

    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_list_files_menu_click(bool)));
    action = new QAction("同步设备文件",this);
    menu->addAction(action);

    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_download_wave(bool)));


    dvm.start();
    dvm.SetStation("工位1");
    loadDeviceUI();

    m_waveWdg = new WaveWidget(ui->plot,8);

    this->startTimer(1000);

    qDebug() <<"MainWindow thread id=" << thread();
}



MainWindow::~MainWindow()
{
    srv->stop();
    delete ui;
}

QString MainWindow::FormatHex(QByteArray& data)
{
    return data.toHex();
}
void MainWindow::Message(SessMessage s)
{
    if(pause)return;
    if(s.getData().size() <   m_debug_bytes)
        ui->txtLog->append(QString("recv-> %1:%2 ").arg(s.getHost().toString()).arg(s.getPort())+FormatHex(s.getData()));
}

void MainWindow::onEnumFiles(Device *dev, MsgFileList files)
{
    ui->listFile->clear();
    for(int i =0; i < files.size();i++)
    {
        QListWidgetItem* item = NULL;
        if(files[i].attr == 1)
            item = new QListWidgetItem(icon_file,files[i].name);
        else
            item = new QListWidgetItem(icon_dir,files[i].name);
        ui->listFile->addItem(item);
    }
}
QString  MainWindow::formatIpaddr(sIP_ADDR& ipaddr)
{
    return QString("%1.%2.%3.%4").arg(ipaddr.addr1).arg(ipaddr.addr2).arg(ipaddr.addr3).arg(ipaddr.addr4);
}

void MainWindow::onReadPara(Device *dev, MsgDevicePara para)
{
    ui->edtDevId->setText(QString("%1").arg(m_cur_dev_id));
    ui->edtPassword->setText((const char*)para.mWifiPass);
    ui->edtSSID->setText((const char*)para.mWifiSSID);
    ui->edtWetDown->setText(QString("%1").arg(para.mWetDown));
    ui->edtWetUp->setText(QString("%1").arg(para.mWetUp));

    ui->cbxMode->setCurrentIndex(para.mWorkMode>1?0:para.mWorkMode);

    ui->edtDevIp->setText(formatIpaddr(para.Local_IP.ipaddr));
    ui->edtNetmask->setText(formatIpaddr(para.Local_IP.SubnetMask));
    ui->edtGateway->setText(formatIpaddr(para.Local_IP.GateWay));

    ui->edtTime->setText(para.mDateTime.toString());
    ui->edtServerIp->setText(formatIpaddr(para.Server_ip.ipaddr));
    ui->edtServerPort->setText(QString("%1").arg(para.Server_ip.port));


}
//设置
void MainWindow::onWritePara(Device *dev, bool result)
{

}

void MainWindow::onWaveProgress(Device *dev, QString progress)
{
    QTreeWidgetItem* item = findItemById(dev->id());
    if(item!=NULL)
    {
        item->setText(1,progress);
    }
}

//void MainWindow::on_btnStart_clicked()
//{
//    //if(srv.start(8888))
//    qDebug() << "start";
//    if(1)
//    {
//        ui->btnStart->setEnabled(false);
//        ui->btnStop->setEnabled(true);
//        QString str = QString("listen%1").arg(8888);
//        ui->statusBar->showMessage(str);
//    }
//}

void MainWindow::on_btnStop_clicked()
{
    if(srv->stop()){
        ui->btnWaveStart->setEnabled(true);
        ui->btnStop->setEnabled(false);
    }
}
//发送读取波形命令
void MainWindow::on_btnReadWave_clicked()
{
    dvm.SendAllWave(true);
}
QTreeWidgetItem* MainWindow::findItemById(quint32 id)
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it) {
        QTreeWidgetItem* item = *it;
        QVariant v = item->data(0,Qt::UserRole);

        quint32 d = v.value<quint32>();

          if (d == id)
          {
              return item;
          }

          ++it;
   }
    return NULL;
}

void MainWindow::onNotify(QString msg)
{
    ui->txtLog->append("send--> "+ msg);
}

void MainWindow::on_menu_click(bool)
{

    if(m_cur_dev_id!=0)
        readParam(m_cur_dev_id);
}
void MainWindow::on_write_menu_click(bool)
{
    on_btnSavePara_clicked();
}
void MainWindow::on_reset_menu_click(bool)
{
    if(m_cur_dev_id!=0)
        dvm.ResetDevice(m_cur_dev_id,1);
}
void MainWindow::on_list_files_menu_click(bool)
{
    if(m_cur_dev_id!=0)
        dvm.ListFiles(m_cur_dev_id);
}
void MainWindow::on_download_wave(bool)
{
    if(m_cur_dev_id!=0)
        dvm.SendWave(m_cur_dev_id,true);
}
void MainWindow::DevOffline(Device *dev)
{
    QTreeWidgetItem* item = findItemById(dev->id());
    if(item!=NULL)
    {
        item->setIcon(0,icon_device[1]);
    }
}

void MainWindow::DevOnline(Device *dev)
{
    QTreeWidgetItem* item = findItemById(dev->id());
    if(item!=NULL)
    {
        item->setIcon(0,icon_device[0]);
    }
}


void MainWindow::timerEvent(QTimerEvent *)
{
    QList<Device*> devices;
    dvm.ListDevice(devices);
    for(int i = 0; i < devices.size();i++)
    {
        QTreeWidgetItem* item = findItemById(devices[i]->id());
        if(item!=NULL)
        {
            if(devices[i]->online())
                item->setIcon(0,icon_device[0]);
            else
                item->setIcon(0,icon_device[1]);
        }
    }
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{

       QTreeWidgetItem* curItem=ui->treeWidget->itemAt(pos);  //获取当前被点击的节点
       if(curItem == NULL)
       {

           return;
       }

       menu->popup(QCursor::pos());

}

void MainWindow::on_btnSaveWave_clicked()
{
    m_waveWdg->Clear();
}

void MainWindow::on_btnWaveStart_clicked()
{
    if(srv->start(8888))
    {
        ui->btnWaveStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        QString str = QString("listen%1").arg(8888);
        ui->statusBar->showMessage(str);
    }
}
void MainWindow::listFiles(quint32 dev_id)
{

    QStringList wvFiles;
    dvm.GetDeviceWaveFiles(dev_id,wvFiles);
    ui->listWidget->clear();
    ui->listWidget->addItems(wvFiles);
    m_cur_dev_id = dev_id;
}
void MainWindow::readParam(quint32 dev_id)
{
    ui->edtDevId->clear();
    ui->edtDevIp->clear();
    ui->edtGateway->clear();
    ui->edtNetmask->clear();
    ui->edtPassword->clear();
    ui->edtSSID->clear();
    ui->edtTime->clear();
    ui->edtWetDown->clear();
    ui->edtWetUp->clear();
    ui->edtServerIp->clear();
    ui->edtServerPort->clear();
    dvm.ReadParam(dev_id);
}
void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    quint32 dev_id = item->data(0,Qt::UserRole).toInt();
    listFiles(dev_id);
    readParam(dev_id);
    dvm.ListFiles(dev_id);
    m_waveWdg->Clear();
}

void MainWindow::on_actionStation1_triggered()
{
    dvm.SetStation("工位1");
}

void MainWindow::on_actionStation2_triggered()
{
    dvm.SetStation("工位2");
}

void MainWindow::ShowDeviceChannel(quint32 dev_id, QString file,int chan)
{
    MsgWaveData wvd;
    dvm.LoadWaveFile(dev_id, file,wvd);
    m_waveWdg->SetData(wvd);

    m_waveWdg->DisplayAllChannel(true);


    checkAll(true);
    ui->statusBar->showMessage(QString("count=%1").arg( wvd.channels[0].size()));

    ui->rball->setChecked(true);
}
//显示该设备的波形.
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item==NULL)return;
    if(m_cur_dev_id == 0) return;


    ShowDeviceChannel(m_cur_dev_id,item->text(),0);
    //paintWave(wvd,0);
    //updatedataSlot();
}

void MainWindow::toIpAddr(QString ipstr, sIP_ADDR& addr)
{
    QHostAddress hostAddr(ipstr);
    quint32 ipaddr = hostAddr.toIPv4Address();
    addr.fromIp(ipaddr);

}
void MainWindow::toString(QString str, INT8U* dest,int size)
{
    std::string s = str.toStdString();
    for(int i = 0; i < s.size(); i++)
    {
        dest[i] = s[i];

    }
}

void MainWindow::toInt16U(QString str, INT16U &dest)
{
dest = str.toInt();
}

void MainWindow::toInt8U(QString str, INT8U &dest)
{
dest = str.toInt();
}

void MainWindow::toInt32U(QString str, quint32 &dest)
{
    dest = str.toInt();
}

void MainWindow::onWaveMsg(Device *dev, MsgWaveData data)
{
   if(data.m_first)
   {
       //m_waveWdg->Clear();
   }
   if(dev->id() != m_cur_dev_id)
   {
       return;
   }
   m_waveWdg->AppendData(data);
   static int count = 0;
   if( (count++ % m_refresh_count) == 0)
   {

       m_waveWdg->Display();

   }

}

//保存参数.
void MainWindow::on_btnSavePara_clicked()
{
    MsgDevicePara para;
    toIpAddr(ui->edtGateway->text(), para.Local_IP.GateWay);
    toIpAddr(ui->edtNetmask->text(), para.Local_IP.SubnetMask);
    toIpAddr(ui->edtDevIp->text(),   para.Local_IP.ipaddr);
    toIpAddr(ui->edtServerIp->text(), para.Server_ip.ipaddr);

    toString(ui->edtPassword->text(),para.mWifiPass,sizeof(para.mWifiPass));
    toString(ui->edtSSID->text(),para.mWifiSSID,sizeof(para.mWifiSSID));

    toInt16U(ui->edtWetUp->text(),para.mWetUp);
    toInt16U(ui->edtWetDown->text(),para.mWetDown);
    para.mWorkMode = ui->cbxMode->currentIndex();

    toInt32U(ui->edtServerPort->text(),para.Server_ip.port);

    dvm.WriteParam(m_cur_dev_id,para);
}


void MainWindow::closeEvent(QCloseEvent *)
{
    m_waveWdg->CloseAll();
}

void MainWindow::on_mytime_out()
{
    qDebug() << "mytimeout";
    dvm.ReadRt(m_cur_dev_id);
}

void MainWindow::on_actionReset_triggered()
{

}

void MainWindow::on_rb1_clicked(bool checked)
{
    m_waveWdg->DisplayChannel(0,checked);
    isAllCheck();
}
void MainWindow::checkAll(bool checked)
{
    ui->rb1->setChecked(checked);
    ui->rb2->setChecked(checked);
    ui->rb3->setChecked(checked);
    ui->rb4->setChecked(checked);
    ui->rb5->setChecked(checked);
    ui->rb6->setChecked(checked);
    ui->rb7->setChecked(checked);
    ui->rb8->setChecked(checked);
}
void MainWindow::on_rball_clicked(bool checked)
{
    m_waveWdg->DisplayAllChannel(checked);
    checkAll(checked);
}

void MainWindow::on_rb2_clicked(bool checked)
{
    m_waveWdg->DisplayChannel(1,checked);
    isAllCheck();
}

void MainWindow::on_rb3_clicked(bool checked)
{
m_waveWdg->DisplayChannel(2,checked);
}

void MainWindow::on_rb4_clicked(bool checked)
{
m_waveWdg->DisplayChannel(3,checked);
isAllCheck();
}

void MainWindow::on_rb5_clicked(bool checked)
{
m_waveWdg->DisplayChannel(4,checked);
isAllCheck();
}

void MainWindow::on_rb6_clicked(bool checked)
{
m_waveWdg->DisplayChannel(5,checked);
isAllCheck();
}

void MainWindow::on_rb7_clicked(bool checked)
{
    m_waveWdg->DisplayChannel(6,checked);
    isAllCheck();
}

void MainWindow::on_rb8_clicked(bool checked)
{

    m_waveWdg->DisplayChannel(7,checked);

isAllCheck();
}
void MainWindow::isAllCheck()
{
    bool isAll = false;
    isAll = ui->rb1->isChecked()&
            ui->rb2->isChecked()&
            ui->rb3->isChecked()&
            ui->rb4->isChecked()&
            ui->rb5->isChecked()&
            ui->rb6->isChecked()&
            ui->rb7->isChecked()&
            ui->rb8->isChecked();
    ui->rball->setChecked(isAll);
}

void MainWindow::onCalibResult(Device *dev, int chan, int index, int result)
{
    QMessageBox::information(this,"信息","标定完成");
}

void MainWindow::onRealTimeResult(Device *dev, RT_AD_RESULT result)
{
    int index = ui->cbxChan->currentIndex();
    if(index == -1) return;
    ui->edtRtAd->setText(QString("%1").arg(result.chan[index].ad));
    ui->edtrtWgt->setText(QString("%1").arg(result.chan[index].weight));
}

void MainWindow::on_btnStart_clicked()
{
    qDebug() << "on_btnStart_clicked";
    m_waveWdg->Clear();
    dvm.StartAll(true);
}

void MainWindow::on_btnStop_2_clicked()
{
    dvm.StartAll(false);
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item!=NULL)
    {

        quint32 dev_id = item->data(0,Qt::UserRole).toInt();
        listFiles(dev_id);
    }

}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_pushButton_2_clicked()
{

//    if(pause){
//        ui->pushButton_2->setText("暂停");
//        pause = false;
//    }else{
//        ui->pushButton_2->setText("继续");
//        pause = true;
//    }

}

void MainWindow::on_btnStartSend_clicked()
{
    dvm.SendAllWave(true);
}

void MainWindow::on_btnStopRead_clicked()
{
    dvm.SendAllWave(false);
}

void MainWindow::on_btnClear_clicked()
{
    ui->txtLog->clear();
}

void MainWindow::on_btnPause_clicked()
{
    if(pause){
        ui->btnPause->setText("暂停");
        pause = false;
    }else{
        ui->btnPause->setText("继续");
        pause = true;
    }
}

void MainWindow::on_btnCalibZero_clicked()
{
    dvm.calib(m_cur_dev_id, ui->cbxChan->currentIndex(),0,0);
}

void MainWindow::on_btnCalibWet_clicked()
{
    bool ok = false;
    int weight  = ui->edtCalibWet->text().toInt(&ok);
    if(ok){
        dvm.calib(m_cur_dev_id,ui->cbxChan->currentIndex(),1,weight);
    }

}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 4){
        m_timer.start(500);
    }else{
        m_timer.stop();
    }
}

void MainWindow::on_btnNext_clicked()
{
//下一页

}

void MainWindow::on_btnPrev_clicked()
{
//上一页

}
