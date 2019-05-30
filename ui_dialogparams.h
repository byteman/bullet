/********************************************************************************
** Form generated from reading UI file 'dialogparams.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPARAMS_H
#define UI_DIALOGPARAMS_H

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

class Ui_DialogParams
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout;
    QLineEdit *edtServerPort;
    QLineEdit *edtTime;
    QLineEdit *edtServerIp;
    QLineEdit *edtDelay;
    QLineEdit *edtGateway;
    QLabel *label_20;
    QLabel *label_7;
    QLabel *label_14;
    QLabel *label;
    QLabel *label_8;
    QLabel *label_18;
    QLineEdit *edtDevIp;
    QLabel *label_2;
    QLabel *label_17;
    QLineEdit *edtTimeOut;
    QLineEdit *edtNetmask;
    QLabel *label_13;
    QLineEdit *edtDevId;
    QLabel *label_3;
    QLineEdit *edtVer;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnRead;
    QPushButton *btnSave;
    QPushButton *btnExit;

    void setupUi(QDialog *DialogParams)
    {
        if (DialogParams->objectName().isEmpty())
            DialogParams->setObjectName(QStringLiteral("DialogParams"));
        DialogParams->resize(586, 590);
        verticalLayout = new QVBoxLayout(DialogParams);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_6);

        groupBox_6 = new QGroupBox(DialogParams);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_6->sizePolicy().hasHeightForWidth());
        groupBox_6->setSizePolicy(sizePolicy);
        groupBox_6->setMinimumSize(QSize(500, 500));
        groupBox_6->setMaximumSize(QSize(100000, 100000));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(14);
        groupBox_6->setFont(font);
        gridLayout = new QGridLayout(groupBox_6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(-1, 20, -1, -1);
        edtServerPort = new QLineEdit(groupBox_6);
        edtServerPort->setObjectName(QStringLiteral("edtServerPort"));
        edtServerPort->setMinimumSize(QSize(0, 32));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(12);
        edtServerPort->setFont(font1);

        gridLayout->addWidget(edtServerPort, 8, 1, 1, 1);

        edtTime = new QLineEdit(groupBox_6);
        edtTime->setObjectName(QStringLiteral("edtTime"));
        edtTime->setMinimumSize(QSize(0, 32));
        edtTime->setFont(font1);

        gridLayout->addWidget(edtTime, 3, 1, 1, 1);

        edtServerIp = new QLineEdit(groupBox_6);
        edtServerIp->setObjectName(QStringLiteral("edtServerIp"));
        edtServerIp->setMinimumSize(QSize(0, 32));
        edtServerIp->setFont(font1);

        gridLayout->addWidget(edtServerIp, 7, 1, 1, 1);

        edtDelay = new QLineEdit(groupBox_6);
        edtDelay->setObjectName(QStringLiteral("edtDelay"));
        edtDelay->setFont(font1);

        gridLayout->addWidget(edtDelay, 9, 1, 1, 1);

        edtGateway = new QLineEdit(groupBox_6);
        edtGateway->setObjectName(QStringLiteral("edtGateway"));
        edtGateway->setMinimumSize(QSize(0, 32));
        edtGateway->setFont(font1);

        gridLayout->addWidget(edtGateway, 6, 1, 1, 1);

        label_20 = new QLabel(groupBox_6);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setFont(font1);

        gridLayout->addWidget(label_20, 8, 0, 1, 1);

        label_7 = new QLabel(groupBox_6);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font1);

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        label_14 = new QLabel(groupBox_6);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setFont(font1);

        gridLayout->addWidget(label_14, 7, 0, 1, 1);

        label = new QLabel(groupBox_6);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font1);

        gridLayout->addWidget(label, 9, 0, 1, 1);

        label_8 = new QLabel(groupBox_6);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font1);

        gridLayout->addWidget(label_8, 1, 0, 1, 1);

        label_18 = new QLabel(groupBox_6);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setFont(font1);

        gridLayout->addWidget(label_18, 6, 0, 1, 1);

        edtDevIp = new QLineEdit(groupBox_6);
        edtDevIp->setObjectName(QStringLiteral("edtDevIp"));
        edtDevIp->setMinimumSize(QSize(0, 32));
        edtDevIp->setFont(font1);

        gridLayout->addWidget(edtDevIp, 4, 1, 1, 1);

        label_2 = new QLabel(groupBox_6);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 10, 0, 1, 1);

        label_17 = new QLabel(groupBox_6);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setFont(font1);

        gridLayout->addWidget(label_17, 5, 0, 1, 1);

        edtTimeOut = new QLineEdit(groupBox_6);
        edtTimeOut->setObjectName(QStringLiteral("edtTimeOut"));
        edtTimeOut->setFont(font1);

        gridLayout->addWidget(edtTimeOut, 10, 1, 1, 1);

        edtNetmask = new QLineEdit(groupBox_6);
        edtNetmask->setObjectName(QStringLiteral("edtNetmask"));
        edtNetmask->setMinimumSize(QSize(0, 32));
        edtNetmask->setFont(font1);

        gridLayout->addWidget(edtNetmask, 5, 1, 1, 1);

        label_13 = new QLabel(groupBox_6);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setFont(font1);

        gridLayout->addWidget(label_13, 4, 0, 1, 1);

        edtDevId = new QLineEdit(groupBox_6);
        edtDevId->setObjectName(QStringLiteral("edtDevId"));
        edtDevId->setMinimumSize(QSize(0, 32));
        edtDevId->setFont(font1);

        gridLayout->addWidget(edtDevId, 1, 1, 1, 1);

        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font1);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        edtVer = new QLineEdit(groupBox_6);
        edtVer->setObjectName(QStringLiteral("edtVer"));
        edtVer->setMinimumSize(QSize(0, 32));
        edtVer->setFont(font1);

        gridLayout->addWidget(edtVer, 2, 1, 1, 1);


        horizontalLayout_8->addWidget(groupBox_6);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnRead = new QPushButton(DialogParams);
        btnRead->setObjectName(QStringLiteral("btnRead"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(12);
        btnRead->setFont(font2);

        horizontalLayout->addWidget(btnRead);

        btnSave = new QPushButton(DialogParams);
        btnSave->setObjectName(QStringLiteral("btnSave"));
        btnSave->setFont(font2);

        horizontalLayout->addWidget(btnSave);

        btnExit = new QPushButton(DialogParams);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        btnExit->setFont(font2);

        horizontalLayout->addWidget(btnExit);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(DialogParams);

        QMetaObject::connectSlotsByName(DialogParams);
    } // setupUi

    void retranslateUi(QDialog *DialogParams)
    {
        DialogParams->setWindowTitle(QApplication::translate("DialogParams", "\350\256\276\345\244\207\345\217\202\346\225\260\351\205\215\347\275\256", 0));
        groupBox_6->setTitle(QApplication::translate("DialogParams", "\351\200\232\344\277\241\345\217\202\346\225\260", 0));
        edtServerPort->setText(QString());
        edtTime->setText(QString());
        edtServerIp->setText(QString());
        edtGateway->setText(QString());
        label_20->setText(QApplication::translate("DialogParams", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243", 0));
        label_7->setText(QApplication::translate("DialogParams", "\350\256\276\345\244\207\346\227\266\351\227\264", 0));
        label_14->setText(QApplication::translate("DialogParams", "\346\234\215\345\212\241\345\231\250IP", 0));
        label->setText(QApplication::translate("DialogParams", "\351\207\207\351\233\206\351\227\264\351\232\224", 0));
        label_8->setText(QApplication::translate("DialogParams", "\350\256\276\345\244\207\345\224\257\344\270\200\347\274\226\345\217\267", 0));
        label_18->setText(QApplication::translate("DialogParams", "\347\275\221\345\205\263", 0));
        edtDevIp->setText(QString());
        label_2->setText(QApplication::translate("DialogParams", "\350\257\273\345\217\226\345\273\266\350\277\237", 0));
        label_17->setText(QApplication::translate("DialogParams", "\345\255\220\347\275\221\346\216\251\347\240\201", 0));
        edtNetmask->setText(QString());
        label_13->setText(QApplication::translate("DialogParams", "\350\256\276\345\244\207IP\357\274\232", 0));
        label_3->setText(QApplication::translate("DialogParams", "\350\275\257\344\273\266\347\211\210\346\234\254", 0));
        btnRead->setText(QApplication::translate("DialogParams", "\345\210\267\346\226\260", 0));
        btnSave->setText(QApplication::translate("DialogParams", "\344\277\235\345\255\230", 0));
        btnExit->setText(QApplication::translate("DialogParams", "\350\277\224\345\233\236", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogParams: public Ui_DialogParams {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPARAMS_H
