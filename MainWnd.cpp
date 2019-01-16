#pragma execution_character_set("utf-8")

#include "MainWnd.h"
#include "ui_MainWnd.h"
#include "iconhelper.h"
#include <QDebug>
void MainWnd::AddLog(QString msg)
{
    ui->txtLog->append(msg);
}
void MainWnd::Init()
{
    m_cur_dev_id = "123";

    qRegisterMetaType<SessMessage>("SessMessage");


    QSettings config("bullet.ini", QSettings::IniFormat);
    config.setIniCodec("UTF-8");//添上这句就不会出现乱码了);

    //m_debug_bytes = config.value("/device/debug",20).toInt();
    //m_refresh_count = config.value("/device/refresh",20).toInt();
    m_cur_station = config.value("/device/station","123").toString();
    int dot= 3 - config.value("/device/dot",3).toInt();
    if(dot < 0 || dot > 3) dot = 0;


    icon_device[0].addFile(":image/device.png");
    icon_device[1].addFile(":image/offline.png");
    icon_channel.addFile(":image/channel.png");
    icon_dir.addFile(":image/dir.png");
    icon_file.addFile(":image/channel.png");


    srv = new GPServer();
    if(!srv->start(8888)){
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
    //connect(&dvm,SIGNAL(ReadParam(Device*,MsgDevicePara)),this,SLOT(onReadPara(Device*,MsgDevicePara)));
    //connect(&dvm,SIGNAL(WriteParam(Device*,bool)),this,SLOT(onWritePara(Device*,bool)));
    //connect(&dvm,SIGNAL(EnumFiles(Device*,ENUM_FILE_RESP)),this,SLOT(onEnumFiles(Device*,ENUM_FILE_RESP)));
    //connect(&dvm,SIGNAL(Progress(Device*,QString)),this,SLOT(onWaveProgress(Device*,QString)));
    connect(&dvm,SIGNAL(WaveMsg(Device*,MsgWaveData)),this,SLOT(onWaveMsg(Device*,MsgWaveData)));
    //connect(&dvm,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCalibResult(Device*,int,int,int)));
    //connect(&dvm,SIGNAL(RealTimeResult(Device*,RT_AD_RESULT)),this,SLOT(onRealTimeResult(Device*,RT_AD_RESULT)));

    //connect(&dvm,SIGNAL(CommResult(Device*,int,int)),this,SLOT(onCommResult(Device*,int,int)));

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(on_mytime_out()));
    ui->treeWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    menu=new QMenu(this);
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



    dvm.start();
    //setCurrentStation(m_cur_station);
    //dvm.SetStation("工位1");
    loadDeviceUI();

    //m_waveWdg = new WaveWidget(ui->plot,12);

    this->startTimer(100);
    on_btnMenu_Max_clicked();
    //stopTime = false;
    qDebug() <<"MainWindow thread id=" << thread();

}
void MainWnd::Message(SessMessage s)
{
    //if(pause)return;
    //if(s.getData().size() <   m_debug_bytes)
   //     ui->txtLog->append(QString("recv-> %1:%2 ").arg(s.getHost().toString()).arg(s.getPort())+FormatHex(s.getData()));
}
void MainWnd::on_mytime_out()
{
    //qDebug() << "mytimeout";
    //dvm.ReadRt(m_cur_dev_id);
}
void MainWnd::onNotify(QString msg)
{

    ui->txtLog->append("send--> "+ msg);
}

void MainWnd::on_menu_click(bool)
{

}
void MainWnd::on_write_menu_click(bool)
{

}
void MainWnd::on_reset_menu_click(bool)
{

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
                item->setIcon(0,icon_device[0]);
        }
    }
    //simData();

}
void MainWnd::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{

       QTreeWidgetItem* curItem=ui->treeWidget->itemAt(pos);  //获取当前被点击的节点
       if(curItem == NULL)
       {

           return;
       }

       menu->popup(QCursor::pos());

}
void MainWnd::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{

}

void MainWnd::onWaveMsg(Device *dev, MsgWaveData data)
{
   if(data.m_first)
   {
       //m_waveWdg->Clear();
   }
//   if(dev->id() != m_cur_dev_id)
//   {
//       return;
//   }


}

MainWnd::MainWnd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWnd)
{
    ui->setupUi(this);
    this->initForm();
    Init();
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

        item->setIcon(0,icon_device[0]);

        item->setData(0,Qt::UserRole,devices[i]->id());
        if(i == 0){
            ui->treeWidget->setCurrentItem(item);
            m_cur_dev_id = devices[i]->id();
        }
    }
}
bool MainWnd::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == ui->widgetTitle) {
            on_btnMenu_Max_clicked();
            return true;
        }
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
    devices = new MyDevices(36,ui->gbDevices);
    devices->SetMaxSampleNum(50);
    devices->SetDeviceNum(1,8);
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
        exit(0);
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
    devices->Resize();
}
