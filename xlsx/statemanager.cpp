#include "statemanager.h"
#include <QDebug>
#include "singletonholder.h"
StateManager::StateManager()
{

}

StateManager &StateManager::instance()
{
    static SingletonHolder<StateManager> sh;
    return *sh.get();
}
bool StateManager::getCellString(QXlsx::Document &xlsx,
                               int row,
                               int col,
                               QString& value)
{
    QXlsx::Cell* cell = xlsx.cellAt(row,col);
    if(cell==NULL){
        qDebug("load cell failed row%d col%d",row,col);
        return true;
    }
    value = cell->readValue().toString();

    return true;
}
bool StateManager::parsePressDev(CellState &cs)
{
    QStringList dev_group = cs.PressDev.split("_");
    if(dev_group.size() != 2){
        return false;
    }
    bool ok = false;
    cs.PressDevId = dev_group[0];
    cs.PressDevChan  = dev_group[1].toInt(&ok);
    return ok;
}
/**
 * @brief 分析一个订单
 * @param xlsx
 * @param sheet
 * @param order
 * @return
 */
bool StateManager::parseOrder(QXlsx::Document &xlsx,
                              QString sheet,
                              CellTestOrderList& orders)
{
    //选中要分析的列表

    xlsx.selectSheet(sheet);
    int row=1;

    do{
        CellState cs;
        row++;
        if(!getCellString(xlsx,row,1,cs.Temp) || cs.Temp.length() < 1){
            //如果获取不到温度，说明读取完成.
            qDebug("read %s finished!",sheet.toStdString().c_str());
            return true;
        }

        if(!getCellString(xlsx,row,2,cs.TestNo) || cs.TestNo.length() < 1){
            //qDebug("get %s testNo row=%d col=%d failed",sheet.toStdString().c_str(),row,2);
            continue;
        }
        getCellString(xlsx,row,3,cs.TestUser);
        getCellString(xlsx,row,4,cs.ProjGroup);
        getCellString(xlsx,row,5,cs.PressDev);
        if(!parsePressDev(cs)){
            //qDebug("get %s parsePressDev row=%d col=%d failed",sheet.toStdString().c_str(),row,5);
            continue;
        }
        getCellString(xlsx,row,6,cs.ChargeDev);
        getCellString(xlsx,row,7,cs.CellNo);
        getCellString(xlsx,row,8,cs.TestState);

        orders[cs.TestNo].push_back(cs);


    }while(true);
    //xlsx.cellAt()
    return true;
}
//分析文件
bool StateManager::parse(QString filename)
{
    QTime tm;
    tm.start();

    QXlsx::Document xlsx(filename);
    if (!xlsx.load()) return false;
    qDebug() << "load elasped" << tm.elapsed();
    hosts.clear();
    QStringList sheets =  xlsx.sheetNames();
   //读取有多少个主机列表.
    for(int i = 0; i < sheets.size()-1; i++)
    {
        CellTestOrderList order;
        QString sheet = sheets[i];
        tm.restart();
        parseOrder(xlsx,sheet, order);
        qDebug() << "load elasped" << tm.elapsed();
        //分析每台电脑上的订单.
        hosts[sheet] = order;
    }
    return true;

}

CellTestHost &StateManager::GetState()
{
    return hosts;

}

CellTestOrderList &StateManager::GetOrderList(QString &host)
{
    if(!hosts.contains(host)){
        return CellTestOrderList();
    }
    return hosts[host];
}

