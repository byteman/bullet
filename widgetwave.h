#ifndef WIDGETWAVE_H
#define WIDGETWAVE_H

#include <QWidget>
#include "wavewidget.h"
#include "protomessage.h"
namespace Ui {
class WidgetWave;
}

class WidgetWave : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetWave(QWidget *parent = 0);
    ~WidgetWave();
    void Append();
    void DisplayWeight(MsgSensorData &data);
    void ShowMe(bool en);
private:
    Ui::WidgetWave *ui;
    WaveWidget* wave;
};

#endif // WIDGETWAVE_H
