#ifndef DIALOGCHECK_H
#define DIALOGCHECK_H

#include <QDialog>
#include <QList>
#include "orderchecker.h"
#include <QPushButton>
#include "httpclient.h"
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
namespace Ui {
class DialogCheck;
}

class DialogCheck : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCheck(QWidget *parent = nullptr);
    ~DialogCheck();
    void Check(QString file);

protected slots:
    void on_report_click(int index);
    void on_query_gen_response(AjaxResponse r);
    void handleLoadWaveFinished();
private slots:
    void on_cbxCells_currentIndexChanged(const QString &arg1);

    void on_checkBox_clicked(bool checked);

    void on_btnAdd_clicked();

    void on_pushButton_clicked();

private:
    Ui::DialogCheck *ui;
    QList<QWidget*> items;
    QString m_file;
    Ajax ajax;
    int m_count;

    OrderInfo oi;
    bool FixData(QString cellNo,CellDataSeg &cell,QPushButton* btn);
    void requestGen(QString cellNo, CellDataSeg &cell);
    void reload();
    void LoadWave();
};

#endif // DIALOGCHECK_H
