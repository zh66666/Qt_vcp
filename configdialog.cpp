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

// ���ܣ����캯��
// ������parent - �����
// ���أ���
ConfigDialog::ConfigDialog(QDialog* parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    readSettings();
}

// ���ܣ��������ļ��ж�ȡ������Ϣ
// ��������
// ���أ���
void ConfigDialog::readSettings()
{
    // ���������ļ�����Ϣ���ļ�����
    QDir appDir("./");
    QSettings settings("vcp_config.ini", QSettings::IniFormat);
    settings.setPath(QSettings::IniFormat, QSettings::UserScope,
                     appDir.absolutePath());

    // ��ȡ����ģʽ
    settings.beginGroup("Mode");
    settings.value("runmode") == "Debug" ?
            ui.debugRadioBtn->setChecked(true) :
            ui.normalRadioBtn->setChecked(true);
    settings.endGroup();

    // ��ȡԤ������ļ��б���Ŀ¼
    settings.beginGroup("FilesAfterPrecompile");
    ui.afterPreDirLineEdit->setText(
            settings.value("directory",
                           appDir.absolutePath() + "/pre_compile").toString());
    settings.endGroup();

    // ��ȡ��ȫ������ļ��б���Ŀ¼
    settings.beginGroup("FilesAfterVitalCoded");
    ui.afterVtlCodedDirLineEdit->setText(
            settings.value("directory",
                           appDir.absolutePath() + "/redcode").toString());
    settings.endGroup();

    // ��ȡ��ں�����
    settings.beginGroup("Entry-pointFunction");
    ui.entryFuncLineEdit->setText(
            settings.value("name", "main").toString());
    settings.endGroup();

    // ��ȡ�༭������
    settings.beginGroup("FontView");
    ui.fontFamilyComboBox->setCurrentFont(
            settings.value("family", QFont("Cambria")).value<QFont>());
    settings.endGroup();
    settings.beginGroup("FontView");
    ui.fontSizeSpinBox->setValue(
            settings.value("size", 12).toInt());
    settings.endGroup();
}

// ���ܣ���������Ϣд�������ļ���
// ��������
// ���أ���
void ConfigDialog::writeSettings()
{
    QDir appDir("./");
    QSettings settings("vcp_config.ini", QSettings::IniFormat);
    settings.setPath(QSettings::IniFormat, QSettings::UserScope,
                     appDir.absolutePath());

    // д������ģʽ
    settings.beginGroup("Mode");
    settings.setValue("runmode",
                      ui.normalRadioBtn->isChecked() ? "Normal" : "Debug");
    settings.endGroup();

    // д����ں�����
    settings.beginGroup("Entry-pointFunction");
    settings.setValue("name", ui.entryFuncLineEdit->text());
    settings.endGroup();

    // д��Ԥ������ļ��б���Ŀ¼
    settings.beginGroup("FilesAfterPrecompile");
    settings.setValue("directory", ui.afterPreDirLineEdit->text());
    settings.endGroup();

    // д�밲ȫ������ļ��б���Ŀ¼
    settings.beginGroup("FilesAfterVitalCoded");
    settings.setValue("directory", ui.afterVtlCodedDirLineEdit->text());
    settings.endGroup();

    // д��༭������
    settings.beginGroup("FontView");
    settings.setValue("family", ui.fontFamilyComboBox->currentFont());
    settings.endGroup();
    settings.beginGroup("FontView");
    settings.setValue("size", ui.fontSizeSpinBox->text());
    settings.endGroup();
}

// ���ܣ���������öԻ����OK��ťʱ
// ��������
// ���أ���
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

// ���ܣ���������öԻ����Cancel��ťʱ
// ��������
// ���أ���
void ConfigDialog::on_configButtonBox_rejected()
{
}

// ���ܣ���������öԻ����Ԥ������ļ�Ŀ¼��browser��ťʱ
// ��������
// ���أ���
void ConfigDialog::on_afterPreBrowserBtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
            this, "Choose Directory for Files after Precompiling");
    if (!dir.isEmpty())
    {
        ui.afterPreDirLineEdit->setText(dir);
    }
}

// ���ܣ���������öԻ���İ�ȫ������ļ�Ŀ¼��browser��ťʱ
// ��������
// ���أ���
void ConfigDialog::on_afterVtlCodedBrowserBtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
            this, "Choose Directory for Files after Vital Coding");
    if (!dir.isEmpty())
    {
        ui.afterVtlCodedDirLineEdit->setText(dir);
    }
}
