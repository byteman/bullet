#ifndef DIALOGPARAMS_H
#define DIALOGPARAMS_H

#include <QDialog>

namespace Ui {
class DialogParams;
}

class DialogParams : public QDialog
{
    Q_OBJECT

public:
    explicit DialogParams(QWidget *parent = 0);
    ~DialogParams();

private:
    Ui::DialogParams *ui;
};

#endif // DIALOGPARAMS_H
