#pragma execution_character_set("utf-8")

#include "MainWnd.h"
#include "ui_MainWnd.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "dao.h"
#include "dialogparams.h"
#include "dialogdevice.h"
#include "dialogchanconfig.h"
#include "utils.h"
#include "config.h"
#include "main.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include "xlsx/statemanager.h"
#define MAX_CHAN_NR 8
#define LISTEN_PORT 8881
void MainWnd::AddLog(QString msg)
{
    ui->txtLog->append(msg);
}
void MainWnd::StartReceiver()
{
    srv = new UdpServer();

    if(!srv->start(Config::instance().m_local_port)){
        AddLog(QString::fromLocal8Bit("服务启动失败,检查端口8881是否被占用!!"));
    }else{
        AddLog(QString::fromLocal8Bit("服务启动成功"));
    }
    connect(srv,SIGNAL(Message(SessMessage)),this,SLOT(Message(SessMessage)));
    connect(srv,SIGNAL(Message(SessMessage)),&dvm
            ,SLOT(Message(SessMessage)));

}
bool MainWnd::InitDvm()
{
    connect(&dvm,SIGNAL(Notify(QString)),this,SLOT(onNotify(QString)));
    connect(&dvm,SIGNAL(DevOffline(Device*)),this,SLOT(DevOffline(Device*)));
    connect(&dvm,SIGNAL(DevOnline(Device*)),this,SLOT(DevOnline(Device*)));
    connect(&dvm,SIGNAL(WaveMsg(Device*,MsgWaveData)),this,SLOT(onWaveMsg(Device*,MsgWaveData)));
    connect(&dvm,SIGNAL(SensorMsg(Device*,MsgSensorData)),this,SLOT(onSensorMsg(Device*,MsgSensorData)));
    connect(&dvm,SIGNAL(ResetResult(Device*,bool)),this,SLOT(onResetResult(Device*,bool)));
    //connect(&dvm,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCalibResult(Device*,int,int,int)));
    connect(&dvm,SIGNAL(RealTimeResult(Device*,RT_AD_RESULT)),this,SLOT(onRealTimeResult(Device*,RT_AD_RESULT)));

    return dvm.Init();
}
//初始化
//1.参数的加载
//2.界面的初始化
//3.系统的启动.


typedef GoInt (*funcBuildReport)(GoString p0,char** res);
#if 0
void testhttp(funcHttpGet HttpGet){
    const char* str="http://www.baidu.com";
    GoString gstr;
    gstr.p=str;
    gstr.n = (ptrdiff_t)strlen(str) ;
    char* res=nullptr;
    qDebug() << "ready call http";
    HttpGet(gstr,&res);
     qDebug() << "return";
     if(res!=nullptr){
         qDebug() << "return ok len=" << strlen(res);
         QString strMsg = QString(res);
         qDebug() << "------------";
         //printf("data=%s\n",res);
          // QByteArray a(res,strlen(res));
         qDebug() << "msg=" << strMsg.left(10);
         qDebug() << "=============";
     }




    //qDebug("%s\n",res);
    //free(res);
    res=nullptr;
}
#endif
static  funcBuildReport pFnBuildReport  = NULL;

void initGoLibrary()
{
     HINSTANCE hInstance = LoadLibraryA("report.dll");
     if (NULL != hInstance)
     {
         qDebug() <<"LoadLib succ";
         pFnBuildReport = (funcBuildReport)GetProcAddress(hInstance,"GenerateReport");
         if (pFnBuildReport)
         {
             qDebug() << "Find GenerateReport";
         }
//             const char* str="http://www.baidu.com";


//             GoString gstr;
//             gstr.p=str;
//             gstr.n = (ptrdiff_t)strlen(str) ;
//             char* res=nullptr;
//             GoInt code = pFnBuildReport(gstr,&res);
//             qDebug() <<"code=" << code << "msg" << QString(res);
//         }

     }else{
         qDebug() <<"load report.dll failed";
     }

}
bool MainWnd::Init()
{

    //StateManager::instance().parse("state.xlms");
    initGoLibrary();
    //首先初始化数据管理模块.

    QSqlError err =  DAO::instance().Init(QCoreApplication::applicationDirPath()+"/measure.db");
    if(err.isValid()){
        //初始化失败
        AddLog(err.text());
        myHelper::ShowMessageBoxError(err.text());
        return false;
    }
    Config::instance().Init();
    //首先初始化设备管理器.
    if(!InitDvm()){
        AddLog(err.text());
        myHelper::ShowMessageBoxError(err.text());
        return false;
    }
    //初始化UI相关.
    this->initUI();
    return true;
}

void MainWnd::Message(SessMessage s)
{
    //if(pause)return;
   // if(s.getData().size() <   m_debug_bytes)
    //AddLog(QString("recv-> %1:%2 ").arg(s.getHost().toString()).arg(s.getPort())+FormatHex(s.getData()));
}

