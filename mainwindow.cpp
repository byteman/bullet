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

        ui->treeWidget->addTopLevelItem(item);
        item->setIcon(0,icon_device[1]);

        item->setData(0,Qt::UserRole,devices[i]->id());
        for(int j = 0; j < 8; j++)
        {
            QTreeWidgetItem* item2= new QTreeWidgetItem(QStringList(QString("通道%1").arg(j+1)));
            item2->setIcon(0,icon_channel);
            item->addChild(item2);
            item2->setData(0,Qt::UserRole,j);
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
    icon_device[0].addFile(":image/online.png");
    icon_device[1].addFile(":image/offline.png");
    icon_channel.addFile(":image/channel.png");
    ui->btnStop->setEnabled(false);
    setupRealtimeDataDemo(ui->qwtPlot);
    srv.setParent(this);
    connect(&srv,SIGNAL(Message(SessMessage)),this,SLOT(Message(SessMessage)));
    connect(&srv,SIGNAL(Message(SessMessage)),&dvm
            ,SLOT(Message(SessMessage)));
    connect(&dvm,SIGNAL(DevOffline(Device*)),this,SLOT(DevOffline(Device*)));
    connect(&dvm,SIGNAL(DevOnline(Device*)),this,SLOT(DevOnline(Device*)));
    connect(&dvm,SIGNAL(onReadParam(Device*,MsgDevicePara)),this,SLOT(onReadPara(Device*,MsgDevicePara)));
    connect(&dvm,SIGNAL(onWriteParam(Device*,bool)),this,SLOT(onWritePara(Device*,bool)));

    ui->treeWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    menu=new QMenu(this);
    QAction* action = new QAction("读取参数",this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_menu_click(bool)));
    dvm.start();
    dvm.SetStation("工位1");
    loadDeviceUI();
    ui->rb1->setChecked(true);
    m_waveWdg = new WaveWidget(ui->qwtPlot,8);

    this->startTimer(1000);
    qDebug() <<"MainWindow thread id=" << thread();
}



MainWindow::~MainWindow()
{
    srv.stop();
    delete ui;
}



void MainWindow::setupRealtimeDataDemo(QwtPlot *qwtplot)
{


    demoName = "测力波形";
    qwtplot->setTitle(demoName);
    qwtplot->setCanvasBackground(Qt::gray);//背景
    qwtplot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);//标签


    QTime curtime;
    curtime=curtime.currentTime();
    qwtplot->setAxisTitle(QwtPlot::xBottom, "时间");
    qwtplot->setAxisTitle(QwtPlot::yLeft,"AD");
    qwtplot->setAxisScale(QwtPlot::yLeft,0,100);
    qwtplot->setAxisScale(QwtPlot::xBottom,0,1);



    QwtPlotZoomer *zoomer = new QwtPlotZoomer( qwtplot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::blue ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( qwtplot->canvas() );                 //默认的滑轮及右键缩放功能  图形的整体缩放

    magnifier->setMouseButton(Qt::LeftButton);     //设置哪个按钮与滑轮为缩放画布  如果不设置(注册掉当前行)按钮默认为滑轮以及右键为缩放

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX( true );//设置网格线
    grid->enableY( true );
    grid->setMajorPen( Qt::black, 0, Qt::DotLine );
    grid->attach(qwtplot);

    //connect(&updateTimer,SIGNAL(timeout()),this,SLOT(updatedataSlot()));
    //updateTimer.start(0);


}

/**
 * @brief getData
 * @param inteval
 * @return
 * 获取一个值  模拟串口接收到的值
 */
double MainWindow::getData(double time){

    double s = qCos( time * M_PI * 2 ) ;
    return s;
}


//用于更新ydata,实际情况就是read数据
void MainWindow::updatedataSlot(){
    static QTime dataTime(QTime::currentTime());
    long int eltime = dataTime.elapsed();
    static int lastpointtime = 0;

    int size = (eltime - lastpointtime);


    if(size>0){//有数据传入
        ydata.erase(ydata.begin(),ydata.begin()+size);//擦除多余的数据
        for(int i=1;i<size+1;i++){
            ydata.append(getData((((double)lastpointtime+i)/1000)));
            xdata.append(0);
        }
        lastpointtime = eltime;
    }

    curve->setSamples(xdata,ydata);
    curve->attach(ui->qwtPlot);
    ui->qwtPlot->replot();



}

