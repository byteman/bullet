//#pragma execution_character_set("utf-8")
//#ifdef _MSC_BUILD
//#pragma execution_character_set("utf-8")
//#endif
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
#include "gotypes.h"
#include "asyncexport.h"
#include <QInputDialog>
#include "usbimport.h"
#include "Logger.h"
#define MAX_CHAN_NR 12
#define LISTEN_PORT 8881
void MainWnd::AddLog(QString msg)
{
    ui->txtLog->append(msg);
}
#include "nettools.h"
void MainWnd::outputVer()
{
    AddLog(QString("Ver-%1-%2").arg("1.0.6").arg(__DATE__));
}
void MainWnd::StartServer()
{
     QString app = utils::GetWorkDir()+"/"+Config::instance().m_report_name;
#if 0

    utils::StartProcess(app);
#else
 if(proc==NULL){
     proc = new QProcess(this);
 }


  qDebug() << "start " << app;
  proc->start(app);
  if(!proc->waitForStarted()){
      qDebug() << "wait for start failed";
      return;
  }
  QString strResult = QString::fromLocal8Bit(proc->readAllStandardOutput());
  qDebug() << "start result" << strResult;

#endif
}
void MainWnd::StartReceiver()
{
    srv = new GPServer();

    if(!srv->start(Config::instance().m_local_port)){
        AddLog(QStringLiteral("服务启动失败,检查端口8881是否被占用!!"));
    }else{
        AddLog(QStringLiteral("服务启动成功"));
    }
//    QStringList ip = NetTools::get_local_ip();
//    for(int i = 0; i< ip.size();i++)
//    {
//        AddLog(QString("ip[%1]=%2").arg(i+1).arg(ip.at(i)));
//    }
    connect(srv,SIGNAL(Message(SessMessage)),this,SLOT(Message(SessMessage)));
    connect(srv,SIGNAL(Message(SessMessage)),&dvm
            ,SLOT(Message(SessMessage)));
    //connect(&ping,SIGNAL(onReply(QString)),this,SLOT(onReply(QString)));


}
void MainWnd::onReply(QString msg)
{
    AddLog(msg);
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
typedef GoInt (*funcGetOrderState)(GoString p0,char** res);
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
static  funcGetOrderState pFnGetOrderState = NULL;
void initGoLibrary()
{
     LOG_DEBUG("LoadLibrary report.dll");
     HINSTANCE hInstance = LoadLibraryA("report.dll");
     LOG_DEBUG() << "LoadLibrary result=" << hInstance;
     if (NULL != hInstance)
     {
         qDebug() <<"LoadLib succ";
         pFnBuildReport = (funcBuildReport)GetProcAddress(hInstance,"GenerateReport");
         if (pFnBuildReport)
         {
             qDebug() << "Find GenerateReport";
         }
         pFnGetOrderState  = (funcGetOrderState)GetProcAddress(hInstance,"GetOrdersState");
         if (pFnGetOrderState)
         {
             qDebug() << "Find GetOrdersState";
         }
     }else{
         qDebug() <<"load report.dll failed";
     }

}
bool MainWnd::Init()
{
    outputVer();
    LOG_DEBUG("initGoLibrary");
    initGoLibrary();

    //首先初始化数据管理模块.
    QString dbDir = utils::GetWorkDir()+"/data/";
    LOG_DEBUG() << "MkMutiDir->" <<dbDir;
    utils::MkMutiDir(dbDir);
    LOG_DEBUG("load config.db");
    QSqlError err =  DAO::instance().Init(
                utils::GetWorkDir()+"/config.db",
                dbDir);
    if(err.isValid()){
        //初始化失败
        AddLog(err.text());
        myHelper::ShowMessageBoxError(err.text());
        return false;
    }
    LOG_DEBUG("Config init");
    Config::instance().Init();
    //首先初始化设备管理器.
    LOG_DEBUG("InitDvm init");
    if(!InitDvm()){
        AddLog(err.text());
        myHelper::ShowMessageBoxError(err.text());
        return false;
    }
    //初始化UI相关.
    LOG_DEBUG("initUI");
    this->initUI();
    LOG_DEBUG("start export");
    StartServer();
    LOG_DEBUG("init complete");

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
    myHelper::ShowMessageBoxInfo(QStringLiteral("复位设备成功"));

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
    //cfg.chanName = QString(QStringLiteral("通道%1")).arg(addr+1);
    QString devname="";
    if(!GetCurrentDeviceName(devname)){
       return;
    }
    dvm.GetDeviceChan(m_cur_dev_id,addr,cfg);
    dlg.SetChanConfig(m_cur_dev_id,devname,addr,cfg);
    int result = dlg.exec();
    if(result == QDialog::Accepted){
        dlg.GetChanConfig(cfg);
        dvm.UpdateDeviceChan(m_cur_dev_id,addr,cfg);
        devices->SetChanSetting(addr,cfg);
        Device* dev = dvm.GetDevice(m_cur_dev_id);
        if(cfg.chanName.length() > 0){
            //已经配置了名字，用新的名字
            if(dev!=NULL){
                devices->SetTitle(addr,QString(QStringLiteral("%1:%2")).arg(dev->name()).arg(cfg.chanName));
            }else{
                devices->SetTitle(addr,QString(QStringLiteral("通道%1")).arg(cfg.chanName));
            }
        }else{
            if(dev!=NULL){
                devices->SetTitle(addr,QString(QStringLiteral("%1:通道%2")).arg(dev->name()).arg(addr));
            }else{
                devices->SetTitle(addr,QString(QStringLiteral("通道%1")).arg(addr));
            }
        }
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
    if(!this->CheckPassWord())return;
    if(dvm.DeviceCount() >=12 ){
         myHelper::ShowMessageBoxError(QStringLiteral("最多添加12个设备"));
         return;
    }
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
bool MainWnd::GetCurrentDeviceName(QString& name)
{
    QTreeWidgetItem* item =  ui->treeWidget->currentItem();
    if(item==NULL){
         return false;
    }
    name = item->text(0);
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
bool MainWnd::GetCurrentDeviceId2Name(QString& name)
{
    QTreeWidgetItem* item =  ui->treeWidget2->currentItem();
    if(item==NULL){
         return false;
    }
    name = item->text(0);
    return true;
}
//删除一个设备.
void MainWnd::on_remove_device_click(bool)
{
    if(!this->CheckPassWord())return;
     QString id;
     if(!GetCurrentDeviceId(id))
     {
         return;
     }
     if(dvm.RemoveDevice(id)){
         myHelper::ShowMessageBoxInfo(QStringLiteral("删除设备成功"));
     }else{
         myHelper::ShowMessageBoxInfo(QStringLiteral("删除设备失败"));
     }
     reloadDeviceList();
      reloadDeviceList2();

}
#include "dialogclearup.h"
bool MainWnd::CheckPassWord()
{
    if("123456" != QInputDialog::getText(this,
                          QStringLiteral("权限验证"),
                          QStringLiteral("请输入密码")))
    {
        return false;
    }
    return true;
}
void MainWnd::on_start_menu_click(bool)
{
    QString id;
    if(!GetCurrentDeviceId(id))
    {
        qDebug()<<"Can not GetCurrentDeviceId";
        return;
    }
    if(devices!=NULL){

        QVector<int> chans = devices->GetSelectChan();
        for(int i = 0; i < chans.size(); i++)
        {
            dvm.ControlDeviceChan(id,chans[i],false);
        }
        devices->SetSelectRecState(false);
    }

}
//下载u盘数据
#include "mainwindow.h"
void MainWnd::on_stop_menu_click(bool)
{
    QString id;
    if(!GetCurrentDeviceId(id)){
        qDebug()<<"Can not GetCurrentDeviceName";
        return;
    }
    Device* dev = dvm.GetDevice(id);
    if(dev==NULL){
        return;
    }

    QString host;
    if(!dev->GetHostAddr(host)){
        return;
    }
    if(ftp!=NULL){
        delete ftp;
        ftp = NULL;
    }
    if(ftp==NULL){
        ftp = new MainWindow();
    }

    ftp->setHost(host);

    //ftp.setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
    //ftp.setWindowModality(Qt::ApplicationModal);
    ftp->setAttribute(Qt::WA_ShowModal, true);
    ftp->setWindowFlags(ftp->windowFlags() | Qt::WindowStaysOnTopHint);

    ftp->showNormal();


}
void MainWnd::on_clear_history_menu_click(bool)
{
    QString id;

    QString name;
     if(!CheckPassWord() )return;
    if(!GetCurrentDeviceName(name)){
        qDebug()<<"Can not GetCurrentDeviceName";
        return;
    }
    if(!IsOk(QStringLiteral("注意"),QString(QStringLiteral("是否清空%1号设备未发送完成的历史数据")).arg(name))){
        return;
    }

    if(!GetCurrentDeviceId(id))
    {
        qDebug()<<"Can not GetCurrentDeviceId";
        return;
    }
    dvm.ClearHistory(id);
}
bool MainWnd::IsOk(QString title,QString message)
{

    int result = myHelper::ShowMessageBoxQuesion(message);
    if (result == 1) {
       return true;
    } else {
       return false;
    }


//    QMessageBox msg(this);
//    msg.setWindowTitle(title);
//    msg.setText(message);
//   // msg.setIcon(QMessageBox::information);
//    msg.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
//    return msg.exec() == QMessageBox::Ok;
}
void MainWnd::on_clearup_menu_click(bool)
{
    if(!CheckPassWord() )return;
    QString id;
    if(!GetCurrentDeviceId(id))
    {
        qDebug()<<"Can not GetCurrentDeviceId";
        return;
    }
    DialogClearUp dlg;
    dlg.SetSerialNo(id);
    dlg.exec();
}

#include "dialogupdate.h"
void MainWnd::on_update_menu_click(bool)
{
    if(!this->CheckPassWord())return;
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
    qDebug() << "ip=" << ip;
    DialogUpdate dlg;


    dlg.SetServerHost(ip);
    dlg.exec();
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
    if(!CheckPassWord() )return;
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
//构造命令
QString MainWnd::queryOrderState()
{
    if(pFnGetOrderState!=NULL){
        QGoString str(ui->cbxHost->currentText());
        char* res=NULL;
        pFnGetOrderState(str.toGoString(),&res);
        return res;
    }else{
        ajax.get("http://localhost:9527/GetOrdersState",this,SLOT(on_query_report_response(AjaxResponse)));
        return "";
    }
}
#include <QTextCodec>
void MainWnd::refreshOrderState(QString res)
{

    QJsonDocument doc =  QJsonDocument::fromJson(res.toUtf8());
    QJsonObject o = doc.object();
    for(int i=0; i < ui->orderList->topLevelItemCount();i++)
    {
       QTreeWidgetItem* item =  ui->orderList->topLevelItem(i);
       if(item==NULL){
           continue;
       }
       QString order = item->text(0);
       if(o.contains(order)){
           //如果包含了订单的状态.
           QJsonObject o2 = o[order].toObject();
           int state = o2["state"].toInt();
           QString msg = "";
           switch(state)
           {
               case 0:
                    msg = QStringLiteral("未开始");
                    break;
               case 1:
                   msg = o2["message"].toString();

                   break;
               case 2:
                   msg = QStringLiteral("生成报告成功");
                   break;
               case 3:
                   msg = o2["message"].toString();
                   break;
           }

           item->setText(5,msg);
       }

    }

}
//定时查询某台主机上所有订单的状态。
void MainWnd::updateOrderState()
{
        QString res = queryOrderState();
        if(res.length() < 1) return;
        refreshOrderState(res);
        qDebug() << "result=" << res;


}
//1s 定时监测设备是否在线.
void MainWnd::timerEvent(QTimerEvent *)
{
    QList<Device*> devices;
    dvm.ListDevice(devices);
    static QString curId;
    bool find = this->GetCurrentDeviceId(curId);
    for(int i = 0; i < devices.size();i++)
    {
        QTreeWidgetItem* item = findItemById(devices[i]->id());
        if(item!=NULL)
        {
            if(devices[i]->online())
                item->setIcon(0,icon_device[0]);
            else
                item->setIcon(0,icon_device[1]);
            if(find && curId==devices[i]->id()){
                this->devices->SetOnline(devices[i]->online());
            }

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

    if(bQueryOrderState){

       updateOrderState();

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
    if(!bFirst)
        devices->ClearDisplay();
    else{
        bFirst = false;
    }
    for(int i = 1; i <= MAX_CHAN_NR; i++)
    {
        DeviceChnConfig cfg;
        //从数据库中获取参数的状态.
        //修改显示的配置
        if(dvm.GetDeviceChan(dev_id,i,cfg)){
            devices->SetChanSetting(i,cfg);
            Device* dev = dvm.GetDevice(dev_id);
            if(cfg.chanName.length() > 0){
                //已经配置了名字，用新的名字
                if(dev!=NULL){
                    devices->SetTitle(i,QString(QStringLiteral("%1:%2")).arg(dev->name()).arg(cfg.chanName));
                }else{
                    devices->SetTitle(i,QString(QStringLiteral("通道%1")).arg(cfg.chanName));
                }
            }else{
                if(dev!=NULL){
                    devices->SetTitle(i,QString(QStringLiteral("%1:通道%2")).arg(dev->name()).arg(i));
                }else{
                    devices->SetTitle(i,QString(QStringLiteral("通道%1")).arg(i));
                }
            }


        }
        //修改设备对象的配置.
        dvm.SetChanConfig(dev_id,i,cfg);
    }
    ui->treeWidget2->setCurrentItem(findItemById2(dev_id));


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
    bFirst(true),
    ftp(NULL),
    proc(NULL),
    bQueryOrderState(false),
    watcher(NULL)
{
    qRegisterMetaType<SessMessage>("SessMessage");
    LOG_DEBUG() << "ui->setupUi";
    ui->setupUi(this);
    LOG_DEBUG() << "Init begin";
    if(this->Init())
    {
        LOG_DEBUG() << "Init ok";
        this->Start();
    }

    qDebug() << "MainWnd complete";


}

MainWnd::~MainWnd()
{
    delete ui;
    if(proc!=NULL){
        proc->kill();
    }
    utils::KillProcess(Config::instance().m_report_name);
}
void MainWnd::reloadDeviceList2()
{
    QList<Device*> devices;
    ui->treeWidget2->clear();
    ui->treeWidget2->setSortingEnabled(true);
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
    ui->treeWidget2->sortItems(0,Qt::SortOrder::AscendingOrder);
}
//加载左侧设备列表.
void MainWnd::reloadDeviceList()
{
    QList<Device*> devices;
    ui->treeWidget->clear();
    ui->treeWidget->setSortingEnabled(true);

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
    ui->treeWidget->sortItems(0,Qt::SortOrder::AscendingOrder);
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
    if(en){
        updateTime();
    }
}
void MainWnd::loadChannels()
{
    //ui->rb1->setChecked(true);
    rbChanList.push_back(ui->rb1);
    rbChanList.push_back(ui->rb2);
    rbChanList.push_back(ui->rb3);
    rbChanList.push_back(ui->rb4);
    rbChanList.push_back(ui->rb5);
    rbChanList.push_back(ui->rb6);
    rbChanList.push_back(ui->rb7);
    rbChanList.push_back(ui->rb8);
    rbChanList.push_back(ui->rb9);
    rbChanList.push_back(ui->rb10);
    rbChanList.push_back(ui->rb11);
    rbChanList.push_back(ui->rb12);

    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i =0 ;i < rbChanList.size(); i++)
    {
        connect(rbChanList[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(rbChanList[i], i);

    }
    connect(signalMapper, SIGNAL(mapped(int)),
                this, SLOT(chan_click(int)));

}
void MainWnd::updateTime()
{
   QString id;
   if(!GetCurrentDeviceId2(id)){
        return;
   }

   QVector<int> chans =  GetSelectChannel();
   if(chans.size() == 0){
       ui->dteTo->setDateTime(QDateTime::currentDateTime());
       ui->dteFrom->setDateTime(QDateTime::currentDateTime());
       return;
   }
   qint64 start = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;

   qint64 stop = 0;
   for(int i = 0; i < chans.size(); i++)
   {
        DeviceChnConfig cfg;
        DAO::instance().DeviceChannalGet(id,chans[i],cfg);
        qDebug() << "chan" << chans[i] << "start:"<<cfg.startTime <<"end:" << cfg.endTime;

        if(cfg.startTime < start && cfg.startTime > 0){
            start = cfg.startTime;
        }
        if(cfg.endTime > stop && cfg.endTime > 0){
            stop = cfg.endTime;
        }
        qDebug() <<"start:"<<start <<"end:"<<stop;
   }
   if(start==stop)return;
   if(start > stop) return;
   if(start == 0) start = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
   if(stop == 0) stop = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
    qDebug() <<"start:"<<start <<"end:"<<stop;

   ui->dteTo->setDateTime(QDateTime::fromMSecsSinceEpoch(stop*1000));
   ui->dteFrom->setDateTime(QDateTime::fromMSecsSinceEpoch(start*1000));



}
void MainWnd::chan_click(int chan)
{
    qDebug() << "chan " << chan << " clicked";
    //1.更新波形通道的显示
    //2.更新是否显示跟踪线
    bool en = rbChanList[chan]->isChecked();
    wave->ShowChan(chan,en);
    if(ui->chkMeasure->isChecked()){
        m_tracer->Show(chan,en);
    }else{
        m_tracer->Show(chan,false);
    }
    updateTime();
    ui->plot3->replot();
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
   // devices->clearAll();
//    QQueue<SensorData>* data = dvm.GetDevice(id)->GetHistoryData(addr);
//    if(data==NULL)return;
//    devices->DisplayDataQueue(addr-1,*data);
}
void MainWnd::mousePress(QMouseEvent* event)
{
   if(event->button() == Qt::LeftButton)
   {
       rubberOrigin = event->pos();
       rubberBand->setGeometry(QRect(rubberOrigin, QSize()));
       rubberBand->show();
   }else if(event->button() == Qt::RightButton)
   {
        //记录当前的值

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
    m_tracer->UpdatePosition(x_val,y_val);
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


    m_tracer = new MyPlotTrace(ui->plot3,MAX_CHAN_NR);

    //connect(ui->plot3, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(myMouseMoveEvent(QMouseEvent*)));
    ui->dteFrom->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->dteTo->setDateTime(QDateTime::currentDateTime());
    //on_cbxTimeSpan_currentIndexChanged(0);
   // ui->cbxTimeSpan->setVisible(false);
    //ui->label_8->setVisible(false);
    devices = new MyDevices(MAX_CHAN_NR+1,ui->gbDevices);
    devices->SetTimeRange(Config::instance().m_rt_wave_min*60);
    devices->SetMaxSampleNum(50);
    devices->SetDeviceNum(1,MAX_CHAN_NR);
    devices->SetUnit("kg");
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
    ui->btnLocalIP->setVisible(false);
    ui->btnPing->setVisible(false);
    ui->edtPingIp->setVisible(false);
    ui->btnPause->setVisible(false);
    ui->orderList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->orderList->setColumnWidth(0,300);
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
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    menu=new QMenu(this);
    menu2=new QMenu(this);
    QIcon setting= QIcon(":image/setting.png");
    QAction* action = new QAction(setting,QStringLiteral("配置参数"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_menu_click(bool)));

//    action = new QAction(QStringLiteral("标定重量"),this);
//    menu->addAction(action);

//    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_write_menu_click(bool)));

    action = new QAction(QStringLiteral("复位设备"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_reset_menu_click(bool)));

    action = new QAction(QStringLiteral("删除设备"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_remove_device_click(bool)));

    action = new QAction(QStringLiteral("修改设备"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_modify_menu_click(bool)));

    action = new QAction(QStringLiteral("升级设备"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_update_menu_click(bool)));

    action = new QAction(QStringLiteral("电脑数据清理"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_clearup_menu_click(bool)));

    action = new QAction(QStringLiteral("设备数据清理"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_clear_history_menu_click(bool)));

    action = new QAction(QStringLiteral("下载U盘数据"),this);
    menu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_stop_menu_click(bool)));


//    action = new QAction(QStringLiteral("复位计数器"),this);
//    menu->addAction(action);
//    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_reset_count_click(bool)));
//    action = new QAction(QStringLiteral("获取计数器"),this);
//    menu->addAction(action);
//    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_get_count_click(bool)));


//GetCount
    action = new QAction(QStringLiteral("添加设备"),this);
    menu2->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_add_device_click(bool)));

    ui->btnMain->click();
    on_btnMenu_Max_clicked();


    fileWatcher = new QFileSystemWatcher(this);

    fileWatcher->addPath(Config::instance().m_data_dir+QStringLiteral("/压力测试状态表.xlsm"));

    connect(fileWatcher,&QFileSystemWatcher::fileChanged,this,&MainWnd::fileChange);

    connect(&AsyncExportManager::instance(),SIGNAL(onProgress(QString,int,int)),this,SLOT(onProgress(QString,int,int)));
    connect(&AsyncExportManager::instance(),SIGNAL(onSucc(QString,QString)),this,SLOT(onSucc(QString,QString)));

    connect(UsbImport::instance(),SIGNAL(onSucc()),this,SLOT(onUsbImportSucc()));

 //加载设备状态.

    initDeviceChannels();
    loadChannels();
    loadSysConfig();
    loadDeviceUI();
}
void MainWnd::fileChange(const QString &path)
{
    qDebug() << path << "change";
    loadStateFile();
}

void MainWnd::onProgress(QString serialNo, int prog, int err)
{
    ui->btnExport->setText(QString(QStringLiteral("已完成%1%")).arg(prog));
}

void MainWnd::onSucc(QString serialNo, QString err)
{
    ui->btnExport->setText(QStringLiteral("导出数据"));
    ui->btnExport->setEnabled(true);
}
void MainWnd::loadSysConfig()
{
    //ui->cbxCorp->clear();
    ui->edtPort->setText(QString("%1").arg(Config::instance().m_local_port));
    ui->edtCorpName->setText(Config::instance().m_corp_name);

    ui->edtFtpBase->setText(Config::instance().m_ftp_base);
    ui->cbxFileFormat->setCurrentIndex(Config::instance().m_file_format);
    ui->edtHost->setText(Config::instance().m_host_name);
}
static bool loading = false;
//加载状态文件.
void MainWnd::loadStateFile(bool create)
{
    qDebug() << "loadStateFile";
    loading = true;
    ui->edtDataDir->setText(Config::instance().m_data_dir);
    ui->cbxHost->clear();
    //ui->cbxTestNo->clear();
    //ui->listFiles->clear();
    if(!StateManager::instance().parse(Config::instance().m_data_dir+QStringLiteral("/压力测试状态表.xlsm")))
    {
        qDebug() << "loadStateFile failed";
        QMessageBox::information(this,"info",QStringLiteral("目录下找不到压力测试状态表.xlsm"));
        this->AddLog("loadStateFile failed");
        loading = false;
        return;
    }
#if 1
    CellTestHost& host = StateManager::instance().GetState();
    //host列表，每个host有一个订单列表.
    QMap<QString,CellTestOrderList>::const_iterator i = host.constBegin();
    while (i != host.constEnd()) {
        qDebug() << i.key();
        ui->cbxHost->addItem(i.key());
        if(create){
            CellTestOrderList::const_iterator it = i.value().constBegin();
            while (it != i.value().constEnd()) {
                QString cell=QString("%1/%2/%3").
                        arg(Config::instance().
                            m_data_dir).
                        arg(i.key()).arg(it.key());
                utils::MkMutiDir(cell);
                ++it;
            }

        }


        ++i;
    }

#endif
    loading = false;
     qDebug() << "loadStateFile ok----";
    ui->cbxHost->setCurrentText(Config::instance().m_host_name);
    qDebug() << "loadStateFile ok";
    emit on_cbxHost_currentIndexChanged(Config::instance().m_host_name);

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
    bQueryOrderState = false;
    if (name == "btnMain") {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "btnConfig") {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "btnData") {
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "btnReport") {
        bQueryOrderState = true;
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
    int result = myHelper::ShowMessageBoxQuesion(QStringLiteral("确定离开?"));
       if (result == 1) {
           this->dvm.Sync();
           event->accept();
       } else {
           event->ignore();
       }


}

void MainWnd::onUsbImportSucc()
{
    ui->btnImport->setEnabled(true);
    ui->btnImport->setText(QStringLiteral("导入数据"));
    QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("导入完成"));
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
    //ui->dteTo->setDateTime(QDateTime::currentDateTime());
    //ui->dteFrom->setDateTime(QDateTime::fromMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch()-time_span_list[index]*1000));
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
     if(!checked){
         m_tracer->ShowAll(false);
     }else{
         for(int i = 0 ; i < rbChanList.size();i++)
         {
             m_tracer->Show(i,rbChanList[i]->isChecked());
         }
     }

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
    QString id,name;
    if(!GetCurrentDeviceId2Name(name)){
        return;
    }
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

    QString fileName = QFileDialog::getExistingDirectory(this,QStringLiteral("保存文件"));
    qDebug() << "Filename=" << fileName;
    if(fileName.length() < 3){
        return;
    }
    qDebug() << "from="<<from << "to=" <<to;
    AsyncExportManager::instance().AddTask(Config::instance().m_file_format,id,name,chans,from,to,fileName);
    ui->btnExport->setText(QStringLiteral("正在导出"));
    ui->btnExport->setEnabled(false);
    // myHelper::ShowMessageBoxInfo(QStringLiteral("导出完成"));
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
void MainWnd::listOrders(QString host)
{
    CellTestOrderList& orders =  StateManager::instance().GetOrderList(host);
    if(orders.size() == 0){
       return;
    }
    QSignalMapper *signalMapper = new QSignalMapper(this);
    QSignalMapper *signalMapper2 = new QSignalMapper(this);
    ui->orderList->clear();

    QMap<QString,QVector<CellState> >::const_iterator i = orders.constBegin();

    while (i != orders.constEnd()) {

        QStringList row;
        //订单名,把这个名字去掉后面的温度后缀.
        row.push_back(i.key());
        //只要有订单存在就
        if(i.value().size() > 0){
            //温度.
           row.push_back(i.value().at(0).Temp);
        }else{

        }
        //订单个数
        row.push_back(QString("%1").arg(i.value().size()));
        QTreeWidgetItem* item = new QTreeWidgetItem(row);
        QVector<CellState> cells = i.value();

        for(int j = 0 ; j < cells.size(); j++)
        {
             QTreeWidgetItem* subItem = new QTreeWidgetItem(QStringList(cells[j].CellNo));
             item->addChild(subItem);
        }

        ui->orderList->addTopLevelItem(item);
        QPushButton *btnReport = new QPushButton(QStringLiteral("生成报告"));
         btnReport->setGeometry(0,0,80,40);

         QPushButton *btnOpenDir = new QPushButton(QStringLiteral("打开目录"));
          btnOpenDir->setGeometry(0,0,80,40);

        connect(btnOpenDir, SIGNAL(clicked()), signalMapper2, SLOT(map()));
        //QString dir = QString("%1").arg(1);
         QString dir = QString(QStringLiteral("%1/%2/%3"))
                        .arg(Config::instance().m_data_dir)
                        .arg(host)
                        .arg(i.key());

        signalMapper2->setMapping(btnOpenDir, dir);

        connect(btnReport, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(btnReport, i.key());



        ui->orderList->setItemWidget(item, 3, btnReport);
        ui->orderList->setItemWidget(item, 4, btnOpenDir);


        ++i;
    }
    connect(signalMapper, SIGNAL(mapped(QString)),
                this, SLOT(on_report_click(QString)));
    connect(signalMapper2, SIGNAL(mapped(QString)),
                this, SLOT(on_opendir_click(QString)));

}
void MainWnd::on_cbxHost_currentIndexChanged(const QString &arg1)
{
    qDebug() << "onchage" << arg1 << loading;
     if(loading) return;
    listOrders(arg1);
}

void MainWnd::on_opendir_click(QString dir)
{
    //QString dir_gbk = utils::UTF82GBK(dir);
   // QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
    //std::string name = code->fromUnicode(fileName).data();

    //bool ok = QDesktopServices::openUrl(QUrl(dir,QUrl::TolerantMode));
    bool ok =QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
    qDebug() << "open " << dir << " result=" << ok;
}
void MainWnd::on_gen_report_response(AjaxResponse r)
{
    //qDebug() << r.error;
}
void MainWnd::on_query_report_response(AjaxResponse r)
{
    if(r.error){
        return;
    }
    QJsonDocument doc;

    if(QJsonParseError::NoError == r.GetJSON(doc).error)
    {
        refreshOrderState(doc.toJson());
    }


    //qDebug() << r.error;
}
void MainWnd::report_reqeust(QString order)
{
    ajax.post("http://localhost:9527/GenerateReport",
              buildReportInputJson(order),
              this,
              SLOT(on_gen_report_response(AjaxResponse)));
}
void MainWnd::on_report_click(QString order)
{
    qDebug() << order << " click";
    if(pFnBuildReport){
        LOG_DEBUG("report by dll");
        QGoString str(buildReportInput(order));
        char* res=NULL;
        int code = pFnBuildReport(str.toGoString(),&res);
        if(code != 0){
           QJsonDocument doc =  QJsonDocument::fromJson(res);
           QJsonObject o = doc.object();
           myHelper::ShowMessageBoxError(o["message"].toString());
        }

        qDebug() << "code=" << code << "result=" << res;

    }else{
        LOG_DEBUG("report by PressReportd.exe");
        StartServer();
        report_reqeust(order);
    }

}
void MainWnd::on_cbxTestNo_currentIndexChanged(const QString &arg1)
{
    //列出所有的电芯

    if(loading) return;
    QString order = ui->cbxHost->currentText();
   CellTestOrderList& orders =  StateManager::instance().GetOrderList(order);
    //ui->listFiles->clear();
   if(orders.contains(arg1)){
       for(int i = 0; i < orders[arg1].size(); i++)
       {
        //   ui->listFiles->addItem(orders[arg1].at(i).CellNo);
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
QString MainWnd::getFtpDir(QString order)
{

   return QString("%1/%2/%3/%4/").
           arg(Config::instance().m_ftp_base).
           arg(ui->edtCorpName->text()).
           arg(QDate::currentDate().toString("yyyy/MM")).
           arg(order);

}

QString MainWnd::parseDateTime(QString order)
{
    int year = order.mid(1,4).toInt();
    int mon = order.mid(5,2).toInt();
    return QString(QStringLiteral("%1年/%2月")).arg(year).arg(mon);
}
QString MainWnd::buildReportInput(QString order)
{
    QJsonDocument doc = buildReportInputJson(order);

    return doc.toJson();
}
QString MainWnd::GetNetFile(QString temp,QString order,QString orderKey)
{
    return QString(QStringLiteral("%1/%2/%3/%4/%5_%6压力测试表.xlsx"))
                    .arg(Config::instance().m_ftp_base)
                    .arg(ui->edtCorpName->text())
                    .arg(parseDateTime(order))
                    .arg(order).arg(order).arg(temp);
}
bool MainWnd::SaveNetFile(QString netFile,QString localFile)
{
    if(utils::ExistFile(netFile)){
         QXlsx::Document xlsx(netFile);
         if(xlsx.load()){

             return xlsx.saveAs(localFile);
         }
    }
    return false;
}

QJsonDocument MainWnd::buildReportInputJson(QString orderKey)
{
    QString name = ui->cbxHost->currentText();
    CellTestOrderList& orders =  StateManager::instance().GetOrderList(name);

    QJsonDocument doc;
    QJsonObject root;
    //QString order = "";
    if(orders.contains(orderKey) && orders[orderKey].size() > 0){

        QJsonArray arr;
        QString order = orders[orderKey].at(0).TestNo;
        QString temp  = orders[orderKey].at(0).Temp;
        root["order_no"] = order;//orders[orderKey].at(0).TestNo;
        root["order_key"] = orderKey;
        root["temp"] = temp ;

        QString netfile = GetNetFile(temp,order,orderKey);

        QString localfile = QString(QStringLiteral("%1/%2/%3/%4_%5压力测试表.xlsx"))
                .arg(Config::instance().m_data_dir)
                .arg(ui->cbxHost->currentText())
                .arg(orderKey).arg(order).arg(temp);
       // qDebug() <<"temp=" << temp<< " localfile=" << localfile;
        root["ftp_dir"] = netfile;
        root["target_file"] =localfile;
        root["db"] = utils::GetWorkDir()+"/config.db";
        root["data_dir"] = QString("%1/data").arg(utils::GetWorkDir());
        root["corp_name"] = ui->edtCorpName->text();
        root["dir_path"]=Config::instance().m_data_dir;
        root["host"] = ui->cbxHost->currentText();
        root["skip_error"] = true;
        root["ftp_enable"] = Config::instance().m_ftp_enable;
        root["ftp_pwd"] = Config::instance().m_ftp_pwd;
        root["ftp_url"] = Config::instance().m_ftp_host;
        root["ftp_port"] = Config::instance().m_ftp_port;
        root["ftp_user"] = Config::instance().m_ftp_user;

        QVector<CellState> &states =  orders[orderKey];
        for(int i = 0; i <states.size(); i++)
        {
             QJsonObject o;


             o["dev_chan"] = orders[orderKey].at(i).PressDevChan;
             o["dev_name"] = orders[orderKey].at(i).PressDevId;
             o["ctrl_name"] = orders[orderKey].at(i).ChargeDev;
             o["cell_no"] =states[i].CellNo;

            arr.push_back(o);
        }
        root["chan_array"] = arr;


    }else{
        qDebug() << "can not find order" << orderKey;
    }
    doc.setObject(root);
    qDebug() << QString(doc.toJson());
    return doc;
    //doc.setObject(root);
   // return doc.toJson();
}


void MainWnd::on_btnExecReport_clicked()
{
    //启动导出，在某个路径下查找电芯文件的路径
    //默认规则是 电芯文件放到 工单号的目录下.
    //选择工单后，就到工单目录下去查找该工单的所有文件
    //然后对每个文件调用保存工具
    if(pFnBuildReport){
        QGoString str(buildReportInput("00"));
        char* res=NULL;
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
    fileWatcher->removePaths(fileWatcher->files());
    fileWatcher->addPath(Config::instance().m_data_dir+QStringLiteral("/压力测试状态表.xlsm"));

    loadStateFile();
}

void MainWnd::on_edtHost_textChanged(const QString &arg1)
{
    Config::instance().SetHostName(arg1);
}

void MainWnd::on_chkMeasure_clicked()
{

}

void MainWnd::on_chkSelAll_clicked(bool checked)
{

}

void MainWnd::on_btnReload_clicked()
{
    //重新加载数据
    loadStateFile(true);
}
#include "websetclient.h"
static  WebSetClient* client=NULL ;
void MainWnd::on_btnLocalIP_clicked()
{
    if(client==NULL){
        client = new WebSetClient(NULL);
    }
     client->show();
   // QStringList ip = NetTools::get_local_ip();
    QStringList mac = NetTools::get_devices();
//    for(int i = 0; i< ip.size();i++)
//    {
//        AddLog(QString("ip[%1]=%2").arg(i+1).arg(ip.at(i)));
//    }

    for(int i = 0; i< mac.size();i++)
    {
        AddLog(QString("%1").arg(mac.at(i)));
    }

}

void MainWnd::on_btnPing_clicked()
{
//    QString target = ui->edtPingIp->text();
//    if(target.length() < 7){
//        return;
//    }
//    ping.ping(target,4);
}

void MainWnd::on_btnClear_clicked()
{
    ui->txtLog->clear();
//    m_ftp.Auth(21,"byteman","wangcheng");
//    m_ftp.AddTask("c:/work/fridge","ftp://127.0.0.1/fridge");
//    m_ftp.AddTask("c:/work/zImage","ftp://127.0.0.1/zImage");

}

void MainWnd::on_chkSensorOff_clicked(bool checked)
{
    Config::instance().EnableRecvSensorOff(checked);
}

void MainWnd::on_rb6_clicked()
{

}

void MainWnd::on_edtFtpHost_textChanged(const QString &arg1)
{
    Config::instance().SetFtpHost(arg1);
}

void MainWnd::on_edtFtpName_textChanged(const QString &arg1)
{
    Config::instance().SetFtpUser(arg1);
}

void MainWnd::on_edtFtpPwd_textChanged(const QString &arg1)
{
    Config::instance().SetFtpPwd(arg1);
}

void MainWnd::on_edtFtpBase_textChanged(const QString &arg1)
{
    Config::instance().SetFtpBase(arg1);
}

void MainWnd::updateTimes()
{

}
void MainWnd::on_rb1_clicked()
{

}

void MainWnd::on_btnStartAll_clicked()
{
    on_start_menu_click(true);
}

void MainWnd::on_btnStopAll_clicked()
{
    on_stop_menu_click(true);
}

void MainWnd::on_cbxFileFormat_currentIndexChanged(int index)
{
    Config::instance().SetFileForamt(index);
}
//从U盘数据中导入压力数据.
void MainWnd::on_btnImport_clicked()
{
    QString id,name;
    if(!GetCurrentDeviceId2(id)){
        return;
    }
    if(!GetCurrentDeviceId2Name(name)){
        return;
    }
    bool ok = IsOk(QStringLiteral("操作确认"),QString(QStringLiteral("%1%2号模块中吗?")).arg(QStringLiteral("确认导入数据到")).arg(name));
    if(!ok){
        return;
    }
    QString dirName = QFileDialog::getExistingDirectory(this,QStringLiteral("选择导入文件目录"));
    qDebug() << "Filename=" << dirName;
    if(dirName.length() < 3){
        return;
    }
    ui->btnImport->setEnabled(false);
    ui->btnImport->setText(QStringLiteral("正在分析"));
    UsbImport::instance()->start(id,dirName);
}

void MainWnd::on_btnOpenReport_clicked()
{
    QString dir  = Config::instance().m_ftp_base+"/"+ui->edtCorpName->text();

    bool ok =QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
    qDebug() << "open " << dir << " result=" << ok;
}
//#include "dialogcorpmanager.h"
//void MainWnd::on_bntCorpConfig_clicked()
//{
//    QStringList old = Config::instance().m_corp_list;

//    DialogCorpManager dlg;
//    dlg.exec();
//    QStringList newc = Config::instance().m_corp_list;
//    if(newc == old){
//        return;
//    }
//    ui->cbxCorp->clear();
//    ui->cbxCorp->addItems(newc);
//    int idx = Config::instance().m_corp_index;
//    if(old.size() > newc.size())
//    {
//        //大小变小了
//        idx = -1;
//    }

//    Config::instance().SetCorpNameInx(idx);
//    ui->cbxCorp->setCurrentIndex(idx);
//}

//void MainWnd::on_cbxCorp_currentIndexChanged(int index)
//{
//    qDebug() << "index=" << index;
//    Config::instance().SetCorpNameInx(index);
//}

void MainWnd::on_edtCorpName_textChanged(const QString &arg1)
{
    Config::instance().SetCorpName(arg1);
}
