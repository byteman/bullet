/********************************************************************************
** Form generated from reading UI file 'widgetchan.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGETCHAN_H
#define UI_WIDGETCHAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WidgetChan
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QToolButton *toolButton;

    void setupUi(QWidget *WidgetChan)
    {
        if (WidgetChan->objectName().isEmpty())
            WidgetChan->setObjectName(QStringLiteral("WidgetChan"));
        WidgetChan->resize(288, 192);
        widget = new QWidget(WidgetChan);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 0, 281, 26));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        toolButton = new QToolButton(widget);
        toolButton->setObjectName(QStringLiteral("toolButton"));

        horizontalLayout->addWidget(toolButton);


        retranslateUi(WidgetChan);

        QMetaObject::connectSlotsByName(WidgetChan);
    } // setupUi

    void retranslateUi(QWidget *WidgetChan)
    {
        WidgetChan->setWindowTitle(QApplication::translate("WidgetChan", "Form", 0));
        label->setText(QApplication::translate("WidgetChan", "\350\256\276\345\244\207:\351\200\232\351\201\223", 0));
        toolButton->setText(QApplication::translate("WidgetChan", "\350\256\276\347\275\256", 0));
    } // retranslateUi

};

namespace Ui {
    class WidgetChan: public Ui_WidgetChan {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGETCHAN_H