void MainWnd::onResetResult(Device *, bool)
{
    myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("复位设备成功"));

}
void MainWnd::onNotify(QString msg)
{

    //ui->txtLog->append("send--> "+ msg);
}
/**
 * 点击了某个设备通道的播放或者暂停.
 * 1.修改界面的状态
 * 2.写数据库修改通道状态
 * 3.修改设备对象中通道的状态.
*/
void MainWnd::onPlayClick(int addr, bool played)
{
    qDebug() <<"addr="<<addr<< " onPlayClick played=" << played;
    dvm.ControlDeviceChan(m_cur_dev_id,addr,played);
    devices->SetRecState(addr,played);
}
/**
 * @brief 点击了通道配置按钮
 * @param addr
 * 1.直接读数据库中通道的参数
 * 2.显示出来供用户修改
 * 3.将修改后的参数写入到数据库
 * 4.更新设备对象中通道的相关参数.
 */
void MainWnd::onChannelClick(int addr)
{
    qDebug() << m_cur_dev_id << " chan=" << addr;
    DialogChanConfig dlg;
    DeviceChnConfig cfg;
    dvm.GetDeviceChan(m_cur_dev_id,addr,cfg);
    dlg.SetChanConfig(m_cur_dev_id,addr,cfg);
    int result = dlg.exec();
    if(result == QDialog::Accepted){
        dlg.GetChanConfig(cfg);
        dvm.UpdateDeviceChan(m_cur_dev_id,addr,cfg);
        devices->SetChanSetting(addr,cfg);

    }
}

//参数配置界面
void MainWnd::on_menu_click(bool)
{
    DialogParams dlg(&dvm,m_cur_dev_id);
    int reason = dlg.exec();

    qDebug() << reason;

}
void MainWnd::on_write_menu_click(bool)
{

}
void MainWnd::on_get_count_click(bool)
{
   int count = dvm.GetDeviceCount(m_cur_dev_id);
   QMessageBox::information(this,QStringLiteral("信息"),QString("count=%1").arg(count));

}
void MainWnd::on_reset_count_click(bool)
{
    dvm.ResetDeviceCount(m_cur_dev_id);
}
//添加一个设备.
void MainWnd::on_add_device_click(bool)
{
    qDebug() << "add device";
    DialogDevice dlg;
    int result = dlg.exec();
    if(result == QDialog::Accepted){
        QString dev_id,dev_name;
        dlg.GetDeviceInfo(dev_id,dev_name);
        dvm.AddDevice(dev_id,dev_name);
        reloadDeviceList();
        reloadDeviceList2();
    }
}
bool MainWnd::GetCurrentDeviceId(QString& id)
{
    QTreeWidgetItem* item =  ui->treeWidget->currentItem();
    if(item==NULL){
         return false;
    }
    id = item->data(0,Qt::UserRole).toString();
    return true;
}
bool MainWnd::GetCurrentDeviceId2(QString& id)
{
    QTreeWidgetItem* item =  ui->treeWidget2->currentItem();
    if(item==NULL){
         return false;
    }
    id = item->data(0,Qt::UserRole).toString();
    return true;
}
//删除一个设备.
void MainWnd::on_remove_device_click(bool)
{

     QString id;
     if(!GetCurrentDeviceId(id))
     {
         return;
     }
     if(dvm.RemoveDevice(id)){
         myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("删除设备成功"));
     }else{
         myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("删除设备失败"));
     }
     reloadDeviceList();
      reloadDeviceList2();

}
#include "dialogupdate.h"
void MainWnd::on_update_menu_click(bool)
{
    QString id;
    if(!GetCurrentDeviceId(id))
    {
        qDebug()<<"Can not GetCurrentDeviceId";
        return;
    }
    Device* dev = dvm.GetDevice(id);
    if(dev==NULL)
    {
        qDebug()<<"Can not GetDevice " << id;
        return;
    }
    QString ip;
    dev->GetHostAddr(ip);
    const QUrl regUrl(QString("http://%1").arg(ip));
    QDesktopServices::openUrl(regUrl);

//    DialogUpdate dlg;


//    dlg.SetServerHost(ip);
//    dlg.exec();
}

