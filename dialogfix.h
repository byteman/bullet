#ifndef DIALOGFIX_H
#define DIALOGFIX_H

#include <QDialog>
#include <QList>
#include <QCheckBox>
#include <QMap>
namespace Ui {
class DialogFix;
}

class DialogFix : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFix(QWidget *parent = nullptr);
    ~DialogFix();

private slots:
    void on_btnGen_clicked();

    void on_btnLoad_clicked();

private:
    Ui::DialogFix *ui;
    QList<QStringList> data;
};

#endif // DIALOGFIX_H
