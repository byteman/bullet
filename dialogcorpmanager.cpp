#include "dialogcorpmanager.h"
#include "ui_dialogcorpmanager.h"
#include "config.h"
#include <QInputDialog>
#include "myhelper.h"
DialogCorpManager::DialogCorpManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCorpManager)
{
    ui->setupUi(this);
    reload();
}
bool DialogCorpManager::GetCorpName(QString &name)
{
    name = QInputDialog::getText(this,
                          QStringLiteral("请输入"),
                          QStringLiteral("请输入实验室名称"));

    return true;
}

DialogCorpManager::~DialogCorpManager()
{
    delete ui;
}

void DialogCorpManager::on_btnClose_clicked()
{
    this->accept();
}
void DialogCorpManager::reload()
{
    QStringList& list = Config::instance().m_corp_list;
    ui->listWidget->clear();

    ui->listWidget->addItems(list);

}
void DialogCorpManager::on_btnAdd_clicked()
{
    QString corp ;
    if(GetCorpName(corp)){
        if(corp.length() < 2){
           myHelper::ShowMessageBoxError(QStringLiteral("名称太短"));
           return;
        }
    }
    if(Config::instance().AddCorpName(corp)){

    }
    reload();
}

void DialogCorpManager::on_btnDelete_clicked()
{
    int index = ui->listWidget->currentRow();
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(index == -1 ||item==nullptr||item->text().isEmpty()){
        myHelper::ShowMessageBoxError(QStringLiteral("请先选中你要删除的实验室名称"));
        return;
    }
    if(Config::instance().DeleteCorpName(item->text())){

    }
    reload();

}
