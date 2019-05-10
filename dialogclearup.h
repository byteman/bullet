#ifndef DIALOGCLEARUP_H
#define DIALOGCLEARUP_H

#include <QDialog>

namespace Ui {
class DialogClearUp;
}

class DialogClearUp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogClearUp(QWidget *parent = 0);
    ~DialogClearUp();
    void SetSerialNo(QString dev);

private slots:
    void on_btnExit_clicked();

    void on_btnClearUp_clicked();

private:
    Ui::DialogClearUp *ui;
    QString m_serialNo;
};

#endif // DIALOGCLEARUP_H
