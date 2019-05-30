/********************************************************************************
** Form generated from reading UI file 'dialogchanconfig.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCHANCONFIG_H
#define UI_DIALOGCHANCONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogChanConfig
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLineEdit *edtMaxValue;
    QLineEdit *edtDevName;
    QLineEdit *edtMinValue;
    QLabel *label_3;
    QLineEdit *edtChan;
    QLabel *label_4;
    QLabel *label;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOK;
    QPushButton *pushButton_2;

    void setupUi(QDialog *DialogChanConfig)
    {
        if (DialogChanConfig->objectName().isEmpty())
            DialogChanConfig->setObjectName(QStringLiteral("DialogChanConfig"));
        DialogChanConfig->resize(526, 421);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(14);
        DialogChanConfig->setFont(font);
        verticalLayout = new QVBoxLayout(DialogChanConfig);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        groupBox = new QGroupBox(DialogChanConfig);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(300, 150));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        edtMaxValue = new QLineEdit(groupBox);
        edtMaxValue->setObjectName(QStringLiteral("edtMaxValue"));
        edtMaxValue->setMinimumSize(QSize(0, 30));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(12);
        edtMaxValue->setFont(font1);

        gridLayout->addWidget(edtMaxValue, 6, 1, 1, 1);

        edtDevName = new QLineEdit(groupBox);
        edtDevName->setObjectName(QStringLiteral("edtDevName"));
        edtDevName->setMinimumSize(QSize(0, 30));
        edtDevName->setFont(font1);

        gridLayout->addWidget(edtDevName, 1, 1, 1, 1);

        edtMinValue = new QLineEdit(groupBox);
        edtMinValue->setObjectName(QStringLiteral("edtMinValue"));
        edtMinValue->setMinimumSize(QSize(0, 30));
        edtMinValue->setFont(font1);

        gridLayout->addWidget(edtMinValue, 3, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font1);

        gridLayout->addWidget(label_3, 6, 0, 1, 1);

        edtChan = new QLineEdit(groupBox);
        edtChan->setObjectName(QStringLiteral("edtChan"));
        edtChan->setMinimumSize(QSize(0, 30));
        edtChan->setFont(font);

        gridLayout->addWidget(edtChan, 2, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font1);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font1);

        gridLayout->addWidget(label, 2, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);


        horizontalLayout_2->addWidget(groupBox);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOK = new QPushButton(DialogChanConfig);
        btnOK->setObjectName(QStringLiteral("btnOK"));
        btnOK->setMinimumSize(QSize(0, 30));
        QFont font2;
        font2.setPointSize(12);
        btnOK->setFont(font2);

        horizontalLayout->addWidget(btnOK);

        pushButton_2 = new QPushButton(DialogChanConfig);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(0, 30));
        pushButton_2->setFont(font2);

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(DialogChanConfig);

        QMetaObject::connectSlotsByName(DialogChanConfig);
    } // setupUi

    void retranslateUi(QDialog *DialogChanConfig)
    {
        DialogChanConfig->setWindowTitle(QApplication::translate("DialogChanConfig", "\351\200\232\351\201\223\351\205\215\347\275\256", 0));
        groupBox->setTitle(QApplication::translate("DialogChanConfig", "\350\256\276\345\244\207\344\277\241\346\201\257", 0));
        edtMaxValue->setText(QString());
        edtDevName->setText(QString());
        edtMinValue->setText(QString());
        label_3->setText(QApplication::translate("DialogChanConfig", "\346\234\200\345\244\247\351\230\200\345\200\274", 0));
        edtChan->setText(QString());
        label_4->setText(QApplication::translate("DialogChanConfig", "\346\234\200\345\260\217\351\230\200\345\200\274", 0));
        label->setText(QApplication::translate("DialogChanConfig", "\351\200\232\351\201\223\345\217\267", 0));
        label_2->setText(QApplication::translate("DialogChanConfig", "\350\256\276\345\244\207\345\220\215\347\247\260", 0));
        btnOK->setText(QApplication::translate("DialogChanConfig", "\344\277\235\345\255\230", 0));
        pushButton_2->setText(QApplication::translate("DialogChanConfig", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogChanConfig: public Ui_DialogChanConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCHANCONFIG_H
