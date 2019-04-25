#ifndef MERGEMANAGER_H
#define MERGEMANAGER_H
#include <QObject>
#include <QVector>

/**
 * @brief The MergeInfo struct
 * {
 *    "task":"1234",
 *     "options":[
 *          {
 *              "batty_code":"123",
 *              "dev_id":"1234",
 *              "dev_chan":1,
 *              "batty_file":"file"
 *          },
 *  *          {
 *              "batty_code":"123",
 *              "dev_id":"1234",
 *              "dev_chan":2,
 *              "batty_file":"file"
 *          },
 *      ],
 * }
 */
struct MergeInfo{
    QString dev; //设备
    int chan; //通道
    QString file;//对应的文件.
    QString code; //电芯的条码
};
struct MergeTask
{
    QString report; //报告文件的路径
    QVector<MergeInfo> MergeList;//合并
};
class MergeManager
{
public:
    MergeManager();
};

#endif // MERGEMANAGER_H
