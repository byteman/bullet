#ifndef WIDGETCHAN_H
#define WIDGETCHAN_H

#include <QWidget>

namespace Ui {
class WidgetChan;
}

class WidgetChan : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetChan(QWidget *parent = 0);
    ~WidgetChan();

private:
    Ui::WidgetChan *ui;
};

#endif // WIDGETCHAN_H
