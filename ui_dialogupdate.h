/********************************************************************************
** Form generated from reading UI file 'dialogupdate.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGUPDATE_H
#define UI_DIALOGUPDATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogUpdate
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLineEdit *edtDevIp;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *edtFileName;
    QPushButton *btnSelectFile;
    QProgressBar *pbUpdate;
    QSpacerItem *verticalSpacer;
    QPushButton *btnStartUpdate;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnExit;

    void setupUi(QDialog *DialogUpdate)
    {
        if (DialogUpdate->objectName().isEmpty())
            DialogUpdate->setObjectName(QStringLiteral("DialogUpdate"));
        DialogUpdate->resize(637, 393);
        verticalLayout = new QVBoxLayout(DialogUpdate);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(DialogUpdate);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        edtDevIp = new QLineEdit(groupBox);
        edtDevIp->setObjectName(QStringLiteral("edtDevIp"));

        verticalLayout_2->addWidget(edtDevIp);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        edtFileName = new QLineEdit(groupBox);
        edtFileName->setObjectName(QStringLiteral("edtFileName"));

        horizontalLayout_2->addWidget(edtFileName);

        btnSelectFile = new QPushButton(groupBox);
        btnSelectFile->setObjectName(QStringLiteral("btnSelectFile"));

        horizontalLayout_2->addWidget(btnSelectFile);


        verticalLayout_2->addLayout(horizontalLayout_2);

        pbUpdate = new QProgressBar(groupBox);
        pbUpdate->setObjectName(QStringLiteral("pbUpdate"));
        pbUpdate->setValue(24);

        verticalLayout_2->addWidget(pbUpdate);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        btnStartUpdate = new QPushButton(groupBox);
        btnStartUpdate->setObjectName(QStringLiteral("btnStartUpdate"));
        btnStartUpdate->setMinimumSize(QSize(0, 30));

        verticalLayout_2->addWidget(btnStartUpdate);


        verticalLayout->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnExit = new QPushButton(DialogUpdate);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        btnExit->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(btnExit);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(DialogUpdate);

        QMetaObject::connectSlotsByName(DialogUpdate);
    } // setupUi

    void retranslateUi(QDialog *DialogUpdate)
    {
        DialogUpdate->setWindowTitle(QApplication::translate("DialogUpdate", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("DialogUpdate", "\345\215\207\347\272\247", 0));
        label_2->setText(QApplication::translate("DialogUpdate", "\350\256\276\345\244\207\345\234\260\345\235\200", 0));
        label->setText(QApplication::translate("DialogUpdate", "\351\200\211\346\213\251\345\215\207\347\272\247\346\226\207\344\273\266", 0));
        btnSelectFile->setText(QApplication::translate("DialogUpdate", "\351\200\211\346\213\251\346\226\207\344\273\266", 0));
        btnStartUpdate->setText(QApplication::translate("DialogUpdate", "\345\274\200\345\247\213\345\215\207\347\272\247", 0));
        btnExit->setText(QApplication::translate("DialogUpdate", "\351\200\200\345\207\272", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogUpdate: public Ui_DialogUpdate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGUPDATE_H
