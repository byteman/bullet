/********************************************************************************
** Form generated from reading UI file 'dialogclearup.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCLEARUP_H
#define UI_DIALOGCLEARUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogClearUp
{
public:
    QDateTimeEdit *dtData;
    QLabel *label;
    QLabel *label_2;
    QPushButton *btnClearUp;
    QPushButton *btnExit;
    QLabel *lblstart;
    QLabel *lblend;
    QLabel *lblcount;
    QLabel *lblend_2;
    QLabel *lblstart_2;
    QLabel *lblcount_2;

    void setupUi(QDialog *DialogClearUp)
    {
        if (DialogClearUp->objectName().isEmpty())
            DialogClearUp->setObjectName(QStringLiteral("DialogClearUp"));
        DialogClearUp->resize(681, 358);
        dtData = new QDateTimeEdit(DialogClearUp);
        dtData->setObjectName(QStringLiteral("dtData"));
        dtData->setGeometry(QRect(180, 174, 261, 51));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(14);
        dtData->setFont(font);
        label = new QLabel(DialogClearUp);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 179, 91, 41));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label_2 = new QLabel(DialogClearUp);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(470, 180, 201, 41));
        label_2->setFont(font1);
        btnClearUp = new QPushButton(DialogClearUp);
        btnClearUp->setObjectName(QStringLiteral("btnClearUp"));
        btnClearUp->setGeometry(QRect(50, 270, 271, 51));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setWeight(75);
        btnClearUp->setFont(font2);
        btnExit = new QPushButton(DialogClearUp);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        btnExit->setGeometry(QRect(380, 270, 191, 51));
        btnExit->setFont(font2);
        lblstart = new QLabel(DialogClearUp);
        lblstart->setObjectName(QStringLiteral("lblstart"));
        lblstart->setGeometry(QRect(340, 10, 211, 41));
        lblstart->setFont(font1);
        lblend = new QLabel(DialogClearUp);
        lblend->setObjectName(QStringLiteral("lblend"));
        lblend->setGeometry(QRect(340, 60, 231, 41));
        lblend->setFont(font1);
        lblcount = new QLabel(DialogClearUp);
        lblcount->setObjectName(QStringLiteral("lblcount"));
        lblcount->setGeometry(QRect(340, 110, 221, 41));
        lblcount->setFont(font1);
        lblend_2 = new QLabel(DialogClearUp);
        lblend_2->setObjectName(QStringLiteral("lblend_2"));
        lblend_2->setGeometry(QRect(170, 60, 231, 41));
        lblend_2->setFont(font1);
        lblstart_2 = new QLabel(DialogClearUp);
        lblstart_2->setObjectName(QStringLiteral("lblstart_2"));
        lblstart_2->setGeometry(QRect(170, 10, 211, 41));
        lblstart_2->setFont(font1);
        lblcount_2 = new QLabel(DialogClearUp);
        lblcount_2->setObjectName(QStringLiteral("lblcount_2"));
        lblcount_2->setGeometry(QRect(170, 110, 151, 41));
        lblcount_2->setFont(font1);

        retranslateUi(DialogClearUp);

        QMetaObject::connectSlotsByName(DialogClearUp);
    } // setupUi

    void retranslateUi(QDialog *DialogClearUp)
    {
        DialogClearUp->setWindowTitle(QApplication::translate("DialogClearUp", "\346\270\205\347\220\206\346\225\260\346\215\256", 0));
        label->setText(QApplication::translate("DialogClearUp", "\346\270\205\351\231\244", 0));
        label_2->setText(QApplication::translate("DialogClearUp", "\344\271\213\345\211\215\347\232\204\346\225\260\346\215\256", 0));
        btnClearUp->setText(QApplication::translate("DialogClearUp", "\346\270\205\347\220\206", 0));
        btnExit->setText(QApplication::translate("DialogClearUp", "\351\200\200\345\207\272", 0));
        lblstart->setText(QApplication::translate("DialogClearUp", "\346\234\200\350\200\201\346\225\260\346\215\256", 0));
        lblend->setText(QApplication::translate("DialogClearUp", "\346\234\200\346\226\260\346\225\260\346\215\256", 0));
        lblcount->setText(QApplication::translate("DialogClearUp", "\346\225\260\346\215\256\346\235\241\346\225\260", 0));
        lblend_2->setText(QApplication::translate("DialogClearUp", "\346\234\200\346\226\260\346\225\260\346\215\256\357\274\232", 0));
        lblstart_2->setText(QApplication::translate("DialogClearUp", "\346\234\200\350\200\201\346\225\260\346\215\256\357\274\232", 0));
        lblcount_2->setText(QApplication::translate("DialogClearUp", "\346\225\260\346\215\256\346\235\241\346\225\260\357\274\232", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogClearUp: public Ui_DialogClearUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCLEARUP_H
