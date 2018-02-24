/********************************************************************************
** Form generated from reading ui file 'config.ui'
**
** Created: Mon Mar 22 15:14:46 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFontComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigDialog
{
public:
    QDialogButtonBox *configButtonBox;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *afterPreDirLineEdit;
    QPushButton *afterPreBrowserBtn;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *afterVtlCodedDirLineEdit;
    QPushButton *afterVtlCodedBrowserBtn;
    QGroupBox *groupBox_3;
    QLabel *label;
    QFontComboBox *fontFamilyComboBox;
    QLabel *label_2;
    QSpinBox *fontSizeSpinBox;
    QGroupBox *groupBox_6;
    QWidget *layoutWidget_5;
    QVBoxLayout *verticalLayout;
    QRadioButton *normalRadioBtn;
    QRadioButton *debugRadioBtn;
    QGroupBox *groupBox_4;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *entryFuncLineEdit;

    void setupUi(QDialog *ConfigDialog)
    {
        if (ConfigDialog->objectName().isEmpty())
            ConfigDialog->setObjectName(QString::fromUtf8("ConfigDialog"));
        ConfigDialog->resize(461, 390);
        configButtonBox = new QDialogButtonBox(ConfigDialog);
        configButtonBox->setObjectName(QString::fromUtf8("configButtonBox"));
        configButtonBox->setGeometry(QRect(110, 350, 331, 32));
        configButtonBox->setOrientation(Qt::Horizontal);
        configButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(ConfigDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 190, 421, 61));
        QFont font;
        font.setFamily(QString::fromUtf8("Cambria"));
        font.setPointSize(12);
        font.setStyleStrategy(QFont::PreferAntialias);
        groupBox->setFont(font);
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 401, 33));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        afterPreDirLineEdit = new QLineEdit(layoutWidget);
        afterPreDirLineEdit->setObjectName(QString::fromUtf8("afterPreDirLineEdit"));

        horizontalLayout->addWidget(afterPreDirLineEdit);

        afterPreBrowserBtn = new QPushButton(layoutWidget);
        afterPreBrowserBtn->setObjectName(QString::fromUtf8("afterPreBrowserBtn"));

        horizontalLayout->addWidget(afterPreBrowserBtn);

        groupBox_2 = new QGroupBox(ConfigDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 270, 421, 61));
        groupBox_2->setFont(font);
        layoutWidget1 = new QWidget(groupBox_2);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 20, 401, 33));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        afterVtlCodedDirLineEdit = new QLineEdit(layoutWidget1);
        afterVtlCodedDirLineEdit->setObjectName(QString::fromUtf8("afterVtlCodedDirLineEdit"));

        horizontalLayout_2->addWidget(afterVtlCodedDirLineEdit);

        afterVtlCodedBrowserBtn = new QPushButton(layoutWidget1);
        afterVtlCodedBrowserBtn->setObjectName(QString::fromUtf8("afterVtlCodedBrowserBtn"));

        horizontalLayout_2->addWidget(afterVtlCodedBrowserBtn);

        groupBox_3 = new QGroupBox(ConfigDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(170, 80, 271, 91));
        groupBox_3->setFont(font);
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 21, 61, 26));
        fontFamilyComboBox = new QFontComboBox(groupBox_3);
        fontFamilyComboBox->setObjectName(QString::fromUtf8("fontFamilyComboBox"));
        fontFamilyComboBox->setGeometry(QRect(80, 21, 182, 27));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 54, 31, 21));
        fontSizeSpinBox = new QSpinBox(groupBox_3);
        fontSizeSpinBox->setObjectName(QString::fromUtf8("fontSizeSpinBox"));
        fontSizeSpinBox->setGeometry(QRect(80, 54, 40, 27));
        fontSizeSpinBox->setMinimum(6);
        fontSizeSpinBox->setMaximum(20);
        fontSizeSpinBox->setValue(12);
        groupBox_6 = new QGroupBox(ConfigDialog);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(20, 80, 121, 91));
        groupBox_6->setFont(font);
        layoutWidget_5 = new QWidget(groupBox_6);
        layoutWidget_5->setObjectName(QString::fromUtf8("layoutWidget_5"));
        layoutWidget_5->setGeometry(QRect(10, 20, 101, 61));
        verticalLayout = new QVBoxLayout(layoutWidget_5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        normalRadioBtn = new QRadioButton(layoutWidget_5);
        normalRadioBtn->setObjectName(QString::fromUtf8("normalRadioBtn"));
        normalRadioBtn->setChecked(true);

        verticalLayout->addWidget(normalRadioBtn);

        debugRadioBtn = new QRadioButton(layoutWidget_5);
        debugRadioBtn->setObjectName(QString::fromUtf8("debugRadioBtn"));

        verticalLayout->addWidget(debugRadioBtn);

        groupBox_4 = new QGroupBox(ConfigDialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(20, 10, 421, 61));
        groupBox_4->setFont(font);
        layoutWidget_2 = new QWidget(groupBox_4);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 20, 401, 29));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        entryFuncLineEdit = new QLineEdit(layoutWidget_2);
        entryFuncLineEdit->setObjectName(QString::fromUtf8("entryFuncLineEdit"));

        horizontalLayout_4->addWidget(entryFuncLineEdit);


        retranslateUi(ConfigDialog);
        QObject::connect(configButtonBox, SIGNAL(accepted()), ConfigDialog, SLOT(accept()));
        QObject::connect(configButtonBox, SIGNAL(rejected()), ConfigDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigDialog)
    {
        ConfigDialog->setWindowTitle(QApplication::translate("ConfigDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ConfigDialog", "Directory for After-precompile Files ", 0, QApplication::UnicodeUTF8));
        afterPreBrowserBtn->setText(QApplication::translate("ConfigDialog", "Browser", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("ConfigDialog", "Directory for After-vitalcoded Files ", 0, QApplication::UnicodeUTF8));
        afterVtlCodedBrowserBtn->setText(QApplication::translate("ConfigDialog", "Browser", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("ConfigDialog", "Font Setting", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ConfigDialog", "Family:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ConfigDialog", "Size:", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("ConfigDialog", "Mode", 0, QApplication::UnicodeUTF8));
        normalRadioBtn->setText(QApplication::translate("ConfigDialog", "Normal", 0, QApplication::UnicodeUTF8));
        debugRadioBtn->setText(QApplication::translate("ConfigDialog", "Debug", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("ConfigDialog", "Entry-point Function", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ConfigDialog", "Function Name:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ConfigDialog);
    } // retranslateUi

};

namespace Ui {
    class ConfigDialog: public Ui_ConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_H
