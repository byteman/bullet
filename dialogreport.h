#ifndef DIALOGREPORT_H
#define DIALOGREPORT_H

#include <QDialog>

namespace Ui {
class DialogReport;
}

class DialogReport : public QDialog
{
    Q_OBJECT

public:
    explicit DialogReport(QWidget *parent = 0);
    ~DialogReport();

private slots:
    void on_btnOk_clicked();

private:
    Ui::DialogReport *ui;
};

#endif // DIALOGREPORT_H
