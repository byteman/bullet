/********************************************************************************
** Form generated from reading UI file 'dialogreport.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGREPORT_H
#define UI_DIALOGREPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogReport
{
public:
    QPushButton *btnOk;
    QPushButton *pushButton_2;

    void setupUi(QDialog *DialogReport)
    {
        if (DialogReport->objectName().isEmpty())
            DialogReport->setObjectName(QStringLiteral("DialogReport"));
        DialogReport->resize(737, 533);
        btnOk = new QPushButton(DialogReport);
        btnOk->setObjectName(QStringLiteral("btnOk"));
        btnOk->setGeometry(QRect(430, 473, 112, 41));
        pushButton_2 = new QPushButton(DialogReport);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(570, 473, 112, 41));

        retranslateUi(DialogReport);

        QMetaObject::connectSlotsByName(DialogReport);
    } // setupUi

    void retranslateUi(QDialog *DialogReport)
    {
        DialogReport->setWindowTitle(QApplication::translate("DialogReport", "Dialog", 0));
        btnOk->setText(QApplication::translate("DialogReport", "\347\241\256\345\256\232", 0));
        pushButton_2->setText(QApplication::translate("DialogReport", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogReport: public Ui_DialogReport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGREPORT_H