//修改设备名称.
void MainWnd::on_modify_menu_click(bool)
{
    qDebug()<<m_cur_dev_id << " modify";

    QString id;
    if(!GetCurrentDeviceId(id))
    {
        qDebug()<<"Can not GetCurrentDeviceId";
        return;
    }
    Device* dev = dvm.GetDevice(id);
    if(dev==NULL)
    {
        qDebug()<<"Can not GetDevice " << id;
        return;
    }
    DialogDevice dlg;
    dlg.UpdateDeviceInfo(dev->id(),dev->name());

    int result = dlg.exec();
    if(result == QDialog::Accepted){
        QString dev_id,dev_name;
        dlg.GetDeviceInfo(dev_id,dev_name);
        dvm.UpdateDevice(dev_id,dev_name);
        reloadDeviceList();
         reloadDeviceList2();
    }
}
//复位设备.
void MainWnd::on_reset_menu_click(bool)
{
    dvm.ResetDevice(m_cur_dev_id,1);
}
QTreeWidgetItem* MainWnd::findItemById(QString id)
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
QTreeWidgetItem* MainWnd::findItemById2(QString id)
{
    QTreeWidgetItemIterator it(ui->treeWidget2);
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
void MainWnd::DevOffline(Device *dev)
{
    QTreeWidgetItem* item = findItemById(dev->id());
    if(item!=NULL)
    {
        item->setIcon(0,icon_device[1]);
    }
    item = findItemById2(dev->id());
    if(item!=NULL)
    {
        item->setIcon(0,icon_device[1]);
    }
}

void MainWnd::DevOnline(Device *dev)
{
    QTreeWidgetItem* item = findItemById(dev->id());
    if(item!=NULL)
    {
        item->setIcon(0,icon_device[0]);
    }
    item = findItemById2(dev->id());
        if(item!=NULL)
        {
            item->setIcon(0,icon_device[0]);
        }
}
//1s 定时监测设备是否在线.
void MainWnd::timerEvent(QTimerEvent *)
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
    for(int i = 0; i < devices.size();i++)
    {
        QTreeWidgetItem* item = findItemById2(devices[i]->id());
        if(item!=NULL)
        {
            if(devices[i]->online())
                item->setIcon(0,icon_device[0]);
            else
                item->setIcon(0,icon_device[1]);
        }
    }
    //simData();

}
//树形设备列表右键菜单.
void MainWnd::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{

       QTreeWidgetItem* curItem=ui->treeWidget->itemAt(pos);  //获取当前被点击的节点
       if(curItem == NULL)
       {
           menu2->popup(QCursor::pos());
           return;
       }
       m_cur_dev_id = curItem->data(0,Qt::UserRole).toString();
       menu->popup(QCursor::pos());

}
//点击了树形设备列表
//切换设备.
//切换切换后设备的当前的通道配置
//读取通道的录制状态.更新状态
void MainWnd::changeDevice(QString dev_id)
{
    if(devices==NULL){
        return;
    }
    for(int i = 1; i <= MAX_CHAN_NR; i++)
    {
        DeviceChnConfig cfg;
        //从数据库中获取参数的状态.
        //修改显示的配置
        if(dvm.GetDeviceChan(dev_id,i,cfg)){
            devices->SetChanSetting(i,cfg);
            Device* dev = dvm.GetDevice(dev_id);
            if(dev!=NULL){
                devices->SetTitle(i,QString(QStringLiteral("%1:通道%2")).arg(dev->name()).arg(i));
            }else{
                devices->SetTitle(i,QString(QStringLiteral("通道%1")).arg(i));
            }

        }
        //修改设备对象的配置.
        dvm.SetChanConfig(dev_id,i,cfg);
    }
    ui->treeWidget2->setCurrentItem(findItemById2(dev_id));
    devices->ClearDisplay();

    int addr = devices->GetZoomWidget();
    if(addr!=-1){
        onWaveShow(addr,true);
    }
}


void MainWnd::onSensorMsg(Device *dev, MsgSensorData data)
{
    if(dev->id() != m_cur_dev_id){
        return;
    }
    for(int i = 0; i < data.channels.size(); i++)
    {
        devices->DisplayWeight(data.channels[i].addr,data.channels[i].weight,0,0);
    }
    //wave->AppendData(0,get_random_number());
    //wave->DisplayChannel(0,true);
}
void DAOTest()
{
    DAO::instance().DeviceAdd("123","hello");
    DAO::instance().DeviceAdd("456","hello2");
    DeviceInfoList list;
    DAO::instance().DeviceList(list);
    for(int i = 0; i < list.size(); i++)
    {
        qDebug() << "serialNo=" << list[i].serialNo << " name=" << list[i].name;
    }
    QSqlError err = DAO::instance().DeviceUpdate("123","jjjj");
    if(err.isValid()){
        qDebug()<<"update=" << err.text();
    }
    err = DAO::instance().DeviceRemove("123");
    err = DAO::instance().DeviceRemove("456");
    if(err.isValid()){
        qDebug()<<"remove="<<err.text();
    }
}
void MainWnd::Start()
{
    StartReceiver();
    //系统定时器.用来监测设备是否在线掉线
    this->startTimer(1000);
}
MainWnd::MainWnd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWnd),
    m_cur_dev_id(""),
    watcher(NULL)
{
    qRegisterMetaType<SessMessage>("SessMessage");
    ui->setupUi(this);

    if(this->Init())
    {
        this->Start();
    }



}

