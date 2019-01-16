#include "dialogparams.h"
#include "ui_dialogparams.h"

DialogParams::DialogParams(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogParams)
{
    ui->setupUi(this);
}

DialogParams::~DialogParams()
{
    delete ui;
}
