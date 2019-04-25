#include "chanselect.h"
#include "ui_chanselect.h"
#include <QFileDialog>
ChanSelect::ChanSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChanSelect)
{
    ui->setupUi(this);
}

ChanSelect::~ChanSelect()
{
    delete ui;
}

QString ChanSelect::GetFileName()
{
    return ui->edtFile->text();
}

int ChanSelect::GetChanNr()
{
    return m_index;
}

QString ChanSelect::GetDeviceId()
{
    return m_dev;
}

void ChanSelect::SetDevice(QString id, int index)
{
    ui->lblChan->setText(QString("通道%1").arg(index));
    m_index = index;
    m_dev = id;
}

void ChanSelect::on_btnSelFile_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,QStringLiteral("请选择文件"));
    if(file.length() != 0){
        m_dev = file;
        ui->edtFile->setText(file);
    }
}
