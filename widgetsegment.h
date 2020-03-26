#ifndef WIDGETSEGMENT_H
#define WIDGETSEGMENT_H

#include <QWidget>

namespace Ui {
class widgetsegment;
}

class widgetsegment : public QWidget
{
    Q_OBJECT

public:
    explicit widgetsegment(QWidget *parent = nullptr);
    ~widgetsegment();

private:
    Ui::widgetsegment *ui;
};

#endif // WIDGETSEGMENT_H
