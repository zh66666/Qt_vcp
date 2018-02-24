/**
 ** -------------------------------
 ** Create By:   Ryan LEUNG
 ** Date:        2009-09-23
 ** -------------------------------
 ** -------------------------------
 ** Modified by: Ryan LEUNG
 ** Date:        2010-01-11
 ** Content:     Add comments
 ** -------------------------------
 ** -------------------------------
 ** Modified by: Ryan LEUNG
 ** Date:        2010-01-13
 ** Content:     Add line number and syntax-highlighting
 ** -------------------------------
 ** -------------------------------
 ** Modified by: Ryan LEUNG
 ** Date:        2010-01-21
 ** Content:     Add config dialog
 ** -------------------------------
 **/

#include <QtGui>
#include "configdialog.h"

// 功能：构造函数
// 参数：parent - 父框架
// 返回：无
ConfigDialog::ConfigDialog(QDialog* parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    readSettings();
}

// 功能：从配置文件中读取配置信息
// 参数：无
// 返回：无
void ConfigDialog::readSettings()
{
    // 设置配置文件的信息，文件名等
    QDir appDir("./");
    QSettings settings("vcp_config.ini", QSettings::IniFormat);
    settings.setPath(QSettings::IniFormat, QSettings::UserScope,
                     appDir.absolutePath());

    // 读取运行模式
    settings.beginGroup("Mode");
    settings.value("runmode") == "Debug" ?
            ui.debugRadioBtn->setChecked(true) :
            ui.normalRadioBtn->setChecked(true);
    settings.endGroup();

    // 读取预处理后文件列表存放目录
    settings.beginGroup("FilesAfterPrecompile");
    ui.afterPreDirLineEdit->setText(
            settings.value("directory",
                           appDir.absolutePath() + "/pre_compile").toString());
    settings.endGroup();

    // 读取安全编码后文件列表存放目录
    settings.beginGroup("FilesAfterVitalCoded");
    ui.afterVtlCodedDirLineEdit->setText(
            settings.value("directory",
                           appDir.absolutePath() + "/redcode").toString());
    settings.endGroup();

    // 读取入口函数名
    settings.beginGroup("Entry-pointFunction");
    ui.entryFuncLineEdit->setText(
            settings.value("name", "main").toString());
    settings.endGroup();

    // 读取编辑器字体
    settings.beginGroup("FontView");
    ui.fontFamilyComboBox->setCurrentFont(
            settings.value("family", QFont("Cambria")).value<QFont>());
    settings.endGroup();
    settings.beginGroup("FontView");
    ui.fontSizeSpinBox->setValue(
            settings.value("size", 12).toInt());
    settings.endGroup();
}

// 功能：将配置信息写入配置文件中
// 参数：无
// 返回：无
void ConfigDialog::writeSettings()
{
    QDir appDir("./");
    QSettings settings("vcp_config.ini", QSettings::IniFormat);
    settings.setPath(QSettings::IniFormat, QSettings::UserScope,
                     appDir.absolutePath());

    // 写入运行模式
    settings.beginGroup("Mode");
    settings.setValue("runmode",
                      ui.normalRadioBtn->isChecked() ? "Normal" : "Debug");
    settings.endGroup();

    // 写入入口函数名
    settings.beginGroup("Entry-pointFunction");
    settings.setValue("name", ui.entryFuncLineEdit->text());
    settings.endGroup();

    // 写入预处理后文件列表存放目录
    settings.beginGroup("FilesAfterPrecompile");
    settings.setValue("directory", ui.afterPreDirLineEdit->text());
    settings.endGroup();

    // 写入安全编码后文件列表存放目录
    settings.beginGroup("FilesAfterVitalCoded");
    settings.setValue("directory", ui.afterVtlCodedDirLineEdit->text());
    settings.endGroup();

    // 写入编辑器字体
    settings.beginGroup("FontView");
    settings.setValue("family", ui.fontFamilyComboBox->currentFont());
    settings.endGroup();
    settings.beginGroup("FontView");
    settings.setValue("size", ui.fontSizeSpinBox->text());
    settings.endGroup();
}

// 功能：当点击配置对话框的OK按钮时
// 参数：无
// 返回：无
void ConfigDialog::on_configButtonBox_accepted()
{
    writeSettings();
    QFont font(ui.fontFamilyComboBox->currentFont());
    font.setPointSize(ui.fontSizeSpinBox->value());
    emit changeSettings(ui.normalRadioBtn->isChecked() ? "Normal" : "Debug",
                        ui.afterPreDirLineEdit->text(),
                        ui.afterVtlCodedDirLineEdit->text(),
                        ui.entryFuncLineEdit->text(),
                        font);
}

// 功能：当点击配置对话框的Cancel按钮时
// 参数：无
// 返回：无
void ConfigDialog::on_configButtonBox_rejected()
{
}

// 功能：当点击配置对话框的预处理后文件目录的browser按钮时
// 参数：无
// 返回：无
void ConfigDialog::on_afterPreBrowserBtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
            this, "Choose Directory for Files after Precompiling");
    if (!dir.isEmpty())
    {
        ui.afterPreDirLineEdit->setText(dir);
    }
}

// 功能：当点击配置对话框的安全编码后文件目录的browser按钮时
// 参数：无
// 返回：无
void ConfigDialog::on_afterVtlCodedBrowserBtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
            this, "Choose Directory for Files after Vital Coding");
    if (!dir.isEmpty())
    {
        ui.afterVtlCodedDirLineEdit->setText(dir);
    }
}
