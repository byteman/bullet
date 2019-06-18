#pragma execution_character_set("utf-8")

#include "MainWnd.h"
#include "ui_MainWnd.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "dao.h"
#include "dialogdevice.h"
#include "utils.h"
#include "config.h"
#include "main.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include "gotypes.h"
#include <QInputDialog>
#include "formgds.h"
#define MAX_CHAN_NR 8
#define LISTEN_PORT 8881
void MainWnd::AddLog(QString msg)
{
    ui->txtLog->append(msg);
}
#include "nettools.h"
void MainWnd::outputVer()
{
    AddLog(QString("Ver-%1-%2").arg("1.0.1").arg(__DATE__));
}
void MainWnd::StartReceiver()
{
    srv = new GPServer();

    if(!srv->start(Config::instance().m_local_port)){
        AddLog(QString::fromLocal8Bit("服务启动失败,检查端口8881是否被占用!!"));
    }else{
        AddLog(QString::fromLocal8Bit("服务启动成功"));
    }

    connect(srv,SIGNAL(Message(SessMessage)),this,SLOT(Message(SessMessage)));
    connect(srv,SIGNAL(Message(SessMessage)),&dvm
            ,SLOT(Message(SessMessage)));


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

static  funcBuildReport pFnBuildReport  = NULL;
static  funcGetOrderState pFnGetOrderState = NULL;
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
    //StateManager::instance().parse("state.xlms");
    initGoLibrary();
    //首先初始化数据管理模块.

    QSqlError err =  DAO::instance().Init(utils::GetWorkDir()+"/measure.db");
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

void MainWnd::on_write_menu_click(bool)
{

}

void MainWnd::on_reset_count_click(bool)
{
    dvm.ResetDeviceCount(m_cur_dev_id);
}
//添加一个设备.
void MainWnd::on_add_device_click(bool)
{
    if(!this->CheckPassWord())return;
    if(dvm.DeviceCount() >=8 ){
         myHelper::ShowMessageBoxError(QStringLiteral("最多添加8个设备"));
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
         myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("删除设备成功"));
     }else{
         myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("删除设备失败"));
     }
     reloadDeviceList();
      reloadDeviceList2();

}

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
                //this->devices->SetOnline(devices[i]->online());
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

}


void MainWnd::onSensorMsg(Device *dev, MsgSensorData data)
{

    if(dev->id() != m_cur_dev_id){
        return;
    }
    for(int i = 0; i < data.channels.size(); i++)
    {
       // devices->DisplayWeight(data.channels[i].addr,data.channels[i].weight,0,0);
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
    bQueryOrderState(false)
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

bool MainWnd::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == ui->widgetTitle) {
            on_btnMenu_Max_clicked();
            return true;
        }
    }else if (event->type() == QEvent::Resize) {
        if(mydev!=NULL)
            mydev->Resize();

        return true;
    }

    return QWidget::eventFilter(watched, event);
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


//GetCount
    action = new QAction(QString::fromLocal8Bit("添加设备"),this);
    menu2->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_add_device_click(bool)));

    ui->btnMain->click();
    on_btnMenu_Max_clicked();

    //initDeviceChannels();
    mydev = new MyDevices(ui->gbDevices);
    loadSysConfig();
    loadDeviceUI();
}

void MainWnd::loadSysConfig()
{
    ui->edtPort->setText(QString("%1").arg(Config::instance().m_local_port));
    ui->sbWaveMin->setValue(Config::instance().m_rt_wave_min);
    ui->chkSensorOff->setChecked(Config::instance().m_recv_sensor_off);

    ui->edtHost->setText(Config::instance().m_host_name);
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
    } else if (name == "btnHelp") {
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
           this->dvm.Sync();
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

void MainWnd::on_edtPort_textChanged(const QString &arg1)
{
    bool ok = false;
    int port = arg1.toInt(&ok);
    if(!ok){
        return;
    }
    Config::instance().SetLocalPort(port);
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


void MainWnd::on_treeWidget2_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    QString id;
    if(!GetCurrentDeviceId2(id)){
      return;
    }
}

void MainWnd::on_btnHelp_clicked()
{

}


void MainWnd::on_btnLocalIP_clicked()
{

    QStringList mac = NetTools::get_devices();

    for(int i = 0; i< mac.size();i++)
    {
        AddLog(QString("%1").arg(mac.at(i)));
    }

}

