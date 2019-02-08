#pragma execution_character_set("utf-8")

#include "MainWnd.h"
#include "ui_MainWnd.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "dao.h"
#include "dialogparams.h"
#include "dialogdevice.h"
#include "dialogchanconfig.h"
#include <QDebug>
void MainWnd::AddLog(QString msg)
{
    ui->txtLog->append(msg);
}
void MainWnd::Init()
{
    m_cur_dev_id = "123";

    qRegisterMetaType<SessMessage>("SessMessage");

    icon_device[0].addFile(":image/device.png");
    icon_device[1].addFile(":image/offline.png");
    icon_channel.addFile(":image/channel.png");
    icon_dir.addFile(":image/dir.png");
    icon_file.addFile(":image/channel.png");


    srv = new GPServer();
    if(!srv->start(8881)){
        AddLog(QString::fromLocal8Bit("服务启动失败,检查端口8888是否被占用!!"));
    }else{
        AddLog(QString::fromLocal8Bit("服务启动成功"));
    }
    connect(srv,SIGNAL(Message(SessMessage)),this,SLOT(Message(SessMessage)));
    connect(srv,SIGNAL(Message(SessMessage)),&dvm
            ,SLOT(Message(SessMessage)));

    connect(&dvm,SIGNAL(Notify(QString)),this,SLOT(onNotify(QString)));
    connect(&dvm,SIGNAL(DevOffline(Device*)),this,SLOT(DevOffline(Device*)));
    connect(&dvm,SIGNAL(DevOnline(Device*)),this,SLOT(DevOnline(Device*)));

    //connect(&dvm,SIGNAL(EnumFiles(Device*,ENUM_FILE_RESP)),this,SLOT(onEnumFiles(Device*,ENUM_FILE_RESP)));
    //connect(&dvm,SIGNAL(Progress(Device*,QString)),this,SLOT(onWaveProgress(Device*,QString)));
    connect(&dvm,SIGNAL(WaveMsg(Device*,MsgWaveData)),this,SLOT(onWaveMsg(Device*,MsgWaveData)));
    connect(&dvm,SIGNAL(SensorMsg(Device*,MsgSensorData)),this,SLOT(onSensorMsg(Device*,MsgSensorData)));
    connect(&dvm,SIGNAL(ResetResult(Device*,bool)),this,SLOT(onResetResult(Device*,bool)));
    //connect(&dvm,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCalibResult(Device*,int,int,int)));
    connect(&dvm,SIGNAL(RealTimeResult(Device*,RT_AD_RESULT)),this,SLOT(onRealTimeResult(Device*,RT_AD_RESULT)));

    //connect(&dvm,SIGNAL(CommResult(Device*,int,int)),this,SLOT(onCommResult(Device*,int,int)));

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(on_mytime_out()));
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


    action = new QAction(QString::fromLocal8Bit("添加设备"),this);
    menu2->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(on_add_device_click(bool)));

    QSqlError err =  DAO::instance().Init("measure.db");
    if(err.isValid()){
        myHelper::ShowMessageBoxError(err.text());
    }
    dvm.start();

    loadDeviceUI();
    this->startTimer(100);
    on_btnMenu_Max_clicked();


}
QString FormatHex(QByteArray& data)
{
    return data.toHex();
}
void MainWnd::Message(SessMessage s)
{
    //if(pause)return;
   // if(s.getData().size() <   m_debug_bytes)
    AddLog(QString("recv-> %1:%2 ").arg(s.getHost().toString()).arg(s.getPort())+FormatHex(s.getData()));
}

void MainWnd::onResetResult(Device *, bool)
{
    myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("复位设备成功"));

}

