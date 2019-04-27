#ifndef GOTYPES_H
#define GOTYPES_H
#include "main.h"
#include <QObject>
class QGoString
{
public:
    QGoString(QString str);
    GoString toGoString();
private:
    QString m_string;
    std::string m_std_string;
};

#endif // GOTYPES_H
