#include "gpserver.h"

#include "qcustomplot.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogchanconfig.h"
#include <QThread>
#include "dao.h"
#define PAGE_SIZE 40

QString MainWindow::GetDevice(int ch)
{
    QMapIterator<QString, int> i(m_chans);
    while (i.hasNext()) {
        i.next();
        int chan = i.value();
        qDebug() << "chan=" << chan << "ch=" <<ch;
        if(ch <= chan){

            return dvm.GetDevice(i.key())->name();
        }
    }
    return "device";
}
/**
 * @brief 点击了通道配置按钮
 * @param addr
 * 1.直接读数据库中通道的参数
 * 2.显示出来供用户修改
 * 3.将修改后的参数写入到数据库
 * 4.更新设备对象中通道的相关参数.
 */
void MainWindow::onChannelClick(int addr)
{
    qDebug() << m_cur_dev_id << " chan=" << addr;
    DialogChanConfig dlg;
    DeviceChnConfig cfg;

    QString name = GetDevice(addr);
    int chan = addr % 8;
    DAO::instance().DeviceChannalGet(name,chan,cfg);

    dlg.SetChanConfig(name,chan,cfg);
    int result = dlg.exec();
    if(result == QDialog::Accepted){
        dlg.GetChanConfig(cfg);

        DAO::instance().DeviceChannalUpdate(name,chan,cfg);
        devices->SetChanSetting(addr,cfg);

    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {

    }else if (event->type() == QEvent::Resize) {
        if(devices!=NULL)
            devices->Resize();
        return true;
    }

    return QWidget::eventFilter(watched, event);
}
void MainWindow::InitDevice(QList<Device*> &devs)
{
    devices = new MyDevices(42,ui->gbDevices);

    devices->SetDeviceNum(1,40);
    devices->SetUnit("kg");
    m_chans.clear();
    for(int i = 0; i < devs.size(); i++)
    {
        for(int j = 0; j < 8; j++)
        {
            int addr = i*8+j+1;
            int ch = j+1;
            QString title = QString("%1:%2").arg(devs[i]->name()).arg(ch);
            devices->SetTitle(addr, title);
            DeviceChnConfig cfg;
            cfg.maxValue = m_max;
            cfg.minValue = m_min;
            //DAO::instance().DeviceChannalGet(devs[i]->name(),j+1,cfg);
            devices->SetChanSetting(i*8+j+1,cfg);
        }
        m_chans[devs[i]->id()] = (i+1)*8;
    }
    ui->gbDevices->installEventFilter(this);
    connect(devices,SIGNAL(onChannelConfig(int)),this,SLOT(onChannelClick(int)));

}
void MainWindow::loadDeviceUI()
{

    pause = false;
    QList<Device*> devs;
    ui->treeWidget->clear();
    ui->treeWidget->setIconSize(QSize(48,48));
    dvm.ListDevice(devs);
    for(int i = 0; i < devs.size();i++)
    {
        QTreeWidgetItem* item = NULL;
        item = new QTreeWidgetItem(QStringList(devs[i]->name()));

        item->setText(1,"100%");
        ui->treeWidget->addTopLevelItem(item);

        item->setIcon(0,icon_device[1]);

        item->setData(0,Qt::UserRole,devs[i]->id());
        if(i == 0){
            ui->treeWidget->setCurrentItem(item);
            m_cur_dev_id = devs[i]->id();
        }
    }
    chanels[0] = ui->label_12;
    chanels[1] = ui->label_19;
    chanels[2] = ui->label_25;
    chanels[3] = ui->label_11;
    chanels[4] = ui->label_24;
    chanels[5] = ui->label_23;
    chanels[6] = ui->label_22;
    chanels[7] = ui->label_21;
    InitDevice(devs);
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_cur_dev_id = "";
    m_cur_page = 0;
    qRegisterMetaType<SessMessage>("SessMessage");
    ui->setupUi(this);
    DAO::instance().Init("bullet.db");
    QSettings config("bullet.ini", QSettings::IniFormat);
    config.setIniCodec("UTF-8");//添上这句就不会出现乱码了);

    m_debug_bytes = config.value("/device/debug",20).toInt();
    m_refresh_count = config.value("/device/refresh",20).toInt();
    m_cur_station = config.value("/device/station",1).toInt();
    int dot= 3 - config.value("/device/dot",3).toInt();
    if(dot < 0 || dot > 3) dot = 0;

    m_max = config.value("/device/max",10000).toDouble();
    m_min = config.value("/device/min",-10000).toDouble();

    m_full =  config.value("/device/full",10000).toInt();
    ui->edtFull->setText(QString("%1").arg(m_full));
    ui->cbxDot->setCurrentIndex(dot);
    m_dot  =  ui->cbxDot->currentText().toInt();
    stationActions[1] = ui->actionStation1;
    stationActions[2] = ui->actionStation2;
    stationActions[3] = ui->action3;
    stationActions[4] = ui->action4;
    stationActions[5] = ui->action5;
    stationActions[6] = ui->actionW;
    stationActions[7] = ui->actionW_2;
    stationActions[8] = ui->action2;
    stationActions[9] = ui->action3_2;
    stationActions[10] = ui->action3_3;


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
    connect(&dvm,SIGNAL(EnumFiles(Device*,ENUM_FILE_RESP)),this,SLOT(onEnumFiles(Device*,ENUM_FILE_RESP)));
    connect(&dvm,SIGNAL(Progress(Device*,QString)),this,SLOT(onWaveProgress(Device*,QString)));
    connect(&dvm,SIGNAL(WaveMsg(Device*,MsgWaveData)),this,SLOT(onWaveMsg(Device*,MsgWaveData)));
    connect(&dvm,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCalibResult(Device*,int,int,int)));
    connect(&dvm,SIGNAL(RealTimeResult(Device*,RT_AD_RESULT)),this,SLOT(onRealTimeResult(Device*,RT_AD_RESULT)));

    connect(&dvm,SIGNAL(CommResult(Device*,int,int)),this,SLOT(onCommResult(Device*,int,int)));

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


    action = new QAction("删除设备文件",this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_remove_menu_click(bool)));


