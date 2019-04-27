#include "dialogreport.h"
#include "ui_dialogreport.h"

DialogReport::DialogReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogReport)
{
    ui->setupUi(this);
}

DialogReport::~DialogReport()
{
    delete ui;
}

void DialogReport::on_btnOk_clicked()
{

}
