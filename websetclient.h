
#ifndef WEBSETCLIENT_HPP
#define WEBSETCLIENT_HPP
 
#include <QMainWindow>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>
//#include "ipcontrol.hpp"
class WebSetClient : public QMainWindow
{
    Q_OBJECT
 
public:
    WebSetClient(QWidget *parent = 0);
    ~WebSetClient();
 
private:
    void initLayout();
    void connectSignals();
    void showIP();
    void showGateWay();
    void showwNetMask();
    void showDNS();
private:
    QLineEdit *m_IPLineedit;
    QLineEdit *m_SubNetMaskLineedit;
    QLineEdit *m_DefaultGatewayLineedit;
    QLineEdit *m_DNSLineedit;
    QRadioButton *m_AutoIPRadioButton;
    QRadioButton *m_SpecifiedIPRadioButton;
    QRadioButton *m_AutoDNSRadioButton;
    QRadioButton *m_SpecifiedDNSRadioButton;
    QPushButton *m_ApplyButton;
    QPushButton *m_CancelButton;
    QComboBox *m_NetCardComboBox;
    QPushButton *m_closebutton;
public slots:
    void onAutoIPButtonclicked();
    void onSpIpButtonclicked();
    void onAutoDNSButtonclicked();
    void onSpDNSButtonclicked();
    void onApplyButtonclicked();
};

#endif
