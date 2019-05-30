/********************************************************************************
** Form generated from reading UI file 'dialogcalib.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCALIB_H
#define UI_DIALOGCALIB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
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

class Ui_DialogCalib
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_5;
    QLabel *label_3;
    QComboBox *cbxChan;
    QLabel *label_5;
    QLineEdit *edtRtAd;
    QLabel *label_6;
    QLineEdit *edtrtWgt;
    QLabel *label_4;
    QLineEdit *edtCalibWet;
    QPushButton *btnCalibZero;
    QPushButton *btnCalibWet;
    QPushButton *btnResetZero;
    QSpacerItem *horizontalSpacer_5;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogCalib)
    {
        if (DialogCalib->objectName().isEmpty())
            DialogCalib->setObjectName(QStringLiteral("DialogCalib"));
        DialogCalib->resize(617, 404);
        verticalLayout = new QVBoxLayout(DialogCalib);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        groupBox_9 = new QGroupBox(DialogCalib);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setMinimumSize(QSize(500, 0));
        gridLayout_5 = new QGridLayout(groupBox_9);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_3 = new QLabel(groupBox_9);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_5->addWidget(label_3, 0, 0, 1, 1);

        cbxChan = new QComboBox(groupBox_9);
        cbxChan->setObjectName(QStringLiteral("cbxChan"));
        cbxChan->setMinimumSize(QSize(0, 30));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(12);
        cbxChan->setFont(font);

        gridLayout_5->addWidget(cbxChan, 0, 1, 1, 2);

        label_5 = new QLabel(groupBox_9);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_5->addWidget(label_5, 1, 0, 1, 1);

        edtRtAd = new QLineEdit(groupBox_9);
        edtRtAd->setObjectName(QStringLiteral("edtRtAd"));
        edtRtAd->setMinimumSize(QSize(0, 30));
        edtRtAd->setFont(font);

        gridLayout_5->addWidget(edtRtAd, 1, 1, 1, 2);

        label_6 = new QLabel(groupBox_9);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_5->addWidget(label_6, 2, 0, 1, 1);

        edtrtWgt = new QLineEdit(groupBox_9);
        edtrtWgt->setObjectName(QStringLiteral("edtrtWgt"));
        edtrtWgt->setMinimumSize(QSize(0, 30));
        edtrtWgt->setFont(font);

        gridLayout_5->addWidget(edtrtWgt, 2, 1, 1, 2);

        label_4 = new QLabel(groupBox_9);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_5->addWidget(label_4, 3, 0, 1, 1);

        edtCalibWet = new QLineEdit(groupBox_9);
        edtCalibWet->setObjectName(QStringLiteral("edtCalibWet"));
        edtCalibWet->setMinimumSize(QSize(0, 30));
        edtCalibWet->setFont(font);

        gridLayout_5->addWidget(edtCalibWet, 3, 1, 1, 2);

        btnCalibZero = new QPushButton(groupBox_9);
        btnCalibZero->setObjectName(QStringLiteral("btnCalibZero"));
        btnCalibZero->setMinimumSize(QSize(0, 35));

        gridLayout_5->addWidget(btnCalibZero, 4, 1, 1, 1);

        btnCalibWet = new QPushButton(groupBox_9);
        btnCalibWet->setObjectName(QStringLiteral("btnCalibWet"));
        btnCalibWet->setMinimumSize(QSize(0, 35));

        gridLayout_5->addWidget(btnCalibWet, 4, 2, 1, 1);

        btnResetZero = new QPushButton(groupBox_9);
        btnResetZero->setObjectName(QStringLiteral("btnResetZero"));
        btnResetZero->setMinimumSize(QSize(0, 35));

        gridLayout_5->addWidget(btnResetZero, 4, 0, 1, 1);


        horizontalLayout_4->addWidget(groupBox_9);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_4);

        buttonBox = new QDialogButtonBox(DialogCalib);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogCalib);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogCalib, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogCalib, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogCalib);
    } // setupUi

    void retranslateUi(QDialog *DialogCalib)
    {
        DialogCalib->setWindowTitle(QApplication::translate("DialogCalib", "Dialog", 0));
        groupBox_9->setTitle(QString());
        label_3->setText(QApplication::translate("DialogCalib", "\351\200\232\351\201\223", 0));
        cbxChan->clear();
        cbxChan->insertItems(0, QStringList()
         << QApplication::translate("DialogCalib", "1", 0)
         << QApplication::translate("DialogCalib", "2", 0)
         << QApplication::translate("DialogCalib", "3", 0)
         << QApplication::translate("DialogCalib", "4", 0)
         << QApplication::translate("DialogCalib", "5", 0)
         << QApplication::translate("DialogCalib", "6", 0)
         << QApplication::translate("DialogCalib", "7", 0)
         << QApplication::translate("DialogCalib", "8", 0)
        );
        label_5->setText(QApplication::translate("DialogCalib", "\345\256\236\346\227\266AD", 0));
        label_6->setText(QApplication::translate("DialogCalib", "\345\256\236\346\227\266\351\207\215\351\207\217", 0));
        label_4->setText(QApplication::translate("DialogCalib", "\346\240\207\345\256\232\351\207\215\351\207\217", 0));
        btnCalibZero->setText(QApplication::translate("DialogCalib", "\346\240\207\345\256\232\351\233\266\347\202\271", 0));
        btnCalibWet->setText(QApplication::translate("DialogCalib", "\346\240\207\345\256\232\351\207\215\351\207\217", 0));
        btnResetZero->setText(QApplication::translate("DialogCalib", "\346\270\205\351\233\266", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogCalib: public Ui_DialogCalib {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCALIB_H