MainWnd::~MainWnd()
{
    delete ui;
}
void MainWnd::reloadDeviceList2()
{
    QList<Device*> devices;
    ui->treeWidget2->clear();
    ui->treeWidget2->setIconSize(QSize(48,48));
    dvm.ListDevice(devices);
    for(int i = 0; i < devices.size();i++)
    {
        QTreeWidgetItem* item = NULL;
        item = new QTreeWidgetItem(QStringList(devices[i]->name()));

        ui->treeWidget2->addTopLevelItem(item);

        item->setIcon(0,icon_device[1]);

        item->setData(0,Qt::UserRole,devices[i]->id());
        if(i == 0){
            ui->treeWidget2->setCurrentItem(item);
            //m_cur_dev_id = devices[i]->id();
        }
    }
}
//加载左侧设备列表.
void MainWnd::reloadDeviceList()
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
        if(i == 0){
            ui->treeWidget->setCurrentItem(item);
            m_cur_dev_id = devices[i]->id();
            changeDevice(m_cur_dev_id);

        }
    }
}
#include <QSignalMapper>
QVector<int> MainWnd::GetSelectChannel()
{
    QVector<int> channels;
    for(int i = 0; i < rbChanList.size(); i++)
    {
        if(rbChanList[i]->isChecked()){
            channels.push_back(i+1);

        }
    }

    return channels;
}
void MainWnd::SelectAll(bool en)
{
    for(int i= 0; i< rbChanList.size();i++)
    {
        rbChanList[i]->setChecked(en);
    }
}
void MainWnd::loadChannels()
{
    ui->rb1->setChecked(true);
    rbChanList.push_back(ui->rb1);
    rbChanList.push_back(ui->rb2);
    rbChanList.push_back(ui->rb3);
    rbChanList.push_back(ui->rb4);
    rbChanList.push_back(ui->rb5);
    rbChanList.push_back(ui->rb6);
    rbChanList.push_back(ui->rb7);
    rbChanList.push_back(ui->rb8);
    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i =0 ;i < rbChanList.size(); i++)
    {
        connect(rbChanList[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(rbChanList[i], i);

    }
    connect(signalMapper, SIGNAL(mapped(int)),
                this, SLOT(chan_click(int)));

}

void MainWnd::chan_click(int chan)
{
    qDebug() << "chan " << chan << " clicked";
}
//设备相关的UI初始化.
void MainWnd::loadDeviceUI()
{
    //设备状态.icon加载
    icon_device[0].addFile(":image/device.png");
    icon_device[1].addFile(":image/offline.png");
    icon_channel.addFile(":image/channel.png");
    icon_dir.addFile(":image/dir.png");
    icon_file.addFile(":image/channel.png");
    reloadDeviceList();
    reloadDeviceList2();
}

void MainWnd::simData()
{
    for(int i = 0; i < 8; i++)
    {
        devices->DisplayWeight(i,get_random_number(),0,0);
        Sleep(i);

    }
}
bool MainWnd::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == ui->widgetTitle) {
            on_btnMenu_Max_clicked();
            return true;
        }
    }else if (event->type() == QEvent::Resize) {
        if(devices!=NULL)
            devices->Resize();
        return true;
    }

    return QWidget::eventFilter(watched, event);
}
void MainWnd::onWaveShow(int addr, bool zoom)
{
    //放大设备波形.
    //1加载当前设备编号，当前波形历史数据,写入波形控件.
    if(!zoom){
        return;
    }
    QString id;
    if(!GetCurrentDeviceId(id)){
        return;
    }
    //devices->clearAll();
//    QQueue<SensorData>* data = dvm.GetDevice(id)->GetHistoryData(addr);
//    if(data==NULL)return;
//    devices->DisplayDataQueue(addr,*data);
}
void MainWnd::mousePress(QMouseEvent* event)
{
   if(event->button() == Qt::LeftButton)
   {
       rubberOrigin = event->pos();
       rubberBand->setGeometry(QRect(rubberOrigin, QSize()));
       rubberBand->show();
   }
}
void MainWnd::mouseDoubleClick(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        RestoreWave();
    }
}

