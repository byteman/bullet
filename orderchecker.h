#ifndef ORDERCHECKER_H
#define ORDERCHECKER_H

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

struct CellDataSeg{
    qint32 start_circelNo;
    qint32 end_circelNo;
    qint32 crcelNo;
    qint64 start_ts;
    qint64 end_ts;
    int charge_start_value;
    int charge_end_value;
    int discharge_start_value;
    int discharge_end_value;
    double random;
    bool zflag;
    //QString cellNo;
    void clear()
    {
        start_circelNo = 0;
        end_circelNo = 0;
    }
};
//一个条码丢失的所有数据段
typedef  QList<CellDataSeg>         CellDataSegList;
//一个订单所有的条码的数据段.
typedef  QMap<QString,CellDataSegList> OrderInfo;

class OrderChecker
{
public:
    OrderChecker();
    bool Check(QString orderName,OrderInfo& orderInfo);
private:
    void parseCell(QXlsx::Document &xlsx, QString sheet, CellDataSegList &list);
    bool getCellString(QXlsx::Document &xlsx, int row, int col, QString &value);

};

#endif // ORDERCHECKER_H
