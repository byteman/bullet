#include "statemanager.h"

StateManager::StateManager()
{

}

bool StateManager::parseOrder(QXlsx::Document &xlsx,
                              QString sheet,
                              CellTestOrder& order)
{
    //选中要分析的列表
    xlsx.selectSheet(sheet);
    return true;
}
//分析文件
bool StateManager::parse(QString filename)
{
    QXlsx::Document xlsx(filename);

    QStringList sheets =  xlsx.sheetNames();
   //读取有多少个主机列表.
    for(int i = 0; i < sheets.size(); i++)
    {
        CellTestOrder order;
        //分析每台电脑上的订单.
        hosts[sheets[i]] = order;
    }
    return true;

}

