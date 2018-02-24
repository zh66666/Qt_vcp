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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include "ui_config.h"

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog(QDialog* parent = 0);

private slots:
    void on_configButtonBox_accepted();
    void on_configButtonBox_rejected();
    void on_afterPreBrowserBtn_clicked();
    void on_afterVtlCodedBrowserBtn_clicked();

signals:
    void changeSettings(QString mode, QString afterPreDir,
                        QString afterVitalDir, QString entryFuncName,
                        QFont editorFont);  // 当更改配置信息时发出此信号

private:
    void readSettings();
    void writeSettings();
    Ui::ConfigDialog ui;
};

#endif // CONFIGDIALOG_H
