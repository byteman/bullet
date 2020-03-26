#include "orderchecker.h"
#include <QDebug>
OrderChecker::OrderChecker()
{

}
bool OrderChecker::getCellString(QXlsx::Document &xlsx,
                               int row,
                               int col,
                               QString& value)
{
    QXlsx::Cell* cell = xlsx.cellAt(row,col);
    if(cell==NULL){
        qDebug("load cell failed row%d col%d",row,col);
        return false;
    }
    value = cell->readValue().toString();

    return true;
}
#include <QApplication>
void OrderChecker::parseCell(QXlsx::Document& xlsx,QString sheet,CellDataSegList& list)
{
    if(!xlsx.selectSheet(sheet)){
        qDebug() << "selectSheet " <<sheet << "failed";
        return;
    }

    int row=1;
    QString loop;
    QString maxV;
    QString minV;
    bool isEmpty = false;
    CellDataSeg seg;
    do{

        row++;
        QApplication::processEvents();
       // qDebug() << "row=" << row;
        //先判断压力通道是否存在并有效，否则就认为分析完了，跳出分析过程.长度至少大于=3 [x_y]
        if(!getCellString(xlsx,row,1,loop) || loop.isEmpty()){
            //如果取不到循环圈数就表示完毕了.
            return;
        }
        if(!getCellString(xlsx,row,4,maxV) ){
            maxV="";
        }
        if(!getCellString(xlsx,row,5,minV)){
            minV="";
        }
        //qDebug() << maxV << minV;

        if(maxV.isEmpty() && minV.isEmpty())
        {

            if(!isEmpty){
                //第一次出现空洞
                seg.clear();
                seg.start_circelNo = row;
                qDebug() << "find empty start" << loop;
                QString cts;
                getCellString(xlsx,row,2,cts);
                seg.start_ts = cts.toInt();

            }
            isEmpty = true;
        }else{
            if(isEmpty){
                //空洞完毕
                seg.end_circelNo = row;
                QString cts;
                qDebug() << "find empty end" << loop;
                getCellString(xlsx,row,2,cts);
                seg.end_ts = cts.toInt();
                qDebug() << seg.start_circelNo << seg.end_circelNo;
                list.push_back(seg);
            }
            isEmpty = false;
        }
    }while(1);

}
#include <QApplication>
bool OrderChecker::Check(QString orderName, OrderInfo &orderInfo)
{
    QXlsx::Document xlsx(orderName);
    if (!xlsx.load()) {
        qDebug() << "load error";
        return false;
    }

    qDebug() << "------222222222222";
    QStringList sheets =  xlsx.sheetNames();
    if(sheets.size() < 3) return false;
     qDebug() << "------33333333333 == " << sheets.size();



   //读取有多少个主机列表.
    for(int i = 2; i < sheets.size(); i++)
    {

        QString sheet = sheets[i];
        qDebug() << "sheet" << i << sheet;

        CellDataSegList cdl;

        parseCell(xlsx,sheet,cdl);
        //分析每台电脑上的订单.
        //if(cdl.size() > 0){
            orderInfo[sheet] = cdl;
        //}

    }

}
