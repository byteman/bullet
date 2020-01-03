#ifndef DIALOGFIXVALUE_H
#define DIALOGFIXVALUE_H

#include <QDialog>
#include <QLineEdit>
#include <QList>
namespace Ui {
class DialogFixValue;
}

class DialogFixValue : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFixValue(QWidget *parent = nullptr);
    ~DialogFixValue();
    void SetDeviceId(QString id);

private slots:
    void on_btnFix_clicked();

    void on_edtComm_textChanged(const QString &arg1);

    void on_btnExit_clicked();

private:
    Ui::DialogFixValue *ui;
    QString device_id;
    QList<QLineEdit*> chanEdit;
};

#endif // DIALOGFIXVALUE_H
