#ifndef DIALOGCLEARUP_H
#define DIALOGCLEARUP_H

#include <QDialog>
#include <QFuture>
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


    bool ClearUp(QString id, qint64 from, qint64 to);
protected slots:
    void clearUpFinished();
private slots:
    void on_btnExit_clicked();

    void on_btnClearUp_clicked();

    void on_btnClearAll_clicked();

private:
    Ui::DialogClearUp *ui;
    QString m_serialNo;
    QFutureWatcher<bool> *watcher;
};

#endif // DIALOGCLEARUP_H