void MainWnd::on_mytime_out()
{
    //qDebug() << "mytimeout";
    //dvm.ReadRt(m_cur_dev_id);
}
void MainWnd::onNotify(QString msg)
{

    //ui->txtLog->append("send--> "+ msg);
}

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
        //loadDeviceUI();
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
void MainWnd::on_add_device_click(bool)
{
    qDebug() << "add device";
    DialogDevice dlg;
    int result = dlg.exec();
    if(result == QDialog::Accepted){
        QString dev_id,dev_name;
        dlg.GetDeviceInfo(dev_id,dev_name);
        dvm.AddDevice(dev_id,dev_name);
        loadDeviceUI();
    }
}
void MainWnd::on_remove_device_click(bool)
{
    qDebug()<<m_cur_dev_id << " removed";
    QTreeWidgetItem* item =  ui->treeWidget->currentItem();
    if(item==NULL){
         return ;
    }
     QString id = item->data(0,Qt::UserRole).toString();
     if(dvm.RemoveDevice(id)){
         myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("删除设备成功"));
     }else{
         myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("删除设备失败"));
     }
     loadDeviceUI();

}
void MainWnd::on_modify_menu_click(bool)
{
    qDebug()<<m_cur_dev_id << " modify";
    DialogDevice dlg;
   QTreeWidgetItem* item =  ui->treeWidget->currentItem();
   if(item==NULL){
        return ;
   }
    QString id = item->data(0,Qt::UserRole).toString();
    Device* dev = dvm.GetDevice(id);
    if(dev==NULL)
    {
        return;
    }
    dlg.UpdateDeviceInfo(dev->id(),dev->name());

    int result = dlg.exec();
    if(result == QDialog::Accepted){
        QString dev_id,dev_name;
        dlg.GetDeviceInfo(dev_id,dev_name);
        dvm.UpdateDevice(dev_id,dev_name);
        loadDeviceUI();
    }
}

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
void MainWnd::DevOffline(Device *dev)
{
    QTreeWidgetItem* item = findItemById(dev->id());
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
}
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
    //simData();

}
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
void MainWnd::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    m_cur_dev_id = item->data(0,Qt::UserRole).toString();
}

void MainWnd::onWaveMsg(Device *dev, MsgWaveData data)
{
   if(data.m_first)
   {
       //m_waveWdg->Clear();
   }

}

void MainWnd::onSensorMsg(Device *dev, MsgSensorData data)
{


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
MainWnd::MainWnd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWnd)
{
    ui->setupUi(this);
    this->initForm();

    Init();

    //DAOTest();
}

MainWnd::~MainWnd()
{
    delete ui;
}
void MainWnd::loadDeviceUI()
{

    //pause = false;
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
        }
    }
}
int get_random_number()
 {
     qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
     int a = qrand()%100;   //随机生成0到9的随机数
     return a;
}
void MainWnd::simData()
{
    for(int i = 0; i < 8; i++)
    {
        devices->DisplayWeight(i,get_random_number(),0,0);
        Sleep(i);

    }
    //wave->AppendData(0,get_random_number());
    //wave->DisplayChannel(0,true);

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

void MainWnd::initForm()
{
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

    //ui->stackedWidget->setStyleSheet("QLabel{font:60pt;}");

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

    ui->btnMain->click();

    ui->gbDevices->installEventFilter(this);
    wave = new WaveWidget(ui->plot3,1);
    wave->SetChannel(0,1);
    devices = new MyDevices(9,ui->gbDevices);
    devices->SetMaxSampleNum(50);
    devices->SetDeviceNum(1,8);
    connect(devices,SIGNAL(onChannelConfig(int)),this,SLOT(onChannelClick(int)));

    loadDeviceUI();
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
        ui->stackedWidget->setCurrentIndex(3);
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


void MainWnd::resizeEvent(QResizeEvent *e)
{
//    qDebug() << e->size();

}


void MainWnd::showEvent(QShowEvent *)
{

}

void MainWnd::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    qDebug() <<"click";
}
void MainWnd::on_btnShou_clicked()
{
    static bool hide = false;
    if(!hide){
        ui->treeWidget->hide();
        ui->btnShou->setIcon(QIcon(":/image/fang.png"));
        qDebug() << "hide";
        //ui->widgetDocker->setStyleSheet("border:0px;");
        //ui->widgetDocker->resize(ui->btnShou->width(),ui->widgetDocker->height());

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

void MainWnd::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    m_cur_dev_id = current->data(0,Qt::UserRole).toString();
    qDebug() << "item change to" << m_cur_dev_id;
}
