#include "dialogselectchan.h"
#include "ui_dialogselectchan.h"

DialogSelectChan::DialogSelectChan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectChan)
{
    ui->setupUi(this);
}

DialogSelectChan::~DialogSelectChan()
{
    delete ui;
}
