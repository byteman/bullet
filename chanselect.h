#ifndef CHANSELECT_H
#define CHANSELECT_H

#include <QWidget>

namespace Ui {
class ChanSelect;
}

class ChanSelect : public QWidget
{
    Q_OBJECT

public:
    explicit ChanSelect(QWidget *parent = 0);
    ~ChanSelect();
    QString GetFileName();
    int GetChanNr();
    QString GetDeviceId();
    void SetDevice(QString id,int index);
private slots:
    void on_btnSelFile_clicked();

private:
    Ui::ChanSelect *ui;
    QString m_dev;
    int m_index;
};

#endif // CHANSELECT_H
