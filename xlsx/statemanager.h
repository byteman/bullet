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
private:
    CellTestHost hosts;
    bool parseOrder(QXlsx::Document &xlsx, QString sheet, CellTestOrder &order);
};

#endif // STATEMANAGER_H
