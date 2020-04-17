#ifndef CSVTHREAD_H
#define CSVTHREAD_H
#include <QThread>
/**
 * @brief The CSVThread class
 *  负责在线程中写入数据
 */

class CSVThread:public QObject
{
    Q_OBJECT
public:
    CSVThread();

public slots:
    void writeData(QString);
};

#endif // CSVTHREAD_H