//    action = new QAction("同步设备文件",this);
//    menu->addAction(action);

//    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_download_wave(bool)));


    dvm.start();
    setCurrentStation(m_cur_station);
    //dvm.SetStation("工位1");
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

void MainWindow::onEnumFiles(Device *dev, ENUM_FILE_RESP resp)
{
    ui->listFile->clear();
    ui->cbxCurPage->clear();
    for(int j = 0; j < resp.total_page;j++)
    {
        ui->cbxCurPage->addItem(QString("%1").arg(j+1));
    }
    ui->cbxCurPage->setCurrentIndex(resp.cur_page);
    m_cur_page = resp.cur_page;
    for(int i =0; i < resp.files.size();i++)
    {
        QListWidgetItem* item = NULL;
        if(resp.files[i].attr == 1)
            item = new QListWidgetItem(icon_file,resp.files[i].name);
        else
            item = new QListWidgetItem(icon_dir,resp.files[i].name);
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
QTreeWidgetItem* MainWindow::findItemById(QString id)
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it) {
        QTreeWidgetItem* item = *it;
        QVariant v = item->data(0,Qt::UserRole);

        QString d = v.value<QString>();

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
    if(pause)return;
    ui->txtLog->append("send--> "+ msg);
}

void MainWindow::on_menu_click(bool)
{

}
void MainWindow::on_write_menu_click(bool)
{
    on_btnSavePara_clicked();
}
void MainWindow::on_reset_menu_click(bool)
{

}
void MainWindow::MyListFiles(int id,int page)
{

}
void MainWindow::on_list_files_menu_click(bool)
{

}
void MainWindow::on_download_wave(bool)
{

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
void MainWindow::listFiles(QString dev_id)
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
    //dvm.ReadParam(dev_id);
}
void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QString dev_id = item->data(0,Qt::UserRole).toString();
    listFiles(dev_id);
    //readParam(dev_id);
    //MyListFiles(dev_id,m_cur_page);

    m_waveWdg->Clear();
}
void MainWindow::setCurrentStation(int index)
{
    QSettings config("bullet.ini", QSettings::IniFormat);
    config.setIniCodec("UTF-8");//添上这句就不会出现乱码了);

    config.setValue("/device/station",index);
    dvm.SetStation(QString("工位%1").arg(index));
    QMapIterator<int, QAction*> i(stationActions);
    while (i.hasNext()) {
        i.next();
        i.value()->setChecked(false);
    }
    if(stationActions.contains(index)){
        stationActions[index]->setChecked(true);
    }
}
void MainWindow::on_actionStation1_triggered()
{
    setCurrentStation(1);
}

void MainWindow::on_actionStation2_triggered()
{
    setCurrentStation(2);
}

void MainWindow::ShowDeviceChannel(QString dev_id, QString file,int chan)
{
    MsgWaveData wvd;
    dvm.LoadWaveFile(dev_id, file,wvd);
    m_waveWdg->SetData(wvd);

    for(int i = 0; i < 8; i++){
        double min,max;
        m_waveWdg->GetValueRange(i,min,max);
        chanels[i]->setText(QString("%1").arg(max));

    }
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

   if(m_chans.contains(dev->id())){
       for(int i = 0; i < 8; i++){
            int start = m_chans[dev->id()]-8;
            if(data.channels[i].size() == 0) continue;
            devices->DisplayWeight(start+i+1, data.channels[i][0],0,m_dot);
//            for(int j=0; j<data.channels[i].size(); j++){
//                devices->DisplayWeight(start+i+1, data.channels[i][j],0,0);
//            }
       }

   }
//   if(dev->id() != m_cur_dev_id)
//   {
//       return;
//   }
//   m_waveWdg->AppendData(data);
//   static int count = 0;
//   if( (count++ % m_refresh_count) == 0)
//   {

//       m_waveWdg->Display();

//   }
//   for(int i = 0; i < 8; i++){
//       double min,max;
//       m_waveWdg->GetValueRange(i,min,max);
//       chanels[i]->setText(QString("%1").arg(max));

//   }

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
    QSettings config("bullet.ini", QSettings::IniFormat);
    config.setIniCodec("UTF-8");

    int dot = ui->cbxDot->currentText().toInt();
    config.setValue("/device/dot",dot);
    config.setValue("/device/full",ui->edtFull->text());
    m_dot = dot;
    dvm.WriteParam(m_cur_dev_id,para);
}


