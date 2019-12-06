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
        return false;
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
 * @brief 分析一个订单,订单名和温度作为唯一的key.
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
        //先判断压力通道是否存在并有效，否则就认为分析完了，跳出分析过程.长度至少大于=3 [x_y]
        if(!getCellString(xlsx,row,5,cs.PressDev) || cs.PressDev.length() < 3){
            //如果获取不到压力设备和通道，说明读取完成.
            qDebug("------------parse sheet[%s] PressDev=%s row=%d finished!",
                   sheet.toStdString().c_str(),
                   cs.PressDev.toStdString().c_str(),
                   row);
            return true;
        }
        //压力通道有值，分析其有效性.通道值无效，只是跳过而不退出.
        if(!parsePressDev(cs)){
            qDebug("sheet[%s] parsePressDev[%s] row=%d col=%d failed",
                   sheet.toStdString().c_str(),
                   cs.PressDev.toStdString().c_str(),
                   row,5);
            continue;
        }
        //有效的通道号，再去找单号，不能把单号查找放前面，否则会导致无法跳出循环.
        if(!getCellString(xlsx,row,2,cs.TestNo) || cs.TestNo.length() < 1){
            //如果测试单号为空，跳过这个单号，继续找下一个单号.
            qDebug("sheet[%s] testNo empty [row=%d col=%d]",sheet.toStdString().c_str(),row,2);
            continue;
        }
        if(!getCellString(xlsx,row,7,cs.CellNo) || cs.CellNo.length() < 1){
            //如果电芯为空，跳过这个电芯
            qDebug("sheet[%s] cellNo empty [row=%d col=%d]",sheet.toStdString().c_str(),row,7);
            continue;
        }
        //只要存在订单号和压力通道号和电芯条码存在的情况下才认为是有效的电芯数据.
        getCellString(xlsx,row,1,cs.Temp); //温度
        getCellString(xlsx,row,3,cs.TestUser); //测试人
        getCellString(xlsx,row,4,cs.ProjGroup); //项目组
        getCellString(xlsx,row,6,cs.ChargeDev); //充放电设备和通道
        getCellString(xlsx,row,8,cs.TestState); //在柜状态.

        QString order = cs.TestNo +"-"+ cs.Temp; //订单温度唯一编号.
        orders[order].push_back(cs);


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
    xlsx.saveAs(filename+".bak");
    return true;

}

CellTestHost &StateManager::GetState()
{
    return hosts;

}

CellTestOrderList &StateManager::GetOrderList(QString &host)
{
    static CellTestOrderList list;
    if(!hosts.contains(host)){
        list.clear();
        return list;
    }
    return hosts[host];
}