void MainWnd::mouseRelease(QMouseEvent* event)
{
    Q_UNUSED(event);
      if (rubberBand->isVisible())
      {
          if(rubberBand->rect().width() < 2)
          {
              rubberBand->hide();
              return;
          }

          const QRect zoomRect = rubberBand->geometry();
          int xp1, yp1, xp2, yp2;
          zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
          double x1 = ui->plot3->xAxis->pixelToCoord(xp1);
          double x2 = ui->plot3->xAxis->pixelToCoord(xp2);
          double y1 = ui->plot3->yAxis->pixelToCoord(yp1);
          double y2 = ui->plot3->yAxis->pixelToCoord(yp2);

          ui->plot3->xAxis->setRange(x1, x2);
          ui->plot3->yAxis->setRange(y1, y2);

          rubberBand->hide();
          ui->plot3->replot();
      }

}
void MainWnd::RestoreWave()
{
    ui->plot3->rescaleAxes();
    ui->plot3->replot();
}
void MainWnd::myMoveEvent(QMouseEvent *event) {
    int x_pos = event->pos().x();
    int y_pos = event->pos().y();
    if(rubberBand->isVisible())
       rubberBand->setGeometry(QRect(rubberOrigin, event->pos()).normalized());
    if(!ui->chkMeasure->isChecked()){
        return;
    }
//这里如果是float x_val 和 y_val 会导致精度损失很大，导致显示跳.这个问题找了一整天.
    double x_val = ui->plot3->xAxis->pixelToCoord(x_pos);
    double y_val = ui->plot3->yAxis->pixelToCoord(y_pos);
    qDebug() << "x_pos" << x_pos << "x_value=" << qint64(x_val);
    m_xTracer->updatePosition(x_val, y_val);


    auto iter = ui->plot3->graph(0)->data()->findBegin(x_val);
    double value1 = iter->mainValue();

    m_tracer1->updatePosition(x_val, value1);

    m_lineTracer->updatePosition(x_val, y_val);

    ui->plot3->replot();//曲线重绘
}
void MainWnd::initDeviceChannels()
{
    wave = new HistWaveWidget(ui->plot3);

    rubberBand = new QRubberBand(QRubberBand::Rectangle, ui->plot3);

    connect(ui->plot3, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(myMoveEvent(QMouseEvent*)));
    connect(ui->plot3, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
    connect(ui->plot3, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));
    connect(ui->plot3, SIGNAL(mouseDoubleClick(QMouseEvent*)), this, SLOT(mouseDoubleClick(QMouseEvent*)));



    m_xTracer       = new MyTracer(ui->plot3, MyTracer::XAxisTracer, ui->plot3);//x轴
    m_tracer1       = new MyTracer(ui->plot3, MyTracer::DataTracer, ui->plot3);
    //m_tracer2       = new MyTracer(ui->plot3, MyTracer::DataTracer, ui->plot3);
    m_lineTracer    = new MyTracer(ui->plot3, MyTracer::CrossLine, ui->plot3);//直线

    //connect(ui->plot3, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(myMouseMoveEvent(QMouseEvent*)));
    ui->dteFrom->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->dteTo->setDateTime(QDateTime::currentDateTime());
    on_cbxTimeSpan_currentIndexChanged(0);
    devices = new MyDevices(9,ui->gbDevices);
    devices->SetTimeRange(Config::instance().m_rt_wave_min*60);
    devices->SetMaxSampleNum(50);
    devices->SetDeviceNum(1,8);
    connect(devices,SIGNAL(onChannelConfig(int)),this,SLOT(onChannelClick(int)));
    connect(devices,SIGNAL(onPlayClick(int,bool)),this,SLOT(onPlayClick(int,bool)));
    connect(devices,SIGNAL(onWaveShow(int,bool)) ,this,SLOT(onWaveShow(int,bool)));

}
void MainWnd::initUI()
{
    qDebug() << "MainWnd id=" << QThread::currentThreadId();
    this->setProperty("form", true);
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    IconHelper::Instance()->setIcon(ui->labIco, QChar(0xf099), 30);
    IconHelper::Instance()->setIcon(ui->btnMenu_Min, QChar(0xf068));
    IconHelper::Instance()->setIcon(ui->btnMenu_Max, QChar(0xf067));
    IconHelper::Instance()->setIcon(ui->btnMenu_Close, QChar(0xf00d));

    //ui->widgetMenu->setVisible(false);
    ui->widgetTitle->installEventFilter(this);
    ui->widgetTitle->setProperty("form", "title");
    ui->widgetTop->setProperty("nav", "top");
    //ui->labTitle->setText(tr("WeiZhi Tech"));
    ui->labTitle->setFont(QFont("Microsoft Yahei", 16));
    this->setWindowTitle(ui->labTitle->text());

    //单独设置指示器大小
    int addWidth = 20;
    int addHeight = 10;
    int rbtnWidth = 15;
    int ckWidth = 13;
    int scrWidth = 12;
    int borderWidth = 3;

    QStringList qss;
    qss.append(QString("QComboBox::drop-down,QDateEdit::drop-down,QTimeEdit::drop-down,QDateTimeEdit::drop-down{width:%1px;}").arg(addWidth));
    qss.append(QString("QComboBox::down-arrow,QDateEdit[calendarPopup=\"true\"]::down-arrow,QTimeEdit[calendarPopup=\"true\"]::down-arrow,"
                       "QDateTimeEdit[calendarPopup=\"true\"]::down-arrow{width:%1px;height:%1px;right:2px;}").arg(addHeight));
    qss.append(QString("QRadioButton::indicator{width:%1px;height:%1px;}").arg(rbtnWidth));
    qss.append(QString("QCheckBox::indicator,QGroupBox::indicator,QTreeWidget::indicator,QListWidget::indicator{width:%1px;height:%1px;}").arg(ckWidth));
    qss.append(QString("QScrollBar:horizontal{min-height:%1px;border-radius:%2px;}QScrollBar::handle:horizontal{border-radius:%2px;}"
                       "QScrollBar:vertical{min-width:%1px;border-radius:%2px;}QScrollBar::handle:vertical{border-radius:%2px;}").arg(scrWidth).arg(scrWidth / 2));
    qss.append(QString("QWidget#widget_top>QToolButton:pressed,QWidget#widget_top>QToolButton:hover,"
                       "QWidget#widget_top>QToolButton:checked,QWidget#widget_top>QLabel:hover{"
                       "border-width:0px 0px %1px 0px;}").arg(borderWidth));
    qss.append(QString("QWidget#widgetleft>QPushButton:checked,QWidget#widgetleft>QToolButton:checked,"
                       "QWidget#widgetleft>QPushButton:pressed,QWidget#widgetleft>QToolButton:pressed{"
                       "border-width:0px 0px 0px %1px;}").arg(borderWidth));
    this->setStyleSheet(qss.join(""));


    QSize icoSize(32, 32);
    int icoWidth = 85;

    //设置顶部导航按钮
    QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns) {
        btn->setIconSize(icoSize);
        btn->setMinimumWidth(icoWidth);
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(buttonClick()));
    }



    ui->gbDevices->installEventFilter(this);

