#ifndef DIALOGCALIB_H
#define DIALOGCALIB_H

#include <QDialog>

namespace Ui {
class DialogCalib;
}

class DialogCalib : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCalib(QWidget *parent = 0);
    ~DialogCalib();

private:
    Ui::DialogCalib *ui;
};

#endif // DIALOGCALIB_H
