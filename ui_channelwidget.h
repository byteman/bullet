/********************************************************************************
** Form generated from reading UI file 'channelwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANNELWIDGET_H
#define UI_CHANNELWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChannelWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lblChan;
    QWidget *widgetContext;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblMax;
    QLabel *lbl_weight;

    void setupUi(QWidget *ChannelWidget)
    {
        if (ChannelWidget->objectName().isEmpty())
            ChannelWidget->setObjectName(QStringLiteral("ChannelWidget"));
        ChannelWidget->resize(363, 271);
        ChannelWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(163, 163, 163);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(255, 255, 255);\n"
"border:1px solid #246424;\n"
"border-radius:3px;\n"
"\n"
""));
        verticalLayout = new QVBoxLayout(ChannelWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(ChannelWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMaximumSize(QSize(16777215, 52));
        widget->setStyleSheet(QStringLiteral(""));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setSpacing(3);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(1, 2, 3, 2);
        lblChan = new QLabel(widget);
        lblChan->setObjectName(QStringLiteral("lblChan"));
        lblChan->setStyleSheet(QLatin1String("border:0px solid #242424;\n"
"border-radius:1px;\n"
"background-color: rgb(255, 85, 127);"));

        horizontalLayout_3->addWidget(lblChan);


        verticalLayout->addWidget(widget);

        widgetContext = new QWidget(ChannelWidget);
        widgetContext->setObjectName(QStringLiteral("widgetContext"));
        widgetContext->setStyleSheet(QStringLiteral(""));
        verticalLayout_2 = new QVBoxLayout(widgetContext);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(2, 2, 2, 2);
        lblMax = new QLabel(widgetContext);
        lblMax->setObjectName(QStringLiteral("lblMax"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblMax->sizePolicy().hasHeightForWidth());
        lblMax->setSizePolicy(sizePolicy);
        lblMax->setMinimumSize(QSize(0, 30));
        lblMax->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_2->addWidget(lblMax);

        lbl_weight = new QLabel(widgetContext);
        lbl_weight->setObjectName(QStringLiteral("lbl_weight"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lbl_weight->sizePolicy().hasHeightForWidth());
        lbl_weight->setSizePolicy(sizePolicy1);
        lbl_weight->setMinimumSize(QSize(0, 60));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(24);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        lbl_weight->setFont(font);
        lbl_weight->setStyleSheet(QString::fromUtf8("font: 24pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lbl_weight->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_2->addWidget(lbl_weight);


        verticalLayout->addWidget(widgetContext);


        retranslateUi(ChannelWidget);

        QMetaObject::connectSlotsByName(ChannelWidget);
    } // setupUi

    void retranslateUi(QWidget *ChannelWidget)
    {
        ChannelWidget->setWindowTitle(QApplication::translate("ChannelWidget", "Form", 0));
        lblChan->setText(QString());
        lblMax->setText(QApplication::translate("ChannelWidget", "\345\263\260\345\200\274:123", 0));
        lbl_weight->setText(QApplication::translate("ChannelWidget", "000", 0));
    } // retranslateUi

};

namespace Ui {
    class ChannelWidget: public Ui_ChannelWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANNELWIDGET_H
