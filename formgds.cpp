#include "formgds.h"
#include "ui_formgds.h"

FormGds::FormGds(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormGds)
{
    ui->setupUi(this);
}

FormGds::~FormGds()
{
    delete ui;
}