//上下文菜单初始化
    ui->treeWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    menu=new QMenu(this);
    menu2=new QMenu(this);
    QIcon setting= QIcon(":image/setting.png");
    QAction* action = new QAction(setting,QString::fromLocal8Bit(" 配置参数"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_menu_click(bool)));

    action = new QAction(QString::fromLocal8Bit("标定重量"),this);
    menu->addAction(action);

    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_write_menu_click(bool)));

    action = new QAction(QString::fromLocal8Bit("复位设备"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_reset_menu_click(bool)));

    action = new QAction(QString::fromLocal8Bit("删除设备"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_remove_device_click(bool)));

    action = new QAction(QString::fromLocal8Bit("修改设备"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_modify_menu_click(bool)));

    action = new QAction(QString::fromLocal8Bit("升级设备"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_update_menu_click(bool)));

    action = new QAction(QString::fromLocal8Bit("复位计数器"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_reset_count_click(bool)));
    action = new QAction(QString::fromLocal8Bit("获取计数器"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_get_count_click(bool)));


//GetCount
    action = new QAction(QString::fromLocal8Bit("添加设备"),this);
    menu2->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_add_device_click(bool)));

    ui->btnMain->click();
    on_btnMenu_Max_clicked();
 //加载设备状态.

    initDeviceChannels();
    loadChannels();
    loadSysConfig();
    loadDeviceUI();
}
void MainWnd::loadSysConfig()
{
    ui->edtPort->setText(QString("%1").arg(Config::instance().m_local_port));
    ui->sbWaveMin->setValue(Config::instance().m_rt_wave_min);
    ui->sbSaveInt->setValue(Config::instance().m_save_intS);
    ui->cbUseSysTime->setChecked(Config::instance().m_use_sys_time);
    ui->edtHost->setText(Config::instance().m_host_name);
}
static bool loading = false;
//加载状态文件.
void MainWnd::loadStateFile()
{
    loading = true;
    ui->edtDataDir->setText(Config::instance().m_data_dir);
    ui->cbxHost->clear();
    ui->cbxTestNo->clear();
    ui->listFiles->clear();
    if(!StateManager::instance().parse(Config::instance().m_data_dir+QStringLiteral("/TW压力测试状态表.xlsm")))
    {
        qDebug() << "loadStateFile failed";
        QMessageBox::information(this,"info",QStringLiteral("找不到TW压力测试状态表.xlsm"));
        this->AddLog("loadStateFile failed");
        return;
    }
#if 1
    CellTestHost& host = StateManager::instance().GetState();
    ui->cbxHost->clear();
    QMap<QString,CellTestOrderList>::const_iterator i = host.constBegin();
    while (i != host.constEnd()) {
        qDebug() << i.key();
        ui->cbxHost->addItem(i.key());
        ++i;
    }

#endif
    loading = false;
    ui->cbxHost->setCurrentText(Config::instance().m_host_name);

}
void MainWnd::buttonClick()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->objectName();

    QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns) {
        if (btn == b) {
            btn->setChecked(true);
        } else {
            btn->setChecked(false);
        }
    }

    if (name == "btnMain") {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "btnConfig") {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "btnData") {
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "btnReport") {
        ui->stackedWidget->setCurrentIndex(3);
        loadStateFile();
    }else if (name == "btnHelp") {
        ui->stackedWidget->setCurrentIndex(4);
    } else if (name == "btnExit") {
        this->close();
    }
}

void MainWnd::on_btnMenu_Min_clicked()
{
    showMinimized();
}

void MainWnd::on_btnMenu_Max_clicked()
{
    static bool max = false;
    static QRect location = this->geometry();

    if (max) {
        this->setGeometry(location);
    } else {
        location = this->geometry();
        qDebug() <<qApp->desktop()->availableGeometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
    }

    this->setProperty("canMove", max);
    max = !max;
}

void MainWnd::on_btnMenu_Close_clicked()
{
    close();
}

void MainWnd::on_btnExit_clicked()
{

}

void MainWnd::on_btnShou_clicked()
{
    static bool hide = false;
    if(!hide){
        ui->treeWidget->hide();
        ui->btnShou->setIcon(QIcon(":/image/fang.png"));
    }else{
        ui->btnShou->setIcon(QIcon(":/image/shou.png"));

        ui->treeWidget->show();
    }
    hide =!hide;
}

void MainWnd::closeEvent(QCloseEvent *event)
{
    int result = myHelper::ShowMessageBoxQuesion(QString::fromLocal8Bit("确定离开?"));
       if (result == 1) {
           event->accept();
       } else {
           event->ignore();
       }


}
//切换后触发.
void MainWnd::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current==NULL){
        return;
    }
    m_cur_dev_id = current->data(0,Qt::UserRole).toString();

    qDebug() << "item change to" << m_cur_dev_id;
}

