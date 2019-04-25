#ifndef DIALOGMERGE_H
#define DIALOGMERGE_H

#include <QDialog>


namespace Ui {
class DialogMerge;
}

class DialogMerge : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMerge(QWidget *parent = 0);
    ~DialogMerge();
    void SetDevice(QString dev, QVector<int> chans);

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DialogMerge *ui;
    QVector<int> m_chans;
    QString m_dev;
};

#endif // DIALOGMERGE_H
