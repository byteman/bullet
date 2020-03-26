#include "dialogcheck.h"
#include "ui_dialogcheck.h"
#include "widgetsegment.h"
#include <QDebug>
#include "widgetsegment.h"
#include <QSignalMapper>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLineEdit>
#include <QPushButton>
DialogCheck::DialogCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCheck)
{
    ui->setupUi(this);
    m_count = 0;
    qDebug() << "DialogCheck";

}

DialogCheck::~DialogCheck()
{
    qDebug() << "~~DialogCheck";

    delete ui;
}


void DialogCheck::reload()
{
    OrderInfo::const_iterator i = oi.constBegin();
    ui->cbxCells->clear();

    while (i != oi.constEnd()) {
        if(!ui->checkBox->isChecked()){
            if(i.value().size() > 0){
                ui->cbxCells->addItem(i.key());
            }
        }else{
            ui->cbxCells->addItem(i.key());
        }
        i++;
    }
}
void DialogCheck::handleLoadWaveFinished()
{

}

void DialogCheck::Check(QString file)
{
    OrderChecker checker;
    checker.Check(file,oi);
    reload();
    m_file = file;
    this->exec();
}

void DialogCheck::on_cbxCells_currentIndexChanged(const QString &arg1)
{
    QSignalMapper *signalMapper = new QSignalMapper(this);

    if(oi.contains(arg1)){
        CellDataSegList list = oi[arg1];
        qDebug() << "count=" << list.size();
        ui->treeWidget->clear();


        for(int i = 0; i < list.size();i++)
        {
            QStringList ll;
            ll << "11" << "2";
            QTreeWidgetItem* item = new QTreeWidgetItem();

            qDebug() << "add";
            QLineEdit* edt_start = new QLineEdit(ui->treeWidget);
            edt_start->setText(QString("%1").arg(list[i].start_circelNo));
            //w->setGeometry(0,0,800,40);
            QLineEdit* edt_stop = new QLineEdit(ui->treeWidget);
            edt_stop->setText(QString("%1").arg(list[i].end_circelNo));

            QLineEdit* edt_charge_start = new QLineEdit(ui->treeWidget);
            edt_charge_start->setText("0");

            QLineEdit* edt_charge_end = new QLineEdit(ui->treeWidget);
            edt_charge_end->setText("0");

            QLineEdit* edt_dis_charge_start = new QLineEdit(ui->treeWidget);
            edt_dis_charge_start->setText("0");

            QLineEdit* edt_dis_charge_end = new QLineEdit(ui->treeWidget);
            edt_dis_charge_end->setText("0");

            QLineEdit* edt_diff = new QLineEdit(ui->treeWidget);
            edt_diff->setText("0");

            QPushButton* btnGen = new QPushButton(ui->treeWidget);
            btnGen->setText(QStringLiteral("查看"));
            btnGen->setEnabled(m_count>5);
            connect(btnGen, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(btnGen, int(i));

            ui->treeWidget->addTopLevelItem(item);
            ui->treeWidget->setItemWidget(item, 0, edt_start);
            ui->treeWidget->setItemWidget(item, 1, edt_stop);
            ui->treeWidget->setItemWidget(item, 2, edt_charge_start);
            ui->treeWidget->setItemWidget(item, 3, edt_charge_end);
            ui->treeWidget->setItemWidget(item, 4, edt_dis_charge_start);
            ui->treeWidget->setItemWidget(item, 5, edt_dis_charge_end);
            ui->treeWidget->setItemWidget(item, 6, edt_diff);
            ui->treeWidget->setItemWidget(item, 7, btnGen);

        }
        connect(signalMapper, SIGNAL(mapped(int)),
                    this, SLOT(on_report_click(int)));

    }

}
void DialogCheck::on_query_gen_response(AjaxResponse r)
{
    if(r.error){
        return;
    }
    QJsonDocument doc;

    if(QJsonParseError::NoError == r.GetJSON(doc).error)
    {

    }


    qDebug() << r.error;
}
void DialogCheck::requestGen(QString cellNo,CellDataSeg &cell)
{
    QJsonDocument body;

    QJsonObject o;
    o["cell_no"] = cellNo;
    o["file_name"] = m_file;
    o["start_circle"] = cell.start_circelNo;
    o["end_circle"] = cell.end_circelNo;
    o["start_circle1"] = cell.start_circelNo+2;
    o["end_circle1"] = cell.end_circelNo+2;
    o["charge_start_value"] = cell.charge_start_value;
    o["charge_end_value"] = cell.charge_end_value;
    o["discharge_start_value"] = cell.discharge_start_value;
    o["discharge_end_value"] = cell.discharge_end_value;
    o["random"] = cell.random;
    o["z_flag"] = cell.zflag;

    body.setObject(o);
    ajax.post("http://localhost:9527/FixData",body,this,SLOT(on_query_gen_response(AjaxResponse)));

}

void DialogCheck::on_report_click(int index)
{
    QTreeWidgetItem* item = ui->treeWidget->topLevelItem(index);
    if(item!=NULL){
        CellDataSeg cell;

       QLineEdit* w = qobject_cast<QLineEdit*>(ui->treeWidget->itemWidget(item,0));
       cell.start_circelNo = w->text().toInt();
       w = qobject_cast<QLineEdit*>(ui->treeWidget->itemWidget(item,1));
       cell.end_circelNo = w->text().toInt();
       w = qobject_cast<QLineEdit*>(ui->treeWidget->itemWidget(item,2));
       cell.charge_start_value = w->text().toInt();
       w = qobject_cast<QLineEdit*>(ui->treeWidget->itemWidget(item,3));
       cell.charge_end_value = w->text().toInt();
       w = qobject_cast<QLineEdit*>(ui->treeWidget->itemWidget(item,4));
       cell.discharge_start_value = w->text().toInt();
       w = qobject_cast<QLineEdit*>(ui->treeWidget->itemWidget(item,5));
       cell.discharge_end_value = w->text().toInt();
       w = qobject_cast<QLineEdit*>(ui->treeWidget->itemWidget(item,6));
       cell.random = w->text().toDouble();
       //FixData(ui->cbxCells->currentText(), cell,qobject_cast<QPushButton*>(ui->treeWidget->itemWidget(item,7)));
        requestGen(ui->cbxCells->currentText(), cell);
    }
    qDebug() << "click" << index;
}

void DialogCheck::on_checkBox_clicked(bool checked)
{
    reload();
}

void DialogCheck::on_btnAdd_clicked()
{

    QString cell = ui->cbxCells->currentText();
    if(oi.contains(cell)){
        CellDataSeg seg;
        seg.clear();
        oi[cell].push_back(seg);
        on_cbxCells_currentIndexChanged(cell);
    }
}

void DialogCheck::on_pushButton_clicked()
{
    m_count++;
    if(m_count > 5){
        for(int i =0; i < ui->treeWidget->topLevelItemCount();i++)
        {
            QTreeWidgetItem * item = ui->treeWidget->topLevelItem(i);
            if(item!=NULL)
            {
                 QWidget* w = ui->treeWidget->itemWidget(item, 7);
                 if(w!=NULL){
                     w->setEnabled(true);
                 }
            }
        }
    }
}
