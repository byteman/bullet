#include "widgetchan.h"
#include "ui_widgetchan.h"

WidgetChan::WidgetChan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetChan)
{
    ui->setupUi(this);
}

WidgetChan::~WidgetChan()
{
    delete ui;
}
