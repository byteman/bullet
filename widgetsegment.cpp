#include "widgetsegment.h"
#include "ui_widgetsegment.h"

widgetsegment::widgetsegment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widgetsegment)
{
    ui->setupUi(this);
}

widgetsegment::~widgetsegment()
{
    delete ui;
}
