#ifndef DIALOGSELECTCHAN_H
#define DIALOGSELECTCHAN_H

#include <QDialog>

namespace Ui {
class DialogSelectChan;
}

class DialogSelectChan : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectChan(QWidget *parent = 0);
    ~DialogSelectChan();

private:
    Ui::DialogSelectChan *ui;
};

#endif // DIALOGSELECTCHAN_H
