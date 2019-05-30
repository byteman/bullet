/********************************************************************************
** Form generated from reading UI file 'dialogdevice.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGDEVICE_H
#define UI_DIALOGDEVICE_H

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

class Ui_DialogDevice
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *edtSerialNo;
    QLabel *label_2;
    QLineEdit *edtDevName;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOK;
    QPushButton *pushButton_2;

    void setupUi(QDialog *DialogDevice)
    {
        if (DialogDevice->objectName().isEmpty())
            DialogDevice->setObjectName(QStringLiteral("DialogDevice"));
        DialogDevice->resize(423, 303);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(14);
        DialogDevice->setFont(font);
        verticalLayout = new QVBoxLayout(DialogDevice);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        groupBox = new QGroupBox(DialogDevice);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(300, 200));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(12);
        label->setFont(font1);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        edtSerialNo = new QLineEdit(groupBox);
        edtSerialNo->setObjectName(QStringLiteral("edtSerialNo"));
        edtSerialNo->setMinimumSize(QSize(0, 30));
        edtSerialNo->setFont(font1);

        gridLayout->addWidget(edtSerialNo, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        edtDevName = new QLineEdit(groupBox);
        edtDevName->setObjectName(QStringLiteral("edtDevName"));
        edtDevName->setMinimumSize(QSize(0, 30));
        edtDevName->setFont(font1);

        gridLayout->addWidget(edtDevName, 1, 1, 1, 1);


        horizontalLayout_2->addWidget(groupBox);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOK = new QPushButton(DialogDevice);
        btnOK->setObjectName(QStringLiteral("btnOK"));
        btnOK->setMinimumSize(QSize(0, 30));
        QFont font2;
        font2.setPointSize(12);
        btnOK->setFont(font2);

        horizontalLayout->addWidget(btnOK);

        pushButton_2 = new QPushButton(DialogDevice);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(0, 30));
        pushButton_2->setFont(font2);

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(DialogDevice);

        QMetaObject::connectSlotsByName(DialogDevice);
    } // setupUi

    void retranslateUi(QDialog *DialogDevice)
    {
        DialogDevice->setWindowTitle(QApplication::translate("DialogDevice", "\350\256\276\345\244\207\351\205\215\347\275\256", 0));
        groupBox->setTitle(QApplication::translate("DialogDevice", "\350\256\276\345\244\207\344\277\241\346\201\257", 0));
        label->setText(QApplication::translate("DialogDevice", "\350\256\276\345\244\207\345\272\217\345\210\227\345\217\267", 0));
        edtSerialNo->setText(QString());
        label_2->setText(QApplication::translate("DialogDevice", "\350\256\276\345\244\207\345\220\215\347\247\260", 0));
        edtDevName->setText(QString());
        btnOK->setText(QApplication::translate("DialogDevice", "\344\277\235\345\255\230", 0));
        pushButton_2->setText(QApplication::translate("DialogDevice", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogDevice: public Ui_DialogDevice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGDEVICE_H