void MainWnd::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
     m_cur_dev_id = item->data(0,Qt::UserRole).toString();

     changeDevice(m_cur_dev_id);
}

void MainWnd::on_sbWaveMin_valueChanged(int arg1)
{
    Config::instance().SetRtWaveMin(arg1);
    devices->SetTimeRange(Config::instance().m_rt_wave_min*60);
}

void MainWnd::on_edtPort_textChanged(const QString &arg1)
{
    bool ok = false;
    int port = arg1.toInt(&ok);
    if(!ok){
        return;
    }
    Config::instance().SetLocalPort(port);
}

void MainWnd::on_cbUseSysTime_stateChanged(int arg1)
{
    Config::instance().SetUseSysTime(arg1!=0);
    qDebug() << arg1;
}


void MainWnd::handleLoadWaveFinished()
{
    wave->DisplayData(m_ddl);
    ui->btnQuery->setEnabled(true);
    ui->btnQuery->setText(QStringLiteral("查询"));
}
//在后台线程进行数据查询
bool MainWnd::LoadWave(QString id, QVector<int> chan, qint64 from, qint64 to)
{
    m_ddl.clear();
    for(int i = 0; i < chan.size(); i++)
    {
        DeviceDataList dll;
        m_ddl[chan[i]] = dll;
    }
    QSqlError err = DAO::instance().DeviceDataQuery(id,from,to, m_ddl);
    return !err.isValid();
}

void MainWnd::on_btnQuery_clicked()
{
    QString id;
    if(!GetCurrentDeviceId2(id)){
        return;
    }
    QVector<int> chans = GetSelectChannel();
    if(chans.size() == 0)
    {
        wave->Clear();
        return;
    }

    qint64 from = ui->dteFrom->dateTime().toMSecsSinceEpoch()/1000;
    qint64 to = ui->dteTo->dateTime().toMSecsSinceEpoch()/1000;

    if(watcher==NULL){
        watcher = new QFutureWatcher<bool>(this);
        connect(watcher, SIGNAL(finished()), this, SLOT(handleLoadWaveFinished()));
    }


    const QFuture<bool> future = QtConcurrent::run(this,&MainWnd::LoadWave, id,chans,from,to);
    watcher->setFuture(future);
    ui->btnQuery->setText(QStringLiteral("查询中..."));
    ui->btnQuery->setEnabled(false);

}

void MainWnd::on_btnShou2_clicked()
{
    static bool hide = false;
    if(!hide){
        ui->treeWidget2->hide();
        ui->btnShou2->setIcon(QIcon(":/image/fang.png"));
    }else{
        ui->btnShou2->setIcon(QIcon(":/image/shou.png"));

        ui->treeWidget2->show();
    }
    hide =!hide;
}

void MainWnd::on_dteFrom_dateChanged(const QDate &date)
{

}
static qint64 time_span_list[]={
        10*60, //10分
        20*60,
        1*3600,
    2*3600,
    5*3600,
    10*3600
        };
void MainWnd::on_cbxTimeSpan_currentIndexChanged(int index)
{
    ui->dteTo->setDateTime(QDateTime::currentDateTime());
    ui->dteFrom->setDateTime(QDateTime::fromMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch()-time_span_list[index]*1000));
    //time_span_list
}

void MainWnd::on_treeWidget2_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    QString id;
    if(!GetCurrentDeviceId2(id)){
      return;
    }
//    int chan = GetSelectChannel();
//    Device* dev = dvm.GetDevice(id);
//    if(dev!=NULL){
//        wave->SetTitle(QString(QStringLiteral("%1:通道%2")).arg(dev->name()).arg(chan));
//    }else{
//        wave->SetTitle(QString(QStringLiteral("通道%1")).arg(chan));
//    }
//    wave->Clear();

}

void MainWnd::on_btnRestore_clicked()
{

}

void MainWnd::on_chkMeasure_clicked(bool checked)
{
    qDebug() << checked;

     m_xTracer->setVisible(checked);
     m_tracer1->setVisible(checked);
     m_lineTracer->setVisible(checked);
     ui->plot3->replot();
}

void MainWnd::on_sbSaveInt_valueChanged(int arg1)
{
    //采样时间变化.
    Config::instance().SetSaveInt(arg1);
}