void MainWindow::closeEvent(QCloseEvent *)
{
    m_waveWdg->CloseAll();
}

void MainWindow::onCommResult(Device *dev, int cmd, int result)
{
    if(cmd == MSG_REMOVE_FILE)
    {
        dvm.ListFiles(m_cur_dev_id,0,PAGE_SIZE);
    }
}

void MainWindow::on_mytime_out()
{
    //qDebug() << "mytimeout";
    //dvm.ReadRt(m_cur_dev_id);
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
static QString float2string(float wf, int dot)
{
    char buf[64] = {0,};
    switch(dot)
    {

        case 1:
            qsnprintf(buf,64,"%0.1f",wf/10);
            break;
        case 2:
            qsnprintf(buf,64,"%0.2f",wf/100);
            break;
        case 3:
            qsnprintf(buf,64,"%0.3f",wf/1000);
            break;
        case 4:
            qsnprintf(buf,64,"%0.4f",wf/10000);
            break;
        default:
            qsnprintf(buf,64,"%d",int(wf));
            break;
    }
    return buf;
}


void MainWindow::onRealTimeResult(Device *dev, RT_AD_RESULT result)
{
    int index = ui->cbxChan->currentIndex();
    if(index == -1) return;
    ui->edtRtAd->setText(QString("%1").arg(result.chan[index].ad));
    ui->edtrtWgt->setText(QString("%1").arg(float2string(result.chan[index].weight,m_dot)));
    //ui->edtCalibWet->setText(QString("%1").arg(result.chan[index].weight));

}

void MainWindow::on_btnStart_clicked()
{
    qDebug() << "on_btnStart_clicked";
    m_waveWdg->Clear();
    dvm.StartAll(true);
    devices->ResetAlarm();

}

void MainWindow::on_btnStop_2_clicked()
{
    dvm.StartAll(false);
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item!=NULL)
    {

        QString dev_id = item->data(0,Qt::UserRole).toString();
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
    //MyListFiles(m_cur_dev_id,m_cur_page+1);

}

void MainWindow::on_btnPrev_clicked()
{
    if(m_cur_page>0)
    {
        //MyListFiles(m_cur_dev_id,m_cur_page-1);
    }
//上一页
    //dvm.ListFiles()
}

void MainWindow::on_cbxCurPage_currentIndexChanged(int index)
{

   //dvm.ListFiles(m_cur_dev_id,index,PAGE_SIZE);
}

void MainWindow::on_btnGo_clicked()
{
    int idx = ui->cbxCurPage->currentIndex();
    if(idx >= 0){
        dvm.ListFiles(m_cur_dev_id,idx,PAGE_SIZE);
    }
}

void MainWindow::removeFiles()
{
    if(QMessageBox::Yes==QMessageBox::question(this,"提示","确定删除文件"))
    {
        dvm.RemoveFile(m_cur_dev_id,"");
        //MyListFiles(m_cur_dev_id,m_cur_page);
    }

}
void MainWindow::on_btnDelAll_clicked()
{
    removeFiles();
}
void MainWindow::on_remove_menu_click(bool)
{
    removeFiles();
}

void MainWindow::on_listFile_itemDoubleClicked(QListWidgetItem *item)
{
    if(QMessageBox::Yes==QMessageBox::question(this,"提示","下载设备文件"))
    {
        dvm.SyncFile(m_cur_dev_id, item->text());
    }

}

void MainWindow::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{

}

void MainWindow::on_btnResetZero_clicked()
{
    dvm.calib(m_cur_dev_id, 0,100,0);
}

void MainWindow::on_action3_triggered()
{
    setCurrentStation(3);
}

void MainWindow::on_action4_triggered()
{
    setCurrentStation(4);
}

void MainWindow::on_action5_triggered()
{
    setCurrentStation(5);
}

void MainWindow::on_actionW_triggered()
{
    setCurrentStation(6);
}

void MainWindow::on_actionW_2_triggered()
{
    setCurrentStation(7);
}

void MainWindow::on_action2_triggered()
{
    setCurrentStation(8);
}

void MainWindow::on_action3_2_triggered()
{
    setCurrentStation(9);
}

void MainWindow::on_action3_3_triggered()
{
    setCurrentStation(10);
}

void MainWindow::on_treeWidget_clicked(const QModelIndex &index)
{

}

void MainWindow::on_treeWidget_itemChanged(QTreeWidgetItem *item, int column)
{

}
