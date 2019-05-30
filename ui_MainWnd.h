/********************************************************************************
** Form generated from reading UI file 'MainWnd.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWND_H
#define UI_MAINWND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_MainWnd
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widgetTitle;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labIco;
    QLabel *labTitle;
    QWidget *widgetTop;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *btnMain;
    QToolButton *btnConfig;
    QToolButton *btnData;
    QToolButton *btnReport;
    QToolButton *btnHelp;
    QToolButton *btnExit;
    QSpacerItem *horizontalSpacer;
    QWidget *widgetMenu;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QPushButton *btnMenu_Min;
    QPushButton *btnMenu_Close;
    QPushButton *btnMenu_Max;
    QStackedWidget *stackedWidget;
    QWidget *page1;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widgetDocker;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QVBoxLayout *verticalLayout_4;
    QToolButton *btnShou;
    QSpacerItem *verticalSpacer_6;
    QGroupBox *gbDevices;
    QVBoxLayout *verticalLayout_14;
    QWidget *page2;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_2;
    QWidget *basicWidget;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_5;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLineEdit *edtHost;
    QLabel *label_12;
    QSpinBox *sbWaveMin;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *label;
    QComboBox *cbxLang;
    QLineEdit *edtPort;
    QCheckBox *chkSensorOff;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    QWidget *page3;
    QHBoxLayout *horizontalLayout_8;
    QWidget *widgetDocker_2;
    QHBoxLayout *horizontalLayout_7;
    QTreeWidget *treeWidget2;
    QVBoxLayout *verticalLayout_5;
    QToolButton *btnShou2;
    QSpacerItem *verticalSpacer_7;
    QGroupBox *gbDevices2;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_9;
    QCustomPlot *plot3;
    QWidget *widgetChan;
    QVBoxLayout *verticalLayout_9;
    QCheckBox *chkSelAll;
    QCheckBox *rb1;
    QCheckBox *rb2;
    QCheckBox *rb3;
    QCheckBox *rb4;
    QCheckBox *rb5;
    QCheckBox *rb6;
    QCheckBox *rb7;
    QCheckBox *rb8;
    QWidget *widgetQuery;
    QGridLayout *gridLayout_3;
    QLabel *label_6;
    QDateTimeEdit *dteFrom;
    QLabel *label_8;
    QComboBox *cbxTimeSpan;
    QCheckBox *chkMeasure;
    QLabel *label_7;
    QDateTimeEdit *dteTo;
    QPushButton *btnQuery;
    QPushButton *btnExport;
    QWidget *page;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_11;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QLabel *label_13;
    QLineEdit *edtDataDir;
    QPushButton *btnSelFile;
    QLabel *label_10;
    QComboBox *cbxHost;
    QPushButton *btnReload;
    QTreeWidget *orderList;
    QVBoxLayout *verticalLayout_12;
    QWidget *page4;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *txtLog;
    QGroupBox *groupBox_8;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *btnClear;
    QPushButton *btnPause;
    QPushButton *btnLocalIP;
    QPushButton *btnPing;
    QLineEdit *edtPingIp;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QDialog *MainWnd)
    {
        if (MainWnd->objectName().isEmpty())
            MainWnd->setObjectName(QStringLiteral("MainWnd"));
        MainWnd->resize(1254, 765);
        MainWnd->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(MainWnd);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        widgetTitle = new QWidget(MainWnd);
        widgetTitle->setObjectName(QStringLiteral("widgetTitle"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widgetTitle->sizePolicy().hasHeightForWidth());
        widgetTitle->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(widgetTitle);
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(10, 0, 0, 0);
        labIco = new QLabel(widgetTitle);
        labIco->setObjectName(QStringLiteral("labIco"));
        labIco->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(labIco);

        labTitle = new QLabel(widgetTitle);
        labTitle->setObjectName(QStringLiteral("labTitle"));
        labTitle->setStyleSheet(QStringLiteral(""));
        labTitle->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(labTitle);

        widgetTop = new QWidget(widgetTitle);
        widgetTop->setObjectName(QStringLiteral("widgetTop"));
        horizontalLayout_3 = new QHBoxLayout(widgetTop);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        btnMain = new QToolButton(widgetTop);
        btnMain->setObjectName(QStringLiteral("btnMain"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btnMain->sizePolicy().hasHeightForWidth());
        btnMain->setSizePolicy(sizePolicy1);
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/main_main.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMain->setIcon(icon);
        btnMain->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_3->addWidget(btnMain);

        btnConfig = new QToolButton(widgetTop);
        btnConfig->setObjectName(QStringLiteral("btnConfig"));
        sizePolicy1.setHeightForWidth(btnConfig->sizePolicy().hasHeightForWidth());
        btnConfig->setSizePolicy(sizePolicy1);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/main_config.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnConfig->setIcon(icon1);
        btnConfig->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_3->addWidget(btnConfig);

        btnData = new QToolButton(widgetTop);
        btnData->setObjectName(QStringLiteral("btnData"));
        sizePolicy1.setHeightForWidth(btnData->sizePolicy().hasHeightForWidth());
        btnData->setSizePolicy(sizePolicy1);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/main_data.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnData->setIcon(icon2);
        btnData->setChecked(false);
        btnData->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_3->addWidget(btnData);

        btnReport = new QToolButton(widgetTop);
        btnReport->setObjectName(QStringLiteral("btnReport"));
        sizePolicy1.setHeightForWidth(btnReport->sizePolicy().hasHeightForWidth());
        btnReport->setSizePolicy(sizePolicy1);
        btnReport->setStyleSheet(QStringLiteral(""));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/image/main_help.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnReport->setIcon(icon3);
        btnReport->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_3->addWidget(btnReport);

        btnHelp = new QToolButton(widgetTop);
        btnHelp->setObjectName(QStringLiteral("btnHelp"));
        sizePolicy1.setHeightForWidth(btnHelp->sizePolicy().hasHeightForWidth());
        btnHelp->setSizePolicy(sizePolicy1);
        btnHelp->setStyleSheet(QStringLiteral(""));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/image/main_person.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnHelp->setIcon(icon4);
        btnHelp->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_3->addWidget(btnHelp);

        btnExit = new QToolButton(widgetTop);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        sizePolicy1.setHeightForWidth(btnExit->sizePolicy().hasHeightForWidth());
        btnExit->setSizePolicy(sizePolicy1);
        btnExit->setStyleSheet(QStringLiteral(""));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/image/main_exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnExit->setIcon(icon5);
        btnExit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_3->addWidget(btnExit);


        horizontalLayout_2->addWidget(widgetTop);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        widgetMenu = new QWidget(widgetTitle);
        widgetMenu->setObjectName(QStringLiteral("widgetMenu"));
        gridLayout_2 = new QGridLayout(widgetMenu);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 1, 1, 3);

        btnMenu_Min = new QPushButton(widgetMenu);
        btnMenu_Min->setObjectName(QStringLiteral("btnMenu_Min"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btnMenu_Min->sizePolicy().hasHeightForWidth());
        btnMenu_Min->setSizePolicy(sizePolicy2);
        btnMenu_Min->setMinimumSize(QSize(30, 30));
        btnMenu_Min->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Min->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btnMenu_Min, 0, 1, 1, 1);

        btnMenu_Close = new QPushButton(widgetMenu);
        btnMenu_Close->setObjectName(QStringLiteral("btnMenu_Close"));
        sizePolicy2.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
        btnMenu_Close->setSizePolicy(sizePolicy2);
        btnMenu_Close->setMinimumSize(QSize(30, 30));
        btnMenu_Close->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Close->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btnMenu_Close, 0, 3, 1, 1);

        btnMenu_Max = new QPushButton(widgetMenu);
        btnMenu_Max->setObjectName(QStringLiteral("btnMenu_Max"));
        sizePolicy2.setHeightForWidth(btnMenu_Max->sizePolicy().hasHeightForWidth());
        btnMenu_Max->setSizePolicy(sizePolicy2);
        btnMenu_Max->setMinimumSize(QSize(30, 30));
        btnMenu_Max->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btnMenu_Max, 0, 2, 1, 1);


        horizontalLayout_2->addWidget(widgetMenu);


        verticalLayout->addWidget(widgetTitle);

        stackedWidget = new QStackedWidget(MainWnd);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setStyleSheet(QStringLiteral(""));
        page1 = new QWidget();
        page1->setObjectName(QStringLiteral("page1"));
        horizontalLayout_4 = new QHBoxLayout(page1);
        horizontalLayout_4->setSpacing(2);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(2, 10, 2, 2);
        widgetDocker = new QWidget(page1);
        widgetDocker->setObjectName(QStringLiteral("widgetDocker"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widgetDocker->sizePolicy().hasHeightForWidth());
        widgetDocker->setSizePolicy(sizePolicy3);
        widgetDocker->setMinimumSize(QSize(0, 0));
        widgetDocker->setStyleSheet(QStringLiteral("border:0px solid ;"));
        horizontalLayout = new QHBoxLayout(widgetDocker);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 15, 0, 0);
        treeWidget = new QTreeWidget(widgetDocker);
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::NoBrush);
        QFont font;
        font.setPointSize(12);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setTextAlignment(0, Qt::AlignCenter);
        __qtreewidgetitem->setFont(0, font);
        __qtreewidgetitem->setBackground(0, QColor(255, 255, 255));
        __qtreewidgetitem->setForeground(0, brush);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        sizePolicy1.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy1);
        treeWidget->setMinimumSize(QSize(220, 0));
        treeWidget->setMaximumSize(QSize(200, 16777215));
        treeWidget->setStyleSheet(QStringLiteral("border:0px;"));
        treeWidget->setAutoScroll(false);
        treeWidget->setAnimated(true);
        treeWidget->setColumnCount(1);
        treeWidget->header()->setVisible(false);
        treeWidget->header()->setDefaultSectionSize(180);
        treeWidget->header()->setMinimumSectionSize(38);

        horizontalLayout->addWidget(treeWidget);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        btnShou = new QToolButton(widgetDocker);
        btnShou->setObjectName(QStringLiteral("btnShou"));
        btnShou->setStyleSheet(QLatin1String("background-color: rgba(93, 93, 93, 10);\n"
"border:0px;\n"
"\n"
"QToolButton#btnShou:hover{\n"
"	border:1px solid red;\n"
"}"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/image/shou.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnShou->setIcon(icon6);

        verticalLayout_4->addWidget(btnShou);

        verticalSpacer_6 = new QSpacerItem(20, 498, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_6);


        horizontalLayout->addLayout(verticalLayout_4);


        horizontalLayout_4->addWidget(widgetDocker);

        gbDevices = new QGroupBox(page1);
        gbDevices->setObjectName(QStringLiteral("gbDevices"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(gbDevices->sizePolicy().hasHeightForWidth());
        gbDevices->setSizePolicy(sizePolicy4);
        gbDevices->setStyleSheet(QStringLiteral("background-color: rgb(95, 95, 95);"));
        verticalLayout_14 = new QVBoxLayout(gbDevices);
        verticalLayout_14->setSpacing(0);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        verticalLayout_14->setContentsMargins(0, 0, 0, 0);

        horizontalLayout_4->addWidget(gbDevices);

        stackedWidget->addWidget(page1);
        page2 = new QWidget();
        page2->setObjectName(QStringLiteral("page2"));
        verticalLayout_3 = new QVBoxLayout(page2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 10, -1, -1);
        scrollArea = new QScrollArea(page2);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setMaximumSize(QSize(16777215, 578));
        scrollArea->setWidgetResizable(true);
        scrollArea->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1224, 576));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        basicWidget = new QWidget(scrollAreaWidgetContents);
        basicWidget->setObjectName(QStringLiteral("basicWidget"));
        basicWidget->setMinimumSize(QSize(0, 200));
        basicWidget->setMaximumSize(QSize(16777215, 16777215));
        basicWidget->setStyleSheet(QLatin1String("#basicWidget{\n"
"\n"
"}"));
        verticalLayout_6 = new QVBoxLayout(basicWidget);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(1, 0, 1, -1);
        label_5 = new QLabel(basicWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setMinimumSize(QSize(0, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(18);
        font1.setBold(true);
        font1.setWeight(75);
        label_5->setFont(font1);
        label_5->setStyleSheet(QLatin1String("background-color: rgba(18, 150, 219, 200);\n"
"border-top-left-radius:5px;\n"
"border-top-right-radius:5px;\n"
"border-width:0px;\n"
" border-left-width:1px;\n"
"border-right-width:1px;\n"
"border-style: solid;\n"
"border-left-color: rgb(80, 170, 255);\n"
"border-right-color: rgb(80, 170, 255);"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_5);

        groupBox = new QGroupBox(basicWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(500, 250));
        groupBox->setFont(font1);
        groupBox->setStyleSheet(QLatin1String("QGroupBox {\n"
"	  border-radius:0px;\n"
"	  border-width:1px;\n"
"border-style: solid;\n"
"border-color: rgb(85, 170, 255);\n"
"border-bottom-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"border-left-color:  qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"border-right-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"margin-top:0px;\n"
"}\n"
""));
        groupBox->setFlat(false);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(15);
        gridLayout->setContentsMargins(20, 20, 20, 20);
        edtHost = new QLineEdit(groupBox);
        edtHost->setObjectName(QStringLiteral("edtHost"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(14);
        edtHost->setFont(font2);

        gridLayout->addWidget(edtHost, 0, 2, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setFont(font2);

        gridLayout->addWidget(label_12, 0, 1, 1, 1);

        sbWaveMin = new QSpinBox(groupBox);
        sbWaveMin->setObjectName(QStringLiteral("sbWaveMin"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font3.setPointSize(14);
        font3.setBold(false);
        font3.setWeight(50);
        sbWaveMin->setFont(font3);

        gridLayout->addWidget(sbWaveMin, 2, 2, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font2);

        gridLayout->addWidget(label_4, 3, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font3);

        gridLayout->addWidget(label_3, 2, 1, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font2);

        gridLayout->addWidget(label, 4, 1, 1, 1);

        cbxLang = new QComboBox(groupBox);
        cbxLang->setObjectName(QStringLiteral("cbxLang"));
        cbxLang->setFont(font2);

        gridLayout->addWidget(cbxLang, 3, 2, 1, 1);

        edtPort = new QLineEdit(groupBox);
        edtPort->setObjectName(QStringLiteral("edtPort"));
        edtPort->setFont(font2);

        gridLayout->addWidget(edtPort, 4, 2, 1, 1);

        chkSensorOff = new QCheckBox(groupBox);
        chkSensorOff->setObjectName(QStringLiteral("chkSensorOff"));
        chkSensorOff->setFont(font2);

        gridLayout->addWidget(chkSensorOff, 1, 2, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font2);

        gridLayout->addWidget(label_2, 1, 1, 1, 1);


        verticalLayout_6->addWidget(groupBox);


        horizontalLayout_5->addWidget(basicWidget);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        verticalLayout_7->addLayout(horizontalLayout_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_2);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollArea);

        stackedWidget->addWidget(page2);
        page3 = new QWidget();
        page3->setObjectName(QStringLiteral("page3"));
        horizontalLayout_8 = new QHBoxLayout(page3);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        widgetDocker_2 = new QWidget(page3);
        widgetDocker_2->setObjectName(QStringLiteral("widgetDocker_2"));
        sizePolicy3.setHeightForWidth(widgetDocker_2->sizePolicy().hasHeightForWidth());
        widgetDocker_2->setSizePolicy(sizePolicy3);
        widgetDocker_2->setMinimumSize(QSize(0, 0));
        widgetDocker_2->setStyleSheet(QStringLiteral("border:0px solid ;"));
        horizontalLayout_7 = new QHBoxLayout(widgetDocker_2);
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 15, 0, 0);
        treeWidget2 = new QTreeWidget(widgetDocker_2);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::NoBrush);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setTextAlignment(0, Qt::AlignCenter);
        __qtreewidgetitem1->setFont(0, font);
        __qtreewidgetitem1->setBackground(0, QColor(255, 255, 255));
        __qtreewidgetitem1->setForeground(0, brush1);
        treeWidget2->setHeaderItem(__qtreewidgetitem1);
        treeWidget2->setObjectName(QStringLiteral("treeWidget2"));
        sizePolicy1.setHeightForWidth(treeWidget2->sizePolicy().hasHeightForWidth());
        treeWidget2->setSizePolicy(sizePolicy1);
        treeWidget2->setMinimumSize(QSize(220, 0));
        treeWidget2->setMaximumSize(QSize(200, 16777215));
        treeWidget2->setStyleSheet(QStringLiteral("border:0px;"));
        treeWidget2->setAutoScroll(false);
        treeWidget2->setAnimated(true);
        treeWidget2->setColumnCount(1);
        treeWidget2->header()->setVisible(false);
        treeWidget2->header()->setDefaultSectionSize(180);
        treeWidget2->header()->setMinimumSectionSize(38);

        horizontalLayout_7->addWidget(treeWidget2);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        btnShou2 = new QToolButton(widgetDocker_2);
        btnShou2->setObjectName(QStringLiteral("btnShou2"));
        btnShou2->setStyleSheet(QLatin1String("background-color: rgba(93, 93, 93, 10);\n"
"border:0px;\n"
"\n"
"QToolButton#btnShou:hover{\n"
"	border:1px solid red;\n"
"}"));
        btnShou2->setIcon(icon6);

        verticalLayout_5->addWidget(btnShou2);

        verticalSpacer_7 = new QSpacerItem(20, 498, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_7);


        horizontalLayout_7->addLayout(verticalLayout_5);


        horizontalLayout_8->addWidget(widgetDocker_2);

        gbDevices2 = new QGroupBox(page3);
        gbDevices2->setObjectName(QStringLiteral("gbDevices2"));
        sizePolicy4.setHeightForWidth(gbDevices2->sizePolicy().hasHeightForWidth());
        gbDevices2->setSizePolicy(sizePolicy4);
        gbDevices2->setStyleSheet(QStringLiteral("background-color: rgb(95, 95, 95);"));
        verticalLayout_8 = new QVBoxLayout(gbDevices2);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        plot3 = new QCustomPlot(gbDevices2);
        plot3->setObjectName(QStringLiteral("plot3"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(plot3->sizePolicy().hasHeightForWidth());
        plot3->setSizePolicy(sizePolicy5);
        plot3->setStyleSheet(QStringLiteral("background-color: rgb(0, 255, 127);"));

        horizontalLayout_9->addWidget(plot3);

        widgetChan = new QWidget(gbDevices2);
        widgetChan->setObjectName(QStringLiteral("widgetChan"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(widgetChan->sizePolicy().hasHeightForWidth());
        widgetChan->setSizePolicy(sizePolicy6);
        widgetChan->setMinimumSize(QSize(100, 0));
        widgetChan->setStyleSheet(QLatin1String("\n"
"#widgetChan{\n"
"	border-style:solid;\n"
"	border-width:1px;\n"
"	border-radius:2px;\n"
"	border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"}"));
        verticalLayout_9 = new QVBoxLayout(widgetChan);
        verticalLayout_9->setSpacing(10);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(-1, 10, -1, 50);
        chkSelAll = new QCheckBox(widgetChan);
        chkSelAll->setObjectName(QStringLiteral("chkSelAll"));
        chkSelAll->setFont(font2);

        verticalLayout_9->addWidget(chkSelAll);

        rb1 = new QCheckBox(widgetChan);
        rb1->setObjectName(QStringLiteral("rb1"));
        rb1->setFont(font2);

        verticalLayout_9->addWidget(rb1);

        rb2 = new QCheckBox(widgetChan);
        rb2->setObjectName(QStringLiteral("rb2"));
        rb2->setFont(font2);

        verticalLayout_9->addWidget(rb2);

        rb3 = new QCheckBox(widgetChan);
        rb3->setObjectName(QStringLiteral("rb3"));
        rb3->setFont(font2);

        verticalLayout_9->addWidget(rb3);

        rb4 = new QCheckBox(widgetChan);
        rb4->setObjectName(QStringLiteral("rb4"));
        rb4->setFont(font2);

        verticalLayout_9->addWidget(rb4);

        rb5 = new QCheckBox(widgetChan);
        rb5->setObjectName(QStringLiteral("rb5"));
        rb5->setFont(font2);

        verticalLayout_9->addWidget(rb5);

        rb6 = new QCheckBox(widgetChan);
        rb6->setObjectName(QStringLiteral("rb6"));
        rb6->setFont(font2);

        verticalLayout_9->addWidget(rb6);

        rb7 = new QCheckBox(widgetChan);
        rb7->setObjectName(QStringLiteral("rb7"));
        rb7->setFont(font2);

        verticalLayout_9->addWidget(rb7);

        rb8 = new QCheckBox(widgetChan);
        rb8->setObjectName(QStringLiteral("rb8"));
        rb8->setFont(font2);

        verticalLayout_9->addWidget(rb8);


        horizontalLayout_9->addWidget(widgetChan);


        verticalLayout_8->addLayout(horizontalLayout_9);

        widgetQuery = new QWidget(gbDevices2);
        widgetQuery->setObjectName(QStringLiteral("widgetQuery"));
        widgetQuery->setMinimumSize(QSize(0, 120));
        widgetQuery->setStyleSheet(QLatin1String("\n"
"#widgetQuery{\n"
"	border-style:solid;\n"
"	border-width:1px;\n"
"	border-radius:2px;\n"
"	border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"}"));
        gridLayout_3 = new QGridLayout(widgetQuery);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_6 = new QLabel(widgetQuery);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font2);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_6, 0, 0, 1, 1);

        dteFrom = new QDateTimeEdit(widgetQuery);
        dteFrom->setObjectName(QStringLiteral("dteFrom"));
        dteFrom->setMinimumSize(QSize(0, 30));
        dteFrom->setFont(font2);

        gridLayout_3->addWidget(dteFrom, 0, 1, 1, 1);

        label_8 = new QLabel(widgetQuery);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font2);
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_8, 0, 2, 1, 1);

        cbxTimeSpan = new QComboBox(widgetQuery);
        cbxTimeSpan->setObjectName(QStringLiteral("cbxTimeSpan"));
        cbxTimeSpan->setFont(font2);

        gridLayout_3->addWidget(cbxTimeSpan, 0, 3, 1, 1);

        chkMeasure = new QCheckBox(widgetQuery);
        chkMeasure->setObjectName(QStringLiteral("chkMeasure"));
        chkMeasure->setFont(font2);

        gridLayout_3->addWidget(chkMeasure, 0, 4, 1, 1);

        label_7 = new QLabel(widgetQuery);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font2);
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_7, 1, 0, 1, 1);

        dteTo = new QDateTimeEdit(widgetQuery);
        dteTo->setObjectName(QStringLiteral("dteTo"));
        dteTo->setMinimumSize(QSize(0, 30));
        dteTo->setFont(font2);

        gridLayout_3->addWidget(dteTo, 1, 1, 1, 1);

        btnQuery = new QPushButton(widgetQuery);
        btnQuery->setObjectName(QStringLiteral("btnQuery"));
        btnQuery->setFont(font3);
        btnQuery->setStyleSheet(QLatin1String("QPushButton{\n"
"\n"
"border-raidus:0px;\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"background-color: rgb(0, 170, 255);\n"
"border-width:1px;\n"
"border-style:sold;\n"
"}\n"
"\n"
"QPushButton:hove{\n"
"border-raidus:0px;\n"
"border-style:solid;\n"
"border-width:0px 0px 2px 0px;\n"
"padding:4px 4px 2px 4px;\n"
"border-color:#00BB9E;\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #646464,stop:1 #525252);\n"
"}"));

        gridLayout_3->addWidget(btnQuery, 1, 3, 1, 1);

        btnExport = new QPushButton(widgetQuery);
        btnExport->setObjectName(QStringLiteral("btnExport"));
        btnExport->setFont(font3);
        btnExport->setStyleSheet(QLatin1String("QPushButton{\n"
"\n"
"border-raidus:0px;\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"background-color: rgb(0, 170, 255);\n"
"border-width:1px;\n"
"border-style:sold;\n"
"}\n"
"\n"
"QPushButton:hove{\n"
"border-raidus:0px;\n"
"border-style:solid;\n"
"border-width:0px 0px 2px 0px;\n"
"padding:4px 4px 2px 4px;\n"
"border-color:#00BB9E;\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #646464,stop:1 #525252);\n"
"}"));

        gridLayout_3->addWidget(btnExport, 1, 4, 1, 1);


        verticalLayout_8->addWidget(widgetQuery);


        horizontalLayout_8->addWidget(gbDevices2);

        stackedWidget->addWidget(page3);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        horizontalLayout_10 = new QHBoxLayout(page);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        groupBox_3 = new QGroupBox(page);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(0, 120));
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setFont(font2);
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_13, 0, 0, 1, 2);

        edtDataDir = new QLineEdit(groupBox_3);
        edtDataDir->setObjectName(QStringLiteral("edtDataDir"));
        edtDataDir->setFont(font2);

        gridLayout_4->addWidget(edtDataDir, 0, 2, 1, 1);

        btnSelFile = new QPushButton(groupBox_3);
        btnSelFile->setObjectName(QStringLiteral("btnSelFile"));
        btnSelFile->setFont(font2);

        gridLayout_4->addWidget(btnSelFile, 0, 3, 1, 1);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font2);
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_10, 1, 0, 1, 1);

        cbxHost = new QComboBox(groupBox_3);
        cbxHost->setObjectName(QStringLiteral("cbxHost"));
        cbxHost->setFont(font2);

        gridLayout_4->addWidget(cbxHost, 1, 2, 1, 1);

        btnReload = new QPushButton(groupBox_3);
        btnReload->setObjectName(QStringLiteral("btnReload"));
        btnReload->setFont(font2);
        btnReload->setStyleSheet(QStringLiteral(""));

        gridLayout_4->addWidget(btnReload, 1, 3, 1, 1);


        verticalLayout_11->addWidget(groupBox_3);

        orderList = new QTreeWidget(page);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem();
        __qtreewidgetitem2->setTextAlignment(5, Qt::AlignCenter);
        __qtreewidgetitem2->setFont(5, font);
        __qtreewidgetitem2->setTextAlignment(4, Qt::AlignCenter);
        __qtreewidgetitem2->setFont(4, font);
        __qtreewidgetitem2->setTextAlignment(3, Qt::AlignCenter);
        __qtreewidgetitem2->setFont(3, font);
        __qtreewidgetitem2->setTextAlignment(2, Qt::AlignCenter);
        __qtreewidgetitem2->setFont(2, font);
        __qtreewidgetitem2->setTextAlignment(1, Qt::AlignCenter);
        __qtreewidgetitem2->setFont(1, font);
        __qtreewidgetitem2->setTextAlignment(0, Qt::AlignCenter);
        __qtreewidgetitem2->setFont(0, font);
        orderList->setHeaderItem(__qtreewidgetitem2);
        orderList->setObjectName(QStringLiteral("orderList"));
        orderList->setStyleSheet(QStringLiteral(""));

        verticalLayout_11->addWidget(orderList);


        horizontalLayout_10->addLayout(verticalLayout_11);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));

        horizontalLayout_10->addLayout(verticalLayout_12);

        stackedWidget->addWidget(page);
        page4 = new QWidget();
        page4->setObjectName(QStringLiteral("page4"));
        verticalLayout_2 = new QVBoxLayout(page4);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        txtLog = new QTextEdit(page4);
        txtLog->setObjectName(QStringLiteral("txtLog"));

        verticalLayout_2->addWidget(txtLog);

        groupBox_8 = new QGroupBox(page4);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setMinimumSize(QSize(0, 80));
        horizontalLayout_6 = new QHBoxLayout(groupBox_8);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        btnClear = new QPushButton(groupBox_8);
        btnClear->setObjectName(QStringLiteral("btnClear"));
        btnClear->setMinimumSize(QSize(0, 50));

        horizontalLayout_6->addWidget(btnClear);

        btnPause = new QPushButton(groupBox_8);
        btnPause->setObjectName(QStringLiteral("btnPause"));
        btnPause->setMinimumSize(QSize(0, 50));

        horizontalLayout_6->addWidget(btnPause);

        btnLocalIP = new QPushButton(groupBox_8);
        btnLocalIP->setObjectName(QStringLiteral("btnLocalIP"));
        btnLocalIP->setMinimumSize(QSize(0, 50));

        horizontalLayout_6->addWidget(btnLocalIP);

        btnPing = new QPushButton(groupBox_8);
        btnPing->setObjectName(QStringLiteral("btnPing"));
        btnPing->setMinimumSize(QSize(0, 50));

        horizontalLayout_6->addWidget(btnPing);

        edtPingIp = new QLineEdit(groupBox_8);
        edtPingIp->setObjectName(QStringLiteral("edtPingIp"));
        edtPingIp->setMinimumSize(QSize(250, 40));

        horizontalLayout_6->addWidget(edtPingIp);

        horizontalSpacer_3 = new QSpacerItem(864, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);


        verticalLayout_2->addWidget(groupBox_8);

        stackedWidget->addWidget(page4);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(MainWnd);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWnd);
    } // setupUi

    void retranslateUi(QDialog *MainWnd)
    {
        MainWnd->setWindowTitle(QApplication::translate("MainWnd", "Form", 0));
        labIco->setText(QString());
        labTitle->setText(QApplication::translate("MainWnd", "\350\206\250\350\203\200\345\212\233\345\210\206\346\236\220\347\263\273\347\273\237  ", 0));
        btnMain->setText(QApplication::translate("MainWnd", "\344\270\273\347\225\214\351\235\242", 0));
        btnConfig->setText(QApplication::translate("MainWnd", "\347\263\273\347\273\237\350\256\276\347\275\256", 0));
        btnData->setText(QApplication::translate("MainWnd", "\350\256\260\345\275\225\346\237\245\350\257\242", 0));
        btnReport->setText(QApplication::translate("MainWnd", "\347\224\237\346\210\220\346\212\245\350\241\250", 0));
        btnHelp->setText(QApplication::translate("MainWnd", "\350\260\203\350\257\225\345\270\256\345\212\251", 0));
        btnExit->setText(QApplication::translate("MainWnd", "\347\224\250\346\210\267\351\200\200\345\207\272", 0));
#ifndef QT_NO_TOOLTIP
        btnMenu_Min->setToolTip(QApplication::translate("MainWnd", "\346\234\200\345\260\217\345\214\226", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Min->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Close->setToolTip(QApplication::translate("MainWnd", "\345\205\263\351\227\255", 0));
#endif // QT_NO_TOOLTIP
        btnMenu_Close->setText(QString());
        btnMenu_Max->setText(QString());
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWnd", "\350\256\276\345\244\207\345\210\227\350\241\250", 0));
        btnShou->setText(QApplication::translate("MainWnd", "...", 0));
        gbDevices->setTitle(QString());
        label_5->setText(QApplication::translate("MainWnd", "\345\237\272\346\234\254\351\205\215\347\275\256", 0));
        groupBox->setTitle(QString());
        label_12->setText(QApplication::translate("MainWnd", "\344\270\273\346\234\272\345\220\215\347\247\260", 0));
        label_4->setText(QApplication::translate("MainWnd", "\347\263\273\347\273\237\350\257\255\350\250\200", 0));
        label_3->setText(QApplication::translate("MainWnd", "\345\256\236\346\227\266\346\263\242\345\275\242\347\274\223\345\255\230\346\227\266\351\227\264\357\274\210\345\210\206\351\222\237)", 0));
        label->setText(QApplication::translate("MainWnd", "\347\233\221\345\220\254\347\253\257\345\217\243", 0));
        cbxLang->clear();
        cbxLang->insertItems(0, QStringList()
         << QApplication::translate("MainWnd", "\344\270\255\346\226\207", 0)
        );
        chkSensorOff->setText(QString());
        label_2->setText(QApplication::translate("MainWnd", "\346\216\245\346\224\266\344\274\240\346\204\237\345\231\250\347\251\272\350\275\275\345\216\213\345\212\233\345\200\274", 0));
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget2->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWnd", "\350\256\276\345\244\207\345\210\227\350\241\250", 0));
        btnShou2->setText(QApplication::translate("MainWnd", "...", 0));
        gbDevices2->setTitle(QString());
        chkSelAll->setText(QApplication::translate("MainWnd", "\345\205\250\351\200\211", 0));
        rb1->setText(QApplication::translate("MainWnd", "\351\200\232\351\201\2231", 0));
        rb2->setText(QApplication::translate("MainWnd", "\351\200\232\351\201\2232", 0));
        rb3->setText(QApplication::translate("MainWnd", "\351\200\232\351\201\2233", 0));
        rb4->setText(QApplication::translate("MainWnd", "\351\200\232\351\201\2234", 0));
        rb5->setText(QApplication::translate("MainWnd", "\351\200\232\351\201\2235", 0));
        rb6->setText(QApplication::translate("MainWnd", "\351\200\232\351\201\2236", 0));
        rb7->setText(QApplication::translate("MainWnd", "\351\200\232\351\201\2237", 0));
        rb8->setText(QApplication::translate("MainWnd", "\351\200\232\351\201\2238", 0));
        label_6->setText(QApplication::translate("MainWnd", "\345\274\200\345\247\213\346\227\266\351\227\264", 0));
        label_8->setText(QApplication::translate("MainWnd", "\346\227\266\351\227\264\350\214\203\345\233\264", 0));
        cbxTimeSpan->clear();
        cbxTimeSpan->insertItems(0, QStringList()
         << QApplication::translate("MainWnd", "\346\234\200\350\277\22110\345\210\206\351\222\237", 0)
         << QApplication::translate("MainWnd", "\346\234\200\350\277\22120\345\210\206\351\222\237", 0)
         << QApplication::translate("MainWnd", "\346\234\200\350\277\2211\344\270\252\345\260\217\346\227\266", 0)
         << QApplication::translate("MainWnd", "\346\234\200\350\277\2212\345\260\217\346\227\266", 0)
         << QApplication::translate("MainWnd", "\346\234\200\350\277\2215\345\260\217\346\227\266", 0)
         << QApplication::translate("MainWnd", "\346\234\200\350\277\22110\345\260\217\346\227\266", 0)
        );
        chkMeasure->setText(QApplication::translate("MainWnd", "\345\220\257\347\224\250\346\265\213\351\207\217\345\267\245\345\205\267", 0));
        label_7->setText(QApplication::translate("MainWnd", "\347\273\223\346\235\237\346\227\266\351\227\264", 0));
        btnQuery->setText(QApplication::translate("MainWnd", "\346\237\245 \350\257\242", 0));
        btnExport->setText(QApplication::translate("MainWnd", "\345\257\274\345\207\272\346\225\260\346\215\256", 0));
        groupBox_3->setTitle(QApplication::translate("MainWnd", "\351\200\211\346\213\251", 0));
        label_13->setText(QApplication::translate("MainWnd", "\346\216\247\345\210\266\346\237\234\346\226\207\344\273\266\347\233\256\345\275\225", 0));
        btnSelFile->setText(QApplication::translate("MainWnd", "\351\200\211\346\213\251...", 0));
        label_10->setText(QApplication::translate("MainWnd", "\344\270\273\346\234\272\345\220\215\347\247\260", 0));
        btnReload->setText(QApplication::translate("MainWnd", "\345\210\267\346\226\260", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = orderList->headerItem();
        ___qtreewidgetitem2->setText(5, QApplication::translate("MainWnd", "\347\212\266\346\200\201", 0));
        ___qtreewidgetitem2->setText(4, QApplication::translate("MainWnd", "\346\211\223\345\274\200\347\233\256\345\275\225", 0));
        ___qtreewidgetitem2->setText(3, QApplication::translate("MainWnd", "\347\224\237\346\210\220\346\212\245\350\241\250", 0));
        ___qtreewidgetitem2->setText(2, QApplication::translate("MainWnd", "\347\224\265\350\212\257\344\270\252\346\225\260", 0));
        ___qtreewidgetitem2->setText(1, QApplication::translate("MainWnd", "\346\270\251\345\272\246", 0));
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWnd", "\350\256\242\345\215\225\347\274\226\345\217\267", 0));
        groupBox_8->setTitle(QString());
        btnClear->setText(QApplication::translate("MainWnd", "\346\270\205\347\251\272", 0));
        btnPause->setText(QApplication::translate("MainWnd", "\346\232\202\345\201\234", 0));
        btnLocalIP->setText(QApplication::translate("MainWnd", "\346\234\254\346\234\272\345\234\260\345\235\200", 0));
        btnPing->setText(QApplication::translate("MainWnd", "Ping", 0));
        edtPingIp->setText(QApplication::translate("MainWnd", "127.0.0.1", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWnd: public Ui_MainWnd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWND_H