QString MainWindow::FormatHex(QByteArray& data)
{
    return data.toHex();
}
void MainWindow::Message(SessMessage s)
{
    ui->txtLog->append(FormatHex(s.getData()));
}
QString  MainWindow::formatIpaddr(sIP_ADDR& ipaddr)
{
    return QString("%1.%2.%3.%4").arg(ipaddr.addr1).arg(ipaddr.addr2).arg(ipaddr.addr3).arg(ipaddr.addr4);
}

void MainWindow::onReadPara(Device *dev, MsgDevicePara para)
{
    ui->edtDevId->setText(QString("%1").arg(1));
    ui->edtPassword->setText((const char*)para.mWifiPass);
    ui->edtSSID->setText((const char*)para.mWifiSSID);
    ui->edtWetDown->setText(QString("%1").arg(para.mWetDown));
    ui->edtWetUp->setText(QString("%1").arg(para.mWetUp));

    ui->cbxMode->setCurrentIndex(para.mWorkMode>1?0:para.mWorkMode);

    ui->edtDevIp->setText(formatIpaddr(para.Local_IP.ipaddr));
    ui->edtNetmask->setText(formatIpaddr(para.Local_IP.SubnetMask));
    ui->edtGateway->setText(formatIpaddr(para.Local_IP.GateWay));


    ui->edtServerIp->setText(formatIpaddr(para.Server_ip.ipaddr));
    ui->edtServerIp->setText(QString("%1").arg(para.Server_ip.port));


}

void MainWindow::onWritePara(Device *dev, bool result)
{


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
    qDebug() << "menu click";
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
    qDebug() << "stop";
    if(1)
    {
        ui->btnWaveStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        QString str = QString("listen%1").arg(8888);
        ui->statusBar->showMessage(str);
    }
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
    if(item->childCount() > 0)
    {
        quint32 dev_id = item->data(0,Qt::UserRole).toInt();
        listFiles(dev_id);
        readParam(dev_id);
    }
    else
    {
        quint32 dev_id = item->parent()->data(0,Qt::UserRole).toInt();
        int index = item->data(0,Qt::UserRole).toInt();


    }


}

void MainWindow::on_actionStation1_triggered()
{
    dvm.SetStation("工位1");
}

void MainWindow::on_actionStation2_triggered()
{
    dvm.SetStation("工位2");
}
void MainWindow::paintWave(MsgWaveData& wvd, int chan)
{
    xdata.clear();
    ydata.clear();
    int size = wvd.channels[chan].size();
    double max=0,min=0;
    if(size>0){//有数据传入
        //ydata.erase(ydata.begin(),ydata.begin()+size);//擦除多余的数据
        max = min= wvd.channels[chan][0];
        double value = 0;
        for(int i=0;i<size;i++){
            xdata.append(i);
            ydata.append(wvd.channels[chan][i]);
            value = wvd.channels[chan][i];
            if(value > max) max = value;
            if(value < min) min = value;
        }

    }
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,min,max);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,size);


    curve->setSamples(xdata,ydata);

    curve->attach(ui->qwtPlot);
    ui->qwtPlot->replot();
}
void MainWindow::ShowDeviceChannel(quint32 dev_id, QString file,int chan)
{
    MsgWaveData wvd;
    dvm.LoadWaveFile(dev_id, file,wvd);
    m_waveWdg->SetData(wvd);
    m_waveWdg->DisplayChannel(chan);
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

void MainWindow::on_rb1_clicked()
{
     m_waveWdg->DisplayChannel(0);
}

void MainWindow::on_rb2_clicked()
{
    m_waveWdg->DisplayChannel(1);
}

void MainWindow::on_rb3_clicked()
{
m_waveWdg->DisplayChannel(2);
}

void MainWindow::on_rb4_clicked()
{
m_waveWdg->DisplayChannel(3);
}

void MainWindow::on_rb5_clicked()
{
m_waveWdg->DisplayChannel(4);
}

void MainWindow::on_rb6_clicked()
{
m_waveWdg->DisplayChannel(5);
}

void MainWindow::on_rb7_clicked()
{
m_waveWdg->DisplayChannel(6);
}

void MainWindow::on_rb8_clicked()
{
    m_waveWdg->DisplayChannel(7);
}
void MainWindow::toIpAddr(QString ipstr, sIP_ADDR& addr)
{
    QHostAddress hostAddr(ipstr);
    quint32 ipaddr = hostAddr.toIPv4Address();
    memcpy((void*)&addr, &ipaddr, sizeof(sIP_ADDR));

}
void MainWindow::toString(QString str, INT8U* dest,int size)
{
    std::string s = str.toStdString();
    for(int i = 0; i < size; i++)
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
