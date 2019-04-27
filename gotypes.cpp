#include "gotypes.h"



QGoString::QGoString(QString str):
    m_string(str)
{

}

GoString QGoString::toGoString()
{
    GoString gs;
    m_std_string = m_string.toStdString();
    gs.p = m_std_string.c_str();
    gs.n = strlen(gs.p);
    return gs;
}
