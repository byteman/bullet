#include "dialogmerge.h"
#include "ui_dialogmerge.h"
#include "chanselect.h"
DialogMerge::DialogMerge(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMerge)
{
    ui->setupUi(this);
}

DialogMerge::~DialogMerge()
{
    delete ui;
}

void DialogMerge::SetDevice(QString dev, QVector<int> chans)
{
    m_dev = dev;
    m_chans = chans;
    QVBoxLayout* layout =  new QVBoxLayout(ui->groupBox);
    layout->addStretch();
    for(int i = 0; i < chans.size(); i++)
    {
        ChanSelect* chan = new ChanSelect(ui->groupBox);
        chan->SetDevice(dev,chans[i]);
        ui->groupBox->layout()->addWidget(chan);
    }
    ui->groupBox->setLayout(layout);

}

void DialogMerge::on_btnOk_clicked()
{
    this->accept();
}

void DialogMerge::on_btnCancel_clicked()
{
    this->reject();
}
