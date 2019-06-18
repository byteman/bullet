#ifndef FORMGDS_H
#define FORMGDS_H

#include <QWidget>

namespace Ui {
class FormGds;
}

class FormGds : public QWidget
{
    Q_OBJECT

public:
    explicit FormGds(QWidget *parent = 0);
    ~FormGds();

private:
    Ui::FormGds *ui;
};

#endif // FORMGDS_H
