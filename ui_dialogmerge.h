/********************************************************************************
** Form generated from reading UI file 'dialogmerge.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGMERGE_H
#define UI_DIALOGMERGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogMerge
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QSpacerItem *verticalSpacer;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    void setupUi(QDialog *DialogMerge)
    {
        if (DialogMerge->objectName().isEmpty())
            DialogMerge->setObjectName(QStringLiteral("DialogMerge"));
        DialogMerge->resize(612, 656);
        verticalLayout_2 = new QVBoxLayout(DialogMerge);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(DialogMerge);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(12);
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_3);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_4);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(14);
        label_5->setFont(font1);

        horizontalLayout_2->addWidget(label_5);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 460, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        widget = new QWidget(DialogMerge);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMinimumSize(QSize(0, 80));
        widget->setMaximumSize(QSize(100000, 80));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnOk = new QPushButton(widget);
        btnOk->setObjectName(QStringLiteral("btnOk"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btnOk->sizePolicy().hasHeightForWidth());
        btnOk->setSizePolicy(sizePolicy2);
        btnOk->setMinimumSize(QSize(0, 0));
        btnOk->setFont(font1);
        btnOk->setStyleSheet(QLatin1String("QPushButton{\n"
"\n"
"border-raidus:0px;\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"background-color: rgb(0, 170, 255);\n"
"border-width:1px;\n"
"border-style:sold;\n"
"}\n"
"\n"
"QPushButton:hove{\n"
"border-raidus:0px;\n"
"border-style:solid;\n"
"border-width:0px 0px 2px 0px;\n"
"padding:4px 4px 2px 4px;\n"
"border-color:#00BB9E;\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #646464,stop:1 #525252);\n"
"}"));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(widget);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        sizePolicy2.setHeightForWidth(btnCancel->sizePolicy().hasHeightForWidth());
        btnCancel->setSizePolicy(sizePolicy2);
        btnCancel->setMinimumSize(QSize(0, 0));
        btnCancel->setFont(font1);
        btnCancel->setStyleSheet(QLatin1String("QPushButton{\n"
"\n"
"border-raidus:0px;\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"background-color: rgb(0, 170, 255);\n"
"border-width:1px;\n"
"border-style:sold;\n"
"}\n"
"\n"
"QPushButton:hove{\n"
"border-raidus:0px;\n"
"border-style:solid;\n"
"border-width:0px 0px 2px 0px;\n"
"padding:4px 4px 2px 4px;\n"
"border-color:#00BB9E;\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #646464,stop:1 #525252);\n"
"}"));

        horizontalLayout->addWidget(btnCancel);


        verticalLayout_2->addWidget(widget);


        retranslateUi(DialogMerge);

        QMetaObject::connectSlotsByName(DialogMerge);
    } // setupUi

    void retranslateUi(QDialog *DialogMerge)
    {
        DialogMerge->setWindowTitle(QApplication::translate("DialogMerge", "Dialog", 0));
        groupBox->setTitle(QString());
        label->setText(QApplication::translate("DialogMerge", "\351\200\232\351\201\223\347\274\226\345\217\267", 0));
        label_2->setText(QApplication::translate("DialogMerge", "\347\224\265\350\212\257\345\220\215\347\247\260", 0));
        label_3->setText(QApplication::translate("DialogMerge", "\346\265\213\350\257\225\346\225\260\346\215\256\346\226\207\344\273\266", 0));
        label_4->setText(QApplication::translate("DialogMerge", "\351\200\211\346\213\251\346\226\207\344\273\266", 0));
        label_5->setText(QApplication::translate("DialogMerge", " \345\216\202\345\225\206", 0));
        btnOk->setText(QApplication::translate("DialogMerge", "\347\241\256\345\256\232", 0));
        btnCancel->setText(QApplication::translate("DialogMerge", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogMerge: public Ui_DialogMerge {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGMERGE_H
