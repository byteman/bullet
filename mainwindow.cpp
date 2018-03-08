#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>


void MainWindow::loadDeviceUI()
{


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
    }

}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_cur_dev_id = 0;
    qRegisterMetaType<SessMessage>("SessMessage");
    ui->setupUi(this);
    icon_device[0].addFile(":image/online.png");
    icon_device[1].addFile(":image/offline.png");
    icon_channel.addFile(":image/channel.png");
    icon_dir.addFile(":image/dir.png");
    icon_file.addFile(":image/channel.png");

    ui->btnStop->setEnabled(false);
    setupRealtimeDataDemo(ui->plot);
    srv.setParent(this);
    connect(&srv,SIGNAL(Message(SessMessage)),this,SLOT(Message(SessMessage)));
    connect(&srv,SIGNAL(Message(SessMessage)),&dvm
            ,SLOT(Message(SessMessage)));
    connect(&dvm,SIGNAL(DevOffline(Device*)),this,SLOT(DevOffline(Device*)));
    connect(&dvm,SIGNAL(DevOnline(Device*)),this,SLOT(DevOnline(Device*)));
    connect(&dvm,SIGNAL(ReadParam(Device*,MsgDevicePara)),this,SLOT(onReadPara(Device*,MsgDevicePara)));
    connect(&dvm,SIGNAL(WriteParam(Device*,bool)),this,SLOT(onWritePara(Device*,bool)));
    connect(&dvm,SIGNAL(EnumFiles(Device*,MsgFileList)),this,SLOT(onEnumFiles(Device*,MsgFileList)));
    connect(&dvm,SIGNAL(Progress(Device*,QString)),this,SLOT(onWaveProgress(Device*,QString)));
    connect(&dvm,SIGNAL(WaveMsg(Device*,MsgWaveData)),this,SLOT(onWaveMsg(Device*,MsgWaveData)));
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


    dvm.start();
    dvm.SetStation("工位1");
    loadDeviceUI();

    m_waveWdg = new WaveWidget(ui->plot,8);

    this->startTimer(1000);
    qDebug() <<"MainWindow thread id=" << thread();
}



MainWindow::~MainWindow()
{
    srv.stop();
    delete ui;
}



void MainWindow::setupRealtimeDataDemo(QCustomPlot* plot)
{


    demoName = "测力波形";



}


QString MainWindow::FormatHex(QByteArray& data)
{
    return data.toHex();
}
void MainWindow::Message(SessMessage s)
{
    ui->txtLog->append(FormatHex(s.getData()));
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
    if(srv.stop()){
        ui->btnWaveStart->setEnabled(true);
        ui->btnStop->setEnabled(false);
    }
}
//发送读取波形命令
void MainWindow::on_btnReadWave_clicked()
{
    dvm.SendAllWave();
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
    ui->txtLog->append(msg);
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
    if(srv.start(8888))
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
    dvm.ReadParam(dev_id);
}
void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    quint32 dev_id = item->data(0,Qt::UserRole).toInt();
    listFiles(dev_id);
    readParam(dev_id);
    dvm.ListFiles(dev_id);
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
  // m_waveWdg->AppendData(data);
   //m_waveWdg->DisplayAllChannel();
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

void MainWindow::on_btnStart_clicked()
{
    dvm.StartAll(true);
}

void MainWindow::on_btnStop_2_clicked()
{
    dvm.StartAll(false);
}
