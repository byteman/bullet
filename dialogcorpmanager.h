#ifndef DIALOGCORPMANAGER_H
#define DIALOGCORPMANAGER_H

#include <QDialog>

namespace Ui {
class DialogCorpManager;
}

class DialogCorpManager : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCorpManager(QWidget *parent = nullptr);
    ~DialogCorpManager();

private slots:
    void on_btnClose_clicked();

    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

private:
    Ui::DialogCorpManager *ui;
    bool GetCorpName(QString &name);
    void reload();
};

#endif // DIALOGCORPMANAGER_H
