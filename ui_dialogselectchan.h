/********************************************************************************
** Form generated from reading UI file 'dialogselectchan.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSELECTCHAN_H
#define UI_DIALOGSELECTCHAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogSelectChan
{
public:
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QListWidget *listDevice;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_3;
    QListWidget *listChan;
    QGroupBox *groupBox_4;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QListWidget *listDevChan;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;

    void setupUi(QDialog *DialogSelectChan)
    {
        if (DialogSelectChan->objectName().isEmpty())
            DialogSelectChan->setObjectName(QStringLiteral("DialogSelectChan"));
        DialogSelectChan->resize(1108, 640);
        horizontalLayout_2 = new QHBoxLayout(DialogSelectChan);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        groupBox = new QGroupBox(DialogSelectChan);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(300, 16777215));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listDevice = new QListWidget(groupBox);
        listDevice->setObjectName(QStringLiteral("listDevice"));

        verticalLayout->addWidget(listDevice);


        horizontalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(DialogSelectChan);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMaximumSize(QSize(300, 16777215));
        horizontalLayout_3 = new QHBoxLayout(groupBox_2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        listChan = new QListWidget(groupBox_2);
        listChan->setObjectName(QStringLiteral("listChan"));
        listChan->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_3->addWidget(listChan);


        horizontalLayout_2->addWidget(groupBox_2);

        groupBox_4 = new QGroupBox(DialogSelectChan);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(150, 0));
        groupBox_4->setMaximumSize(QSize(150, 16777215));
        pushButton = new QPushButton(groupBox_4);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 220, 121, 34));
        pushButton_2 = new QPushButton(groupBox_4);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 300, 121, 34));

        horizontalLayout_2->addWidget(groupBox_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox_3 = new QGroupBox(DialogSelectChan);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        listDevChan = new QListWidget(groupBox_3);
        listDevChan->setObjectName(QStringLiteral("listDevChan"));

        verticalLayout_2->addWidget(listDevChan);


        verticalLayout_3->addWidget(groupBox_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_4 = new QPushButton(DialogSelectChan);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout->addWidget(pushButton_4);

        pushButton_3 = new QPushButton(DialogSelectChan);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);


        verticalLayout_3->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_3);


        retranslateUi(DialogSelectChan);

        QMetaObject::connectSlotsByName(DialogSelectChan);
    } // setupUi

    void retranslateUi(QDialog *DialogSelectChan)
    {
        DialogSelectChan->setWindowTitle(QApplication::translate("DialogSelectChan", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("DialogSelectChan", "\350\256\276\345\244\207\351\200\211\346\213\251", 0));
        groupBox_2->setTitle(QApplication::translate("DialogSelectChan", "\351\200\232\351\201\223\351\200\211\346\213\251", 0));
        groupBox_4->setTitle(QApplication::translate("DialogSelectChan", "\346\223\215\344\275\234", 0));
        pushButton->setText(QApplication::translate("DialogSelectChan", "\346\267\273\345\212\240>>>", 0));
        pushButton_2->setText(QApplication::translate("DialogSelectChan", "\345\210\240\351\231\244<<<", 0));
        groupBox_3->setTitle(QApplication::translate("DialogSelectChan", "\345\276\205\345\220\210\345\271\266\351\200\232\351\201\223", 0));
        pushButton_4->setText(QApplication::translate("DialogSelectChan", "\345\205\263\351\227\255", 0));
        pushButton_3->setText(QApplication::translate("DialogSelectChan", "\344\270\213\344\270\200\346\255\245", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogSelectChan: public Ui_DialogSelectChan {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSELECTCHAN_H
