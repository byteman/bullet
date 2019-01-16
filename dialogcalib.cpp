#include "dialogcalib.h"
#include "ui_dialogcalib.h"

DialogCalib::DialogCalib(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCalib)
{
    ui->setupUi(this);
}

DialogCalib::~DialogCalib()
{
    delete ui;
}
