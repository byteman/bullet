#include "websetclient.h"
#include <QGroupBox>
#include <QLabel>
#include <QFile>
#include <QNetworkInterface>
#include <QProcess>
#include <QMessageBox>
#include <stdio.h>
#include <windows.h>
#include <Iphlpapi.h>
 
WebSetClient::WebSetClient(QWidget *parent)
    : QMainWindow(parent)
{
    //布局
    initLayout();
 
    //连接信号
    connectSignals();
 
    //显示Ip网卡等信息
    showIP();
    showwNetMask();
    showGateWay();
    showDNS();
}
 
 
void WebSetClient:: initLayout()
{
 
    QHBoxLayout *hbox1 = new QHBoxLayout;
    QLabel *titleLabel = new QLabel(QStringLiteral("网络设置"));
    m_closebutton = new QPushButton;
    m_closebutton->setIcon(QIcon(":/image/close"));
 
    QWidget *widget1 = new QWidget;
    widget1->setLayout(hbox1);
    hbox1->addWidget(titleLabel);
    hbox1->addStretch();
    hbox1->addWidget(m_closebutton);
 
    QGroupBox *infoGroupBox = new QGroupBox;
    infoGroupBox->setTitle(QStringLiteral("配置信息"));
 
    QVBoxLayout *vbox2 = new QVBoxLayout;
    infoGroupBox->setLayout(vbox2);
    //选择网卡+本地连接，hbox11
    QLabel *slctNetCard = new QLabel(QStringLiteral("选择网卡"));
    m_NetCardComboBox = new QComboBox;
    QHBoxLayout *hbox11 = new QHBoxLayout;
    hbox11->addWidget(slctNetCard,1);
    hbox11->addWidget(m_NetCardComboBox,3);
    //ip地址设置,vbox12
    QGroupBox *ipGroupBox = new QGroupBox;
    ipGroupBox->setTitle(QStringLiteral("IP地址设置"));
    QVBoxLayout *vbox12 = new QVBoxLayout;
    ipGroupBox->setLayout(vbox12);
    QHBoxLayout *hbox21 = new QHBoxLayout;
    QHBoxLayout *hbox22 = new QHBoxLayout;
    QHBoxLayout *hbox23 = new QHBoxLayout;
    QHBoxLayout *hbox24 = new QHBoxLayout;
    vbox12->addLayout(hbox21,1);
    vbox12->addLayout(hbox22,1);
    vbox12->addLayout(hbox23,1);
    vbox12->addLayout(hbox24,1);
    m_AutoIPRadioButton = new QRadioButton;
    m_AutoIPRadioButton->setText(QStringLiteral("自动获取IP地址"));
    m_SpecifiedIPRadioButton = new QRadioButton;
    m_SpecifiedIPRadioButton->setText(QStringLiteral("使用指定IP地址"));
    m_SpecifiedIPRadioButton->setChecked(true);
    hbox21->addWidget(m_AutoIPRadioButton);
    hbox21->addStretch(1);
    hbox21->addWidget(m_SpecifiedIPRadioButton);
    hbox21->addStretch(2);
 
    QLabel *ipLabel = new QLabel(QStringLiteral("  IP地址"));
    ipLabel->setAlignment(Qt::AlignRight);
    m_IPLineedit = new QLineEdit;
    hbox22->addWidget(ipLabel,2);
    hbox22->addWidget(m_IPLineedit,4);
    hbox22->addStretch(2);
 
    QLabel *maskLabel = new QLabel(QStringLiteral("子网掩码"));
    maskLabel->setAlignment(Qt::AlignRight);
    m_SubNetMaskLineedit = new QLineEdit;
    hbox23->addWidget(maskLabel,2);
    hbox23->addWidget(m_SubNetMaskLineedit,4);
    hbox23->addStretch(2);
 
    QLabel *gatewayLabel = new QLabel(QStringLiteral("默认网关"));
    gatewayLabel->setAlignment(Qt::AlignRight);
    m_DefaultGatewayLineedit = new QLineEdit;
    hbox24->addWidget(gatewayLabel,2);
    hbox24->addWidget(m_DefaultGatewayLineedit,4);
    hbox24->addStretch(2);
    //dns服务器设置,vbox13
    QGroupBox *dnsGroupBox = new QGroupBox;
    dnsGroupBox->setTitle(QStringLiteral("DNS服务器设置"));
    QVBoxLayout *vbox13 = new QVBoxLayout;
    dnsGroupBox->setLayout(vbox13);
 
    QHBoxLayout *hbox131 = new QHBoxLayout;
    QHBoxLayout *hbox132 = new QHBoxLayout;
    vbox13->addLayout(hbox131);
    vbox13->addLayout(hbox132);
 
    m_AutoDNSRadioButton = new QRadioButton;
    m_AutoDNSRadioButton->setText(QStringLiteral("自动获取DNS服务器"));
    m_AutoDNSRadioButton->setEnabled(false);
    m_SpecifiedDNSRadioButton = new QRadioButton;
    m_SpecifiedDNSRadioButton->setText(QStringLiteral("使用指定DNS服务器"));
    m_SpecifiedDNSRadioButton->setChecked(true);
    hbox131->addWidget(m_AutoDNSRadioButton);
    hbox131->addStretch(1);
    hbox131->addWidget(m_SpecifiedDNSRadioButton);
    hbox131->addStretch(2);
 
    QLabel *dnsLabel = new QLabel(QStringLiteral("DNS服务器"));
    dnsLabel->setAlignment(Qt::AlignRight);
    m_DNSLineedit = new QLineEdit;
    hbox132->addWidget(dnsLabel,2);
    hbox132->addWidget(m_DNSLineedit,4);
    hbox132->addStretch(2);
 
    vbox2->addLayout(hbox11,1);
    vbox2->addWidget(ipGroupBox,4);
    vbox2->addWidget(dnsGroupBox,2);
 
    //应用,取消
    QHBoxLayout *hbox3 = new QHBoxLayout;
    QWidget *widget3 = new QWidget;
    widget3->setLayout(hbox3);
    m_ApplyButton = new QPushButton(QStringLiteral("应用设置"));
    m_CancelButton = new QPushButton(QStringLiteral("取消"));
    hbox3->addStretch();
    hbox3->addWidget(m_ApplyButton);
    hbox3->addWidget(m_CancelButton);
    //
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(widget1);
    mainLayout->addWidget(infoGroupBox);
    mainLayout->addWidget(widget3);
 
    QWidget *widget = new QWidget;
    this->setCentralWidget(widget);
    widget->setLayout(mainLayout);
 
    //qss样式
    m_ApplyButton->setStyleSheet("QPushButton{background:#20B2AA;"
                                 "color:white;}");
    m_CancelButton->setStyleSheet("QPushButton{background:#20B2AA;"
                                  "color:white;}");
    widget1->setStyleSheet("QWidget{background-color:#00868B;}");
    widget3->setStyleSheet("QWidget{background-color:#00868B;}");
    titleLabel->setStyleSheet("QLabel{color:white;}");
    m_closebutton->setStyleSheet("QPushButton{background-color:#00868B;border:none;color:white;}");
 
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(450,400);
}
 
 
void WebSetClient::connectSignals()
{
    connect(m_closebutton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(m_CancelButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(m_AutoIPRadioButton,SIGNAL(clicked(bool)),this,SLOT(onAutoIPButtonclicked()));
    connect(m_SpecifiedIPRadioButton,SIGNAL(clicked(bool)),this,SLOT(onSpIpButtonclicked()));
    connect(m_AutoDNSRadioButton,SIGNAL(clicked(bool)),this,SLOT(onAutoDNSButtonclicked()));
    connect(m_SpecifiedDNSRadioButton,SIGNAL(clicked(bool)),this,SLOT(onSpDNSButtonclicked()));
    connect(m_ApplyButton,SIGNAL(clicked(bool)),this,SLOT(onApplyButtonclicked()));
}
 
 
void WebSetClient::showIP()
{
    // 获取第一个本主机的IPv4地址
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    int nListSize = ipAddressesList.size();
    QString IP;
    for (int i = 0; i < nListSize; ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            IP = ipAddressesList.at(1).toString();
            m_IPLineedit->setText(IP);
        }
    }
    if (IP.isEmpty())
        m_IPLineedit->setText(QHostAddress(QHostAddress::LocalHost).toString());
}
void WebSetClient::showwNetMask(){
    //筛选可用的非环路接口
    QList<QNetworkInterface> ifaceList = QNetworkInterface::allInterfaces();
    for (int i = 0; i < ifaceList.count(); i++)
    {
        QNetworkInterface var = ifaceList.at(i);
        //获取子网掩码
        QList<QNetworkAddressEntry> entryList = var.addressEntries();
        foreach(QNetworkAddressEntry entry,entryList){
            m_SubNetMaskLineedit->setText(entry.netmask().toString());
        }
        if(var.isValid()
                && !var.flags().testFlag(QNetworkInterface::IsLoopBack)
                && var.flags().testFlag(QNetworkInterface::IsRunning))
        {
            m_NetCardComboBox->addItem(var.humanReadableName());
        }
    }
}
void WebSetClient::showGateWay(){
    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);
    int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
    if (ERROR_BUFFER_OVERFLOW == nRel){
        delete pIpAdapterInfo;
        pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
        GetAdaptersInfo(pIpAdapterInfo,&stSize);
    }
    m_DefaultGatewayLineedit->setText(pIpAdapterInfo->GatewayList.IpAddress.String);
}
void WebSetClient::showDNS(){
    FIXED_INFO *FixedInfo = (FIXED_INFO *) GlobalAlloc(GPTR, sizeof(FIXED_INFO));
    unsigned long ulOutBufLen = sizeof(FIXED_INFO);
    GetNetworkParams(FixedInfo, &ulOutBufLen);
    m_DNSLineedit->setText(FixedInfo->DnsServerList.IpAddress.String);
}
WebSetClient::~WebSetClient()
{
 
}
void WebSetClient::onAutoIPButtonclicked(){
    m_IPLineedit->setEnabled(false);
    m_SubNetMaskLineedit->setEnabled(false);
    m_DefaultGatewayLineedit->setEnabled(false);
    m_SpecifiedDNSRadioButton->setChecked(true);
    m_AutoDNSRadioButton->setEnabled(true);
    m_IPLineedit->setText(" . . . ");
    m_DefaultGatewayLineedit->setText(" . . . ");
    m_SubNetMaskLineedit->setText(" . . . ");
}
void WebSetClient::onSpIpButtonclicked(){
    m_IPLineedit->setEnabled(true);
    m_SubNetMaskLineedit->setEnabled(true);
    m_DefaultGatewayLineedit->setEnabled(true);
    m_SpecifiedDNSRadioButton->setChecked(true);
    m_AutoDNSRadioButton->setEnabled(false);
}
void WebSetClient::onAutoDNSButtonclicked(){
    m_DNSLineedit->setEnabled(false);
    m_DNSLineedit->setText(" . . . ");
}
void WebSetClient::onSpDNSButtonclicked(){
    m_DNSLineedit->setEnabled(true);
}
void WebSetClient::onApplyButtonclicked(){
    if(m_SpecifiedIPRadioButton->isChecked()){
        QString name = m_NetCardComboBox->currentText();
        QString ip = m_IPLineedit->text();
        QString netmask = m_SubNetMaskLineedit->text();
        QString gateway = m_DefaultGatewayLineedit->text();
        QString dns = m_DNSLineedit->text();
        QProcess p1(this);
        QProcess p2(this);
        QString ipmaskgateway = "netsh interface ip set address " + name + " static "+ ip + QString(" ") + netmask + QString(" ") + gateway + " 1";
        QString adddns = "netsh interface ip set dns " + name + " static " + dns;
        QStringList arguments1;
        QStringList arguments2;
        arguments1 << "/c" << ipmaskgateway;
        arguments2 << "/c" << adddns;
        p1.start("cmd",arguments1);
        p1.waitForFinished(3000);
        p2.start("cmd",arguments2);
        p2.waitForFinished(3000);
        QString strResult1 = QString::fromLocal8Bit(p1.readAllStandardOutput());
        QString strResult2 = QString::fromLocal8Bit(p2.readAllStandardOutput());
        QMessageBox msgBox(this);
        msgBox.setText("IP: "+ strResult1 + "DNS: "+ strResult2);
        msgBox.exec();
    }
    else if (m_AutoIPRadioButton->isChecked())
    {
        QMessageBox msg(this);
        msg.setText(QStringLiteral("暂不支持"));
        msg.exec();
    }
}