void MainWnd::on_chkSelAll_clicked()
{
    SelectAll(ui->chkSelAll->isChecked());
}
#include "asyncexport.h"
#include <QFileDialog>
void MainWnd::on_btnExport_clicked()
{
    QString id;
    if(!GetCurrentDeviceId2(id)){
        return;
    }
    QVector<int> chans = GetSelectChannel();
    if(chans.size() == 0)
    {
        return;
    }

    qint64 from = ui->dteFrom->dateTime().toMSecsSinceEpoch()/1000;
    qint64 to = ui->dteTo->dateTime().toMSecsSinceEpoch()/1000;

    QString fileName = QFileDialog::getSaveFileName(this,QStringLiteral("保存文件"),
                               id+".csv",
                               tr("csv (*.csv)"));
    qDebug() << "Filename=" << fileName;
    AsyncExportManager::instance().AddTask(id,chans,from,to,fileName);

     myHelper::ShowMessageBoxInfo(QStringLiteral("导出完成"));
}
#include <dialogmerge.h>
#include <dialogreport.h>
void MainWnd::on_btnMerge_clicked()
{
    //出现设备选择框
    QString id;
    if(!GetCurrentDeviceId2(id)){
        return;
    }
    QVector<int> chans = GetSelectChannel();
    if(chans.size() == 0)
    {
        return;
    }
    DialogReport dlg;

//    DialogMerge dlg;
//    dlg.SetDevice(id,chans);
    if(QDialog::Rejected == dlg.exec()){
        //点击了取消
        return;
    }

}

void MainWnd::on_btnHelp_clicked()
{

}

void MainWnd::on_cbxHost_currentIndexChanged(const QString &arg1)
{
     if(loading) return;
    QString x = arg1;
   CellTestOrderList& orders =  StateManager::instance().GetOrderList(x);

   ui->cbxTestNo->clear();
   QMap<QString,QVector<CellState>>::const_iterator i = orders.constBegin();
   while (i != orders.constEnd()) {
       //cout << i.key() << ": " << i.value() << endl;
       ui->cbxTestNo->addItem(i.key());
       ++i;
   }

}

void MainWnd::on_cbxTestNo_currentIndexChanged(const QString &arg1)
{
    //列出所有的电芯

    if(loading) return;
   CellTestOrderList& orders =  StateManager::instance().GetOrderList(ui->cbxHost->currentText());
    ui->listFiles->clear();
   if(orders.contains(arg1)){
       for(int i = 0; i < orders[arg1].size(); i++)
       {
           ui->listFiles->addItem(orders[arg1].at(i).CellNo);
       }

   }


}
/**

type MergeChanInfo struct{

    DevId string //设备名称
    DevChan int //通道编号

    CtrlName string //控制柜的名称
    CtrlChan string //控制柜的通道名  这两个结合起来可以得到控制柜的文件名.

    FileType string  //控制柜文件类型.
    FileName string //控制柜的文件名称
    CellNo string //电芯编号

}

func BuildPressReport(
    channels []MergeChanInfo,
    testNo string , //测试的单号
    temp string , //测试的温度
    )

 */
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QString MainWnd::buildReportInput()
{
    CellTestOrderList& orders =  StateManager::instance().GetOrderList(ui->cbxHost->currentText());
    QString order = ui->cbxTestNo->currentText();
    QJsonDocument doc;
    QJsonObject root;
    if(orders.contains(order)){
        QJsonArray arr;


        root["order_no"] = order;
        root["temp"] = "50";
        root["target_file"] = QString(QStringLiteral("%1/%2/%3/%4_%5压力测试表.xlsx"))
                .arg(Config::instance().m_data_dir)
                .arg(ui->cbxHost->currentText())
                .arg(order).arg(order).arg("45");
        root["db"] = QCoreApplication::applicationDirPath()+"/measure.db";

        QVector<CellState> &states =  orders[order];
        for(int i = 0; i <states.size(); i++)
        {
             QJsonObject o;


             o["dev_chan"] = orders[order].at(i).PressDevChan;
             o["dev_name"] = orders[order].at(i).PressDevId;
             o["file_name"] = QString("%1/%2/%3/%4.xlsx")
                     .arg(Config::instance().m_data_dir)
                     .arg(ui->cbxHost->currentText())
                     .arg(order)
                     .arg(states[i].CellNo);
             o["cell_no"] =states[i].CellNo;

            arr.push_back(o);
        }
        root["chan_array"] = arr;


    }
    doc.setObject(root);
    return doc.toJson();
}

#include "gotypes.h"
void MainWnd::on_btnExecReport_clicked()
{
    //启动导出，在某个路径下查找电芯文件的路径
    //默认规则是 电芯文件放到 工单号的目录下.
    //选择工单后，就到工单目录下去查找该工单的所有文件
    //然后对每个文件调用保存工具
    if(pFnBuildReport){
        QGoString str(buildReportInput());

//        std::string input = buildReportInput().toStdString();
//        GoString gstr;
//        gstr.p= input.c_str();
//        gstr.n = (ptrdiff_t)strlen(gstr.p) ;
        char* res=nullptr;
 //       qDebug() << "data=>" << gstr.n;
        int code = pFnBuildReport(str.toGoString(),&res);

        qDebug() << "code=" << code << "result=" << res;
    }

}

void MainWnd::on_btnSelFile_clicked()
{

    QString path  = QFileDialog::getExistingDirectory(this);
    if(path.length()< 2){
        return;
    }
    ui->edtDataDir->setText(path);
    Config::instance().SetDataDir(path);
    loadStateFile();
}

void MainWnd::on_edtHost_textChanged(const QString &arg1)
{
    Config::instance().SetHostName(arg1);
}
