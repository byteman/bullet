/********************************************************************************
** Form generated from reading UI file 'chanselect.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANSELECT_H
#define UI_CHANSELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChanSelect
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *lblChan;
    QLineEdit *edtCode;
    QLineEdit *edtFile;
    QPushButton *btnSelFile;
    QComboBox *comboBox;

    void setupUi(QWidget *ChanSelect)
    {
        if (ChanSelect->objectName().isEmpty())
            ChanSelect->setObjectName(QStringLiteral("ChanSelect"));
        ChanSelect->resize(696, 91);
        horizontalLayout = new QHBoxLayout(ChanSelect);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lblChan = new QLabel(ChanSelect);
        lblChan->setObjectName(QStringLiteral("lblChan"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(12);
        lblChan->setFont(font);

        horizontalLayout->addWidget(lblChan);

        edtCode = new QLineEdit(ChanSelect);
        edtCode->setObjectName(QStringLiteral("edtCode"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(edtCode->sizePolicy().hasHeightForWidth());
        edtCode->setSizePolicy(sizePolicy);
        edtCode->setMinimumSize(QSize(100, 0));
        edtCode->setMaximumSize(QSize(120, 16777215));
        edtCode->setFont(font);

        horizontalLayout->addWidget(edtCode);

        edtFile = new QLineEdit(ChanSelect);
        edtFile->setObjectName(QStringLiteral("edtFile"));
        edtFile->setFont(font);

        horizontalLayout->addWidget(edtFile);

        btnSelFile = new QPushButton(ChanSelect);
        btnSelFile->setObjectName(QStringLiteral("btnSelFile"));
        btnSelFile->setFont(font);

        horizontalLayout->addWidget(btnSelFile);

        comboBox = new QComboBox(ChanSelect);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setFont(font);

        horizontalLayout->addWidget(comboBox);


        retranslateUi(ChanSelect);

        QMetaObject::connectSlotsByName(ChanSelect);
    } // setupUi

    void retranslateUi(QWidget *ChanSelect)
    {
        ChanSelect->setWindowTitle(QApplication::translate("ChanSelect", "Form", 0));
        lblChan->setText(QApplication::translate("ChanSelect", "\351\200\232\351\201\223", 0));
        btnSelFile->setText(QApplication::translate("ChanSelect", "\351\200\211\346\213\251\346\226\207\344\273\266..", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("ChanSelect", "\347\221\236\350\203\275", 0)
         << QApplication::translate("ChanSelect", "\346\226\260\345\250\201", 0)
         << QApplication::translate("ChanSelect", "\346\211\277\345\276\267", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class ChanSelect: public Ui_ChanSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANSELECT_H
