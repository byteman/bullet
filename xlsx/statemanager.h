#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QObject>
#include "stateinfo.h"
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
class StateManager
{
public:
    StateManager();
    static StateManager& instance();
    bool parse(QString filename);
    CellTestHost& GetState();
    CellTestOrderList& GetOrderList(QString& host);
    QVector<CellState>& GetCells(QString order);
private:
    CellTestHost hosts;
    bool parseOrder(QXlsx::Document &xlsx,
                    QString sheet,
                    CellTestOrderList &order);

    bool getCell(QXlsx::Document &xlsx, int row, int col, QVariant &value);
    bool getCellString(QXlsx::Document &xlsx, int row, int col, QString &value);
    bool parsePressDev(CellState &cs);
};

#endif // STATEMANAGER_H
