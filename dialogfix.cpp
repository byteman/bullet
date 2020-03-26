#include "dialogfix.h"
#include "ui_dialogfix.h"
#include "qtcsv/writer.h"
#include "qtcsv/reader.h"
#include "qtcsv/stringdata.h"
#include <QFileDialog>
#include <QDebug>
#include <QRandomGenerator>
#include "utils.h"
DialogFix::DialogFix(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFix)
{
    ui->setupUi(this);
}

DialogFix::~DialogFix()
{
    delete ui;
}

void DialogFix::on_btnGen_clicked()
{
    QString file = QFileDialog::getSaveFileName(this,QStringLiteral("请选择文件保存路径"),QString(),"*.csv");
    if(file.isEmpty()){
        return;
    }

    QtCSV::StringData strData;
    QStringList strHeader;
    QString chargeV = "0";
    QString disV =" 0";
    int charStart  = ui->sbChargeBegin->value();
    int charEnd = ui->sbChargeEnd->value();
    int disStart = ui->sbDisChargeBegin->value();
    int disEnd = ui->sbDischargeEnd->value();
    float span = ui->edtSpan->text().toDouble();
    float charDiff = float(charEnd - charStart) / data.size();
    float disDiff  = float(disEnd - disStart)   / data.size();
    float charVv = charStart;
    float disVv = disStart;
    int v1 = -span;
    int v2 = span;

    bool zflag = ui->checkBox->isChecked();
    for (int i=0; i < data.size(); i++) {
        double value = 0;
        if(zflag){
           value= QRandomGenerator::global()->bounded(v1,v2);
        }else{
           value= QRandomGenerator::global()->bounded(span);
        }
        chargeV = QString("%1").arg(int(charVv+value));
        if(data[i].size()<4){
            data[i] <<  chargeV;
        }else{
            data[i][3] = chargeV;
        }
        disV = QString("%1").arg(int(disVv+value));
        if(data[i].size()<5){
            data[i] <<  disV;
        }else{
            data[i][4] = disV;
        }
        strData.addRow(data[i]);
        int percent = (i * 100) /data.size();
        ui->btnGen->setText(QString("%1").arg(percent));
        QApplication::processEvents();
        charVv+= charDiff;
        disVv += disDiff;
    }

    QtCSV::Writer::write(file, strData,QString(","),QString("\""),QtCSV::Writer::REWRITE,QStringList());
    ui->btnGen->setText(QStringLiteral("已经完成"));
}

void DialogFix::on_btnLoad_clicked()
{

    QString file = QFileDialog::getOpenFileName(this,QStringLiteral("请选择文件保存路径"),QString(),"*.csv");

    data = QtCSV::Reader::readToList(file);
    ui->sbInt->setValue(data.size());
    if(data.size() < 1 ){
        return;
    }
    QStringList start = data.first();
    QStringList end = data.last();
    if(start.size() >= 5){
        ui->sbChargeBegin->setValue(start[3].toInt());
        ui->sbChargeEnd->setValue(end[3].toInt());

    }

    if(end.size() >= 5){
        ui->sbDisChargeBegin->setValue(start[4].toInt());
        ui->sbDischargeEnd->setValue(end[4].toInt());

    }

}
