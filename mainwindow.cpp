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
 ** -------------------------------
 ** Modified by: Ryan LEUNG
 ** Date:        2010-03-01
 ** Content:     Modify the preProcess function, using mcpp.exe instead of gcc.
 ** -------------------------------
 **/

#include <QtGui>
#include "mainwindow.h"
#include "configdialog.h"
#include<QProcess>

// ���ܣ�MainWindow���캯�������ڹ��������������
// ������parent - ������ָ��
// ���أ���
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    needPreCmpl = true;  // ���Ƿ���Ҫ����Ԥ�����־λ��Ϊtrue������Ҫ����Ԥ����
    lastOpenDir = "";  // ���ڼ�¼�ϴδ򿪵�·����

    getConfig(); // �������ļ��ж�ȡ������Ϣ

    createMdiArea();  // �������ļ��༭����
    createActions();  // ������ť
    createMenus();  // ����Ŀ¼��
    createToolBars();  // ����������
    createStatusBar();  // ����״̬��
    createDockWindows();  // ����Ԥ����ǰ�ļ��б�ȶ����ͣ������

    configDialog = new ConfigDialog();
    connect(configDialog,
            SIGNAL(changeSettings(QString, QString,
                                  QString, QString, QFont)),
            this,
            SLOT(getChangedConfig(QString, QString,
                                  QString, QString, QFont)));

    windowMapper = new QSignalMapper(this);  // ���ڰ󶨴Ӵ��ڲ˵��������Ķ���ź�
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));  // ������ź���
                                                // setActiveSubWindow��������һ��

    updateFileAndEditMenus();  // ���²˵�����״̬

    setWindowTitle(tr("Vital Coded Processor"));  // ���������ڵ�����
    setWindowIcon(QIcon(":/icons/mainwindow.png"));  // ���������ڵ�ͼ��
    showMaximized();  // �����������
}

// ���ܣ�MainWindow��������
// ��������
// ���أ���
MainWindow::~MainWindow()
{
}

// ���ܣ������ڹر��¼����������������ڽ��յ����ڹر�����ʱִ��
// ������event - ���ڹر������¼�
// ���أ���
void MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();  // �����ļ��༭�����Ӵ���ȫ���ر�

    // �ж϶��ļ��༭�����Ӵ����Ƿ�ȫ���ر�
    getActiveMdiChild() ?
            event->ignore() :  // �����д򿪵��Ӵ��ڣ�����������¼����Ȳ��ر�������
            event->accept();   // ��������¼������ر�������
}

// ���ܣ��������ļ������û�����˵������߹������С��½��ļ�����ťʱ���á�
// ��������
// ���أ���
void MainWindow::newFile()
{
    MdiChild* child = createMdiChild();  // ����һ��MdiChild�������ڴ���һ�����ļ�
    child->newFile(); // �������ļ�
    child->show(); // ��ʾ���ļ��Ӵ���
}

// ���ܣ��򿪴����ļ������û�����˵������߹������С����ļ�����ťʱ���á�
// ��������
// ���أ���
void MainWindow::openFile()
{
    QString fileName
            = QFileDialog::getOpenFileName(
                    this, tr("Open File"),
                    lastOpenDir.isEmpty() ? "." : lastOpenDir,
                    tr("C/C++ File (*.c *.cc *.cxx *.cpp *.h)"));  // ���ô򿪴���
                                               // �Ի��򣬲�ȡ���û��򿪵��ļ����ļ���
    if (!fileName.isEmpty())
    {
        // ������ļ����ļ�����Ϊ��
        lastOpenDir = fileName.mid(0, fileName.lastIndexOf('/'));//ȡ���ļ�����Ŀ¼
        loadFile(fileName);  // �ڶ��ļ��༭����ʾ���ļ�
    }
}

// ���ܣ������ļ������û�����˵������߹������С����桱��ťʱ���á�
// ��������
// ���أ���
void MainWindow::save()
{
    if (getActiveMdiChild() && getActiveMdiChild()->save())
    {
        // ������ļ��༭�����ڴ��ļ��Ӵ��ڣ����Ӵ��ڱ���ɹ�
        updateStatusBar(tr("Save file successfully..."));  // ����״̬����ʾ��Ϣ
    }
}

// ���ܣ����Ϊ�ļ������û�����˵������߹������С����Ϊ����ťʱ���á�
// ��������
// ���أ���
void MainWindow::saveAs()
{
    if (getActiveMdiChild() && getActiveMdiChild()->saveAs())
    {
        // ������ļ��༭�����ڴ��ļ��Ӵ��ڣ����Ӵ��ڱ���ɹ�
        updateStatusBar(tr("Save file successfully..."));  // ����״̬����ʾ��Ϣ
    }
}

// ���ܣ����ļ�ȫ�����棬���û�����˵������߹������С�����ȫ������ťʱ���á�
// ��������
// ���أ���
void MainWindow::saveAll()
{
    // �������ļ��༭���������Ӵ��ڣ��������
    foreach (QMdiSubWindow* windows, mdiArea->subWindowList())
    {
        MdiChild* child = qobject_cast<MdiChild*>(windows->widget());// ��ȡ�Ӵ���
        if(child->save())
        {
            // ����Ӵ��ڱ���ɹ�
            updateStatusBar(tr("Save file successfully..."));  // ����״̬����ʾ��Ϣ
        }
    }
}

// ���ܣ���ӡ�ļ������û�����˵������߹������С���ӡ����ťʱ���á�
// ��������
// ���أ���
void MainWindow::print()
{
    if (getActiveMdiChild())
    {
        // ������ļ��༭���ڴ��ڴ򿪵��ļ�
        getActiveMdiChild()->print(); // ��ӡ�ļ�
        updateStatusBar(tr("Ready to print..."));  // ����״̬����ʾ��Ϣ
    }
}

// ���ܣ��������������û�����˵������߹������С���������ťʱ���á�
// ��������
// ���أ���
void MainWindow::unDo()
{
    if (getActiveMdiChild())
    {
        // ������ļ��༭���ڴ��ڴ򿪵��ļ�
        getActiveMdiChild()->document()->undo();
    }
}

// ���ܣ��������������û�����˵������߹������С���������ťʱ���á�
// ��������
// ���أ���
void MainWindow::reDo()
{
    if (getActiveMdiChild())
    {
        // ������ļ��༭���ڴ��ڴ򿪵��ļ�
        getActiveMdiChild()->document()->redo();
    }
}

// ���ܣ����в��������û�����˵������߹������С����С���ťʱ���á�
// ��������
// ���أ���
void MainWindow::cut()
{
    if (getActiveMdiChild())
    {
        // ������ļ��༭���ڴ��ڴ򿪵��ļ�
        getActiveMdiChild()->cut();
    }
}

// ���ܣ��������������û�����˵������߹������С���������ťʱ���á�
// ��������
// ���أ���
void MainWindow::copy()
{
    if (getActiveMdiChild())
    {
        // ������ļ��༭���ڴ��ڴ򿪵��ļ�
        getActiveMdiChild()->copy();
    }
}

// ���ܣ�ճ�����������û�����˵������߹������С�ճ������ťʱ���á�
// ��������
// ���أ���
void MainWindow::paste()
{
    if (getActiveMdiChild())
    {
        // ������ļ��༭���ڴ��ڴ򿪵��ļ�
        getActiveMdiChild()->paste();
    }
}

// ���ܣ�ɾ�����������û�����˵������߹������С�ɾ������ťʱ���á�
// ��������
// ���أ���
void MainWindow::del()
{
    if (getActiveMdiChild())
    {
        // ������ļ��༭���ڴ��ڴ򿪵��ļ�
        QTextCursor tmp = getActiveMdiChild()->textCursor();
        tmp.removeSelectedText();
        getActiveMdiChild()->setTextCursor(tmp);
    }
}

// ���ܣ�ȫ��ѡ����������û�����˵������߹������С�ȫ��ѡ�񡱰�ťʱ���á�
// ��������
// ���أ���
void MainWindow::selAll()
{

    if (getActiveMdiChild())
    {
        // ������ļ��༭���ڴ��ڴ򿪵��ļ�
        getActiveMdiChild()->selectAll();
    }
}

// ���ܣ�Ԥ������������û�����˵������߹������С�Ԥ������ťʱ���á�
// ��������
// ���أ���
void MainWindow::preProcess()
{
    if (createDir(afterPreDir) &&  // �����ڰ�ȫ�������õ����ļ��У�λ��Ϊ��ǰĿ¼
        createDir(afterVitalDir))
    {
        // ��������ļ��ж��ɹ�����
        afterPreList->clear();  // ���Ԥ������ļ��б�
        QStringList flNmList;  // ���ڴ��Ԥ������ļ��б�

        // ����Ԥ����ǰ���ļ��б���һ����Ԥ����
        for (int i = 0; i < fileList->count(); i++)
        {
            QString orgFlName = fileList->item(i)->text();  // ȡ��Ԥ����ǰ�ļ��б�
                                              // �е��ļ��Ĵ�·�����ļ�������ԭ�ļ���
            if (!orgFlName.isEmpty())
            {
                saveBeforeProcess(orgFlName);

                // ��ԭ�ļ�����Ϊ��
                QString fileName = orgFlName.mid(
                        orgFlName.lastIndexOf('/') + 1);  // ȥ��·�����ļ���
                QString saveFlName = QDir(afterPreDir).
                                     absoluteFilePath(fileName);  // Ԥ�������ļ�
                               // �Ĵ�·�����ļ�������·������Ϊ./pre_compile/fileName

                QString qStr = QString(
                        "./vcp_bin/mcpp.exe -C -W 0 \"%1\" -o \"%2\"")
                        .arg(orgFlName).arg(saveFlName);  // Ԥ��������

                QProcess* process = new QProcess(this);  // ����Ԥ���������Process
                process->start(qStr);  // ��ʼԤ��������process
                QApplication::setOverrideCursor(Qt::WaitCursor);  // �������״̬
                process->waitForFinished();  // �ȴ�Ԥ��������ִ�����
                QString errStr = process->readAllStandardError(); // ��ȡ���д���
                if (1)
                {
                    // ��ִ��Ԥ���������û�з�������
                    flNmList << saveFlName;  // ��Ԥ������ļ��Ĵ�·�����ļ������뵽
                                             // Ԥ������ļ��б�
                    setRsltLst(tr("Succeed in pre-compiling file [ %1 ]!\n"
                                  "   Create a new file [ %2 ]!")
                               .arg(orgFlName)
                               .arg(saveFlName)); // �ڽ����Ϣ�б�����ʾԤ����ɹ���Ϣ
                    needPreCmpl = false;  // ���Ƿ���Ҫ����Ԥ����ı�־λ����Ϊfalse
                }
                else
                {
                    // ��ִ��Ԥ��������з�������
                    setRsltLst(tr("Failed in pre-compiling file [ %1 ]:\n"
                                  "   %2")
                               .arg(orgFlName)
                               .arg(errStr)); // �ڽ����Ϣ�б�����ʾԤ����ɹ���Ϣ
                    needPreCmpl = true; // ���Ƿ���Ҫ����Ԥ����ı�־λ����Ϊfalse
                }
                QApplication::restoreOverrideCursor();
                process->close();
            }
        }

        showFiles(flNmList, afterPreList);  // ��Ԥ������ļ��б�����ʾ����Ԥ������ļ�
        refreshAfterPreList();
        updateProjMenu();  // ���¡����̡���ť״̬
    }
}

// ���ܣ���ȫ������������û�����˵������߹������С���ȫ���롱��ťʱ���á�
// ��������
// ���أ���
void MainWindow::vitalCodedPrcs()
{
    if (!needPreCmpl)
    {
        // ������Ҫ�Ƚ���Ԥ�������
        if (entryFuncName.trimmed().isEmpty())
        {
            // ��δ������ں�����
            setRsltLst(tr("Please enter Entrance Function Name!")); // ��ʾ������Ϣ
            return;
        }
        QStringList flNmList;  // ��Ű�ȫ�������ļ��б�
        finalFlsList->clear();  // ����ȫ������ļ��б�����ʾ���ļ��б����

        for(int i = 0; i < afterPreList->count(); ++i)
        {
            QString flNm = afterPreList->item(i)->text();
            saveBeforeProcess(flNm);
        }

        QString cmbndFlNm = cmbneFlNms(fileList); // ��Ԥ����ǰ�ļ��б��е��ļ���
                                                      // ƴ���������ÿո����
        if (!cmbndFlNm.isEmpty())
        {
            if (runMode == "Debug" &&
                !(createDir("./temp_result") &&
                  createDir("./temp_result/ori_pro") &&
                  createDir("./temp_result/replace_typedef") &&
                  createDir("./temp_result/trans_enum") &&
                  createDir("./temp_result/trans_array") &&
                  createDir("./temp_result/split") &&
                  createDir("./temp_result/sig_info")))
            {
                setRsltLst(tr("ERROR: Failed in creating directory "
                              "for temp files under DEBUG mode!"));
                return;
            }

            // ��ƴ�Ӻ���ļ�����Ϊ��
            FILE *fp;
            fp=fopen("./new_vcpcode/Debug/aaa��txt","w+");

           QString qStr = QString("./vcp2017.exe %1 %2 %3 -o %4")
                          .arg(runMode == "Debug" ? "-D" : "-N")
                          .arg(entryFuncName.trimmed())
                        .arg(cmbndFlNm.trimmed())
                           .arg(afterVitalDir);  // ��ȫ��������

            QProcess* process = new QProcess(this);  // ���а�ȫ���������Process
            process->start(qStr);  // ��ʼ���а�ȫ��������
            QApplication::setOverrideCursor(Qt::WaitCursor); // �������״̬Ϊ�ȴ�
            process->waitForFinished();  // �ȴ���ȫ�����������
            QString outputStr = process->readAllStandardOutput();//��ȡ���б�׼���
            if (!outputStr.isEmpty())
            {
                // ����׼�����Ϊ��
                setRsltLst(outputStr);
            }
            QString errStr = process->readAllStandardError(); // ��ȡ���д�����Ϣ
            if (errStr.isEmpty())
            {
                // ��ִ�а�ȫ����Ĺ�����û�з�������
                setRsltLst(tr("Succeed in vital coding files: [\n%1\n]!")
                           .arg(cmbndFlNm.replace(QString(" "),
                                                  QString("\n")))); // ��ʾ�ɹ���Ϣ
                updateStatusBar(
                        tr("Succeed in vital code process..."));  // ����״̬��
            }
            else
            {
                // ��ִ�а�ȫ����Ĺ����з�������
                setRsltLst(tr("Failed in vital coding files [\n%1\n]! : '%2'!")
                           .arg(cmbndFlNm.replace(QString(" "), QString("\n")))
                           .arg(errStr)); // ��ʾ������Ϣ
                updateStatusBar(
                        tr("Failed in vital code process..."));  // ����״̬��
            }
            QApplication::restoreOverrideCursor();
            process->close();

            getFilesFromDir(flNmList, afterVitalDir);
            showFiles(flNmList, finalFlsList); // ��ʾ��ȫ�������ļ��б�
            refreshFinalList();
        }
        else
        {
            // ��ƴ�Ӻ���ļ���Ϊ�գ�˵��Ԥ������ļ��б������ļ�������ȫ���룬����ʾ������Ϣ
            setRsltLst(tr("NO file in the file list after pre-compiling!"));
        }
    }
    else
    {
        // ����Ҫ�Ƚ���Ԥ�����������ʾ�û��Ƚ���Ԥ�������
        setRsltLst(tr("Need to pre-compile first!"));
    }

}

// ���ܣ���ʾ���ԣ����û�����˵������߹������С����ԡ���ťʱ���á�
// ��������
// ���أ���
void MainWindow::showSetting()
{


//    QString qStr = QString(
//         "vcp2017.exe -N 111 C:/Users/zhouhang/Desktop/vcp2017/debug/test_operation.c -o C:/Users/zhouhang/Desktop/vcp2017/Debug/TestFile/FileTest_Output/VitalCode " );  // Ԥ��������
// C:\\Users\\zhouhang\\Desktop\\helloworld\\helloworld.exe
//C:\\Users\\zhouhang\\Desktop\\vcp2017.exe
//    //C:\\Users\\zhouhang\\Desktop\\process\\process.exe
//   QProcess* process = new QProcess(this);  // ����Ԥ���������Process
//    QString qstr2=QString("-N 111 C:/Users/zhouhang/Desktop/vcp2017/debug/test_operation.c -o C:/Users/zhouhang/Desktop/vcp2017/Debug/TestFile/FileTest_Output/VitalCode");
//    QStringList list1=qstr2.split(" ");
//    QStringList arguments;
//    arguments  <<  "-style"   << "fusion";
//system("C:\\Users\\zhouhang\\Desktop\\vcp2017.exe -N 111 C:/Users/zhouhang/Desktop/vcp2017/debug/test_operation.c -o C:/Users/zhouhang/Desktop/vcp2017/Debug/TestFile/FileTest_Output/VitalCode");
//system("pause");
// process->execute(qStr,arguments);
  // process->start(qStr);  // ��ʼԤ��������process
    //QProcess::startDetached(qStr,list1);
    // �������״̬/
    delete configDialog;
    configDialog = new ConfigDialog();
   connect(configDialog,
          SIGNAL(changeSettings(QString, QString,
                                  QString, QString, QFont)),
            this,
            SLOT(getChangedConfig(QString, QString,
                                  QString, QString, QFont)));
    configDialog->show();
   configDialog->setParent(this, Qt::Dialog);
    configDialog->exec();
}

// ���ܣ���ʾ������Ϣ�����û�����˵������߹������С����ڡ���ťʱ���á�
// ��������
// ���أ���
void MainWindow::about()
{
    QMessageBox::about(this, tr("About"),
                       tr("Vital Coded Processor\nVersion 2.0\n") +
                       tr("Copyright 2009 - 2010 TJU.\nAll rights reserved\n") +
                       tr("Date: 2010-01-13"));
}

// ���ܣ����ļ��б��е��ļ����ڶ��ļ��༭�����½��Ӵ�����ʾ�����û�˫��Ԥ����ǰ�ļ��б�
// Ԥ������ļ��б���ȫ������ļ��б��е��ļ�ʱ���á�
// ������item - �б��е�һ��Ԫ��
// ���أ���
void MainWindow::openItemOfLst(QListWidgetItem* item)
{
    loadFile(currentDir.absoluteFilePath(item->text()));
}

// ���ܣ����ɲ���ָ�����Ӵ����趨Ϊ��ǰ����ڡ�
// ������window - ���趨Ϊ����ڵĴ���ָ��
// ���أ���
void MainWindow::setActiveSubWindow(QWidget* window)
{
    if (window)
    {
        // �����趨Ϊ����ڵĴ���ָ�벻Ϊ0
        mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
    }
}

// ���ܣ������ļ��ͱ༭�˵����͹������и�����ť��״̬����ʼ�������ڼ��л���ǰ��Ӵ���ʱ���á�
// ��������
// ���أ���
void MainWindow::updateFileAndEditMenus()
{
    MdiChild* actChld = getActiveMdiChild();  // ��ȡ��ǰ����Ӵ���
    bool hasMdiChild = (actChld != 0);  // ��ʾ�Ƿ���ڻ���Ӵ��ڵı�ʾ��

    saveAct->setEnabled(hasMdiChild);  // ���ñ��水ť��״̬
    saveAsAct->setEnabled(hasMdiChild);  // �������Ϊ��ť��״̬
    saveAllAct->setEnabled(hasMdiChild);  // ����ȫ�����水ť��״̬
    printAct->setEnabled(hasMdiChild);  // ���ô�ӡ��ť��״̬
    pasteAct->setEnabled(hasMdiChild);  // ����ճ����ť��״̬

    bool hasContent = (actChld &&
                       actChld->toPlainText().count() != 0);  // ��ʾ�Ƿ���ڻ��
                                        // �Ӵ��ڣ��Ҵ��Ӵ�����ʾ���ļ��Ƿ������ݵı�ʾ��
    selAllAct->setEnabled(hasContent);  // ����ѡ��ȫ����ť��״̬

    reDoAct->setEnabled(
            (actChld &&
             actChld->document()->isRedoAvailable()));  // ���ó�����ť��״̬
    unDoAct->setEnabled(
            (actChld &&
             actChld->document()->isUndoAvailable()));  // ����������ť��״̬

    bool hasSelection = (actChld &&
                         actChld->textCursor().hasSelection());  // ��ʾ�Ƿ���ڻ�
                                           // ���ĵ��Ӵ��ڣ��Ҵ��Ӵ����Ƿ��б�ѡ�е�����
    copyAct->setEnabled(hasSelection);  // ���ÿ�����ť��״̬
    cutAct->setEnabled(hasSelection);  // ���ü��а�ť��״̬
    delAct->setEnabled(hasSelection);  // ����ɾ����ť��״̬
}

// ���ܣ����´��ڲ˵���������ť��״̬��ÿ����ʾ���ڲ˵���ʱ���á�
// ��������
// ���أ���
void MainWindow::updateWindowMenu()
{
    windowMenu->clear();  // ��մ��ڲ˵��������а�ť
    windowMenu->addAction(closeAct);  // �ڴ��ڲ˵�������ӹرյ�ǰ���ڰ�ť
    closeAct->setEnabled(getActiveMdiChild() != 0);  // ���ùرյ�ǰ���ڰ�ť��״̬
    windowMenu->addAction(closeAllAct);  // �ڴ��ڲ˵�������ӹر����д��ڰ�ť
    closeAllAct->setEnabled(getActiveMdiChild() != 0);  // ���ùر����д��ڰ�ť��״̬
    windowMenu->addSeparator();  // �ڴ��ڲ˵�������ӷָ���
    windowMenu->addAction(tileAct);  // �ڴ��ڲ˵��������ƽ�����д��ڰ�ť
    tileAct->setEnabled(getActiveMdiChild() != 0);  // ����ƽ�����д��ڰ�ť��״̬
    windowMenu->addAction(cascadeAct);  // �ڴ��ڲ˵�������Ӳ�����д��ڰ�ť
    cascadeAct->setEnabled(getActiveMdiChild() != 0);  // ���ò�����д��ڰ�ť��״̬
    windowMenu->addSeparator();  // �ڴ��ڲ˵�������ӷָ���
    windowMenu->addAction(nextAct);  // �ڴ��ڲ˵����������һ�����ڰ�ť
    nextAct->setEnabled(getActiveMdiChild() != 0);  // ������һ�����ڰ�ť��״̬
    windowMenu->addAction(previousAct);  // �ڴ��ڲ˵����������һ�����ڰ�ť
    previousAct->setEnabled(getActiveMdiChild() != 0);  // ������һ�����ڰ�ť��״̬
    windowMenu->addAction(separatorAct);  // �ڴ��ڲ˵�������ӷָ���

    QList<QMdiSubWindow*> windows = mdiArea->subWindowList(); // ȡ�������Ӵ���
    separatorAct->setVisible(!windows.isEmpty());   // �������һ���ָ�����״̬
                                    // �����ļ��༭���д򿪵��Ӵ��ڣ������һ���ָ����ɼ�

    // ���������Ӵ���
    for(int i = 0; i < windows.size(); i++)
    {
        MdiChild* child =
                qobject_cast<MdiChild*>(windows.at(i)->widget());  // ��i���Ӵ���

        // Ϊ��i���Ӵ��ڣ�i��0��ʼ���ڴ��ڲ˵����趨���֣������ָ�ʽΪһ�����֣�i+1���Ӵ��Ӵ���
        // �򿪵��ļ����ļ������м��ÿո���������磺��0���Ӵ��ڵ�����Ϊ��1 E:/proj/test.cc����
        // ǰ9���Ӵ����趨�˿�ݼ�����i�����ڵĿ�ݼ�Ϊi+1
        QString windowName
                = (i < 9)? tr("&%1 %2").arg(i + 1).arg(child->getCurrentFile())
                  : tr("%1 %2").arg(i + 1).arg(child->getCurrentFile());

        QAction* windowAction =
                windowMenu->addAction(windowName);//������������Ϊ��ť����ڴ��ڲ˵���
        windowAction->setCheckable(true);  // �趨�Ӵ������ְ�ť�Ŀɹ�ѡ��
        windowAction->setChecked(
                child == getActiveMdiChild()); // �����Ӵ���Ϊ��ǰ����ڣ�
                                               // ����������ǰ����Ϲ�

        // Ϊ��ӵ��Ӵ������ְ�ť�ĵ�����������Ӧ�����������ĳһ���ְ�ť���漴�Ѵ��Ӵ����л�
        // Ϊ��ǰ��Ӵ���
        connect(windowAction, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(windowAction, windows.at(i));
    }
}

// ���ܣ�������Ŀ�˵����͹������и�����ť��״̬��
// ��������
// ���أ���
void MainWindow::updateProjMenu()
{
    prePrcsAct->setEnabled(fileList->count()); // ����Ԥ����ť��״̬ΪԤ����ǰ�ļ���
                               // ���е��ļ������ޣ���Ԥ����ǰ�ļ��б��е��ļ�������Ϊ��ʱ��
                               //Ԥ����ť��Ϊ��Ч״̬

    vtlCdPrcsAct->setEnabled(!needPreCmpl);  // ���ð�ȫ���밴ť��״̬�����������Ԥ����
                                             // ��ȫ���밴ť��Ч
}

// ���ܣ�����Ԥ����ǰ�ļ��б�������ļ���ɾ���ļ������ƺ������ĸ���ť��״̬��
// ��������
// ���أ���
void MainWindow::updateDockBtns()
{
    addFlBtn->setEnabled(true);  // ����ļ���ťʼ��Ϊ��Ч

    delFlBtn->setEnabled(
            fileList->count() != 0
            && fileList->selectedItems().count() != 0); // ɾ���ļ���ť��״̬����
                                                        // Ԥ����ǰ�ļ��б��е��ļ�
                                                        // �����ͱ�ѡ�е��ļ�����

    moveUpBtn->setEnabled(
            fileList->count() != 0
            && fileList->currentRow() != 0);  // ���ư�ť��״̬����Ԥ����ǰ�ļ��б��е�
                                              //�ļ������ͱ�ѡ�е��ļ�������λ�ö���

    moveDownBtn->setEnabled(
            fileList->count() != 0
            && fileList->currentRow() != fileList->count() - 1);  // �����ư�ť����
}

// ���ܣ���Ԥ����ǰ�ļ��б�������ļ������û��������ļ���ťʱ���á�
// ��������
// ���أ���
void MainWindow::addFls()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(
            this, tr("Choose files to add..."),
            lastOpenDir.isEmpty() ? "." : lastOpenDir,
            tr("C/C++ File (*.c *cc *cxx *.cpp)"));  // �Ӵ��ļ��Ի����л�ȡ
                                                     // ���ļ��Ĵ�·�����ļ���

    if(!fileNames.isEmpty())
    {
        // ���ļ�����Ϊ��
        lastOpenDir = fileNames.at(0).mid(
                0, fileNames.at(0).lastIndexOf('/')); // �����ļ�����·����Ϊ���һ��
                                                   // �򿪵�·�����Թ��´δ��ļ�ʹ��
        showFiles(fileNames, fileList);  // ����Ԥ����ǰ�ļ��б���ļ���ʾ
        needPreCmpl = true;  // ����Ҫ�Ƚ���Ԥ����ı�ʾ����Ϊtrue
        updateProjMenu();  // ���¹��̲˵���״̬
        updateDockBtns();  // ����Ԥ����ǰ�ļ��б����ĸ���ť״̬
    }
}

// ���ܣ���Ԥ����ǰ�ļ��б���ɾ���ļ������û����ɾ���ļ���ťʱ���á�
// ��������
// ���أ���
void MainWindow::delFls()
{
    delete fileList->takeItem(fileList->currentRow());  // ɾ����ѡ�ļ�
    updateDockBtns();  // ����Ԥ����ǰ�ļ��б����ĸ���ť״̬
    needPreCmpl = true;  // ����Ҫ�Ƚ���Ԥ����ı�ʾ����Ϊtrue
    updateProjMenu();  // ���¹��̲˵���״̬
}

// ���ܣ���Ԥ����ǰ�ļ��б���ĳһָ���ļ�����һ��λ�á����û�������ư�ťʱ���á�
// ��������
// ���أ���
void MainWindow::moveUp()
{
    int orgRow = fileList->currentRow();  // ���ļ�ԭ����λ��
    fileList->insertItem(
            orgRow - 1, fileList->takeItem(orgRow));  // ��ԭ��λ����ɾ����
                                                      // ���뵽��һ��λ��
    fileList->setCurrentRow(orgRow - 1);  // ��ԭ��λ�õ���һ��λ������Ϊ��ǰλ��
    needPreCmpl = true;  // ����Ҫ�Ƚ���Ԥ����ı�ʾ����Ϊtrue
    updateProjMenu();  // ���¹��̲˵���״̬
}

// ���ܣ���Ԥ����ǰ�ļ��б���ĳһָ���ļ�����һ��λ�á����û�������ư�ťʱ���á�
// ��������
// ���أ���
void MainWindow::moveDown()
{
    int orgRow = fileList->currentRow();  // ���ļ�ԭ����λ��
    fileList->insertItem(
            orgRow + 1, fileList->takeItem(orgRow));  // ��ԭ��λ����ɾ����
                                                      // ���뵽��һ��λ��
    fileList->setCurrentRow(orgRow + 1);  // ��ԭ��λ�õ���һ��λ������Ϊ��ǰλ��
    needPreCmpl = true;  // ����Ҫ�Ƚ���Ԥ����ı�ʾ����Ϊtrue
    updateProjMenu();  // ���¹��̲˵���״̬
}

// ���ܣ�ˢ�¡�Ԥ������ļ��б������û������Ԥ������ļ��б��е�ˢ�°�ťʱ���á�
// ��������
// ���أ���
void MainWindow::refreshAfterPreList()
{
    afterPreList->clear();
    QStringList list;
    getFilesFromDir(list, afterPreDir);
    showFiles(list, afterPreList);
}

// ���ܣ�ˢ�¡���ȫ������ļ��б������û��������ȫ������ļ��б��е�ˢ�°�ťʱ���á�
// ��������
// ���أ���
void MainWindow::refreshFinalList()
{
    finalFlsList->clear();
    QStringList list;
    getFilesFromDir(list, afterVitalDir);
    showFiles(list, finalFlsList);
}

// ���ܣ���ȡ���¹���������Ϣ�����û��ر����öԻ���ʱ���á�
// ��������
// ���أ���
void MainWindow::getChangedConfig(QString mode,
                                  QString aftrPrDir, QString aftrVtlDir,
                                  QString funcName, QFont font)
{
    runMode = mode;
    afterPreDir = aftrPrDir;
    afterVitalDir = aftrVtlDir;
    entryFuncName = funcName;
    editorFont = font;
    fileList->setFont(editorFont);
    afterPreList->setFont(editorFont);
    finalFlsList->setFont(editorFont);
    rsltList->setFont(editorFont);

    // �������д��Ӵ��ڵ�����
    foreach (QMdiSubWindow* window, mdiArea->subWindowList())
    {
        MdiChild* mdi = qobject_cast<MdiChild*>(window->widget());
        mdi->setFont(editorFont);
    }
}

// ���ܣ��������������õ������а�ť
// ��������
// ���أ���
void MainWindow::createActions()
{
    // �������½��ļ�����ť
    newFileAct = new QAction(QIcon(":/icons/new.png"), tr("&New..."), this);
    newFileAct->setShortcuts(QKeySequence::New);
    newFileAct->setStatusTip(tr("Create a new file..."));
    connect(newFileAct, SIGNAL(triggered()), this, SLOT(newFile()));

    // ���������ļ�����ť
    openAct = new QAction(QIcon(":/icons/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a file..."));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    // �����������ļ�����ť
    saveAct = new QAction(QIcon(":/icons/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save a open file..."));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    // ���������Ϊ�ļ�����ť
    saveAsAct = new QAction(tr("Save &As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save as..."));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    // ����������ȫ������ť
    saveAllAct = new QAction(tr("Save a&ll"), this);
    saveAllAct->setShortcut(tr("Ctrl+Shift+S"));
    saveAllAct->setStatusTip(tr("Save all open files..."));
    connect(saveAllAct, SIGNAL(triggered()), this, SLOT(saveAll()));

    // ��������ӡ�ļ�����ť
    printAct = new QAction(QIcon(":/icons/print.png"), tr("&Print"), this);
    printAct->setStatusTip(tr("Print files..."));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    // �������˳�ϵͳ����ť
    quitAct = new QAction(tr("E&xit"), this);
    quitAct->setShortcut(tr("Ctrl+Q"));
    quitAct->setStatusTip(tr("Quit this application..."));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    // ��������������ť
    unDoAct = new QAction(QIcon(":/icons/undo.png"), tr("&Undo"), this);
    unDoAct->setShortcuts(QKeySequence::Undo);
    unDoAct->setStatusTip(tr("Undo..."));
    connect(unDoAct, SIGNAL(triggered()), this, SLOT(unDo()));

    // ��������������ť
    reDoAct = new QAction(QIcon(":/icons/redo.png"), tr("&Redo"), this);
    reDoAct->setShortcuts(QKeySequence::Redo);
    reDoAct->setStatusTip(tr("Redo..."));
    connect(reDoAct, SIGNAL(triggered()), this, SLOT(reDo()));

    // ���������С���ť
    cutAct = new QAction(QIcon(":/icons/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the selected text to clipboard..."));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    // ��������������ť
    copyAct = new QAction(QIcon(":/icons/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the selected text to clipboard..."));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    // ������ճ������ť
    pasteAct = new QAction(QIcon(":/icons/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the text from clipboard..."));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    // ������ɾ������ť
    delAct = new QAction(tr("&Delete"), this);
    delAct->setShortcuts(QKeySequence::Delete);
    delAct->setStatusTip(tr("Delete the selected text..."));
    connect(delAct, SIGNAL(triggered()), this, SLOT(del()));

    // ������ѡ��ȫ������ť
    selAllAct = new QAction(tr("Select a&ll"), this);
    selAllAct->setShortcuts(QKeySequence::SelectAll);
    selAllAct->setStatusTip(tr("Select all text..."));
    connect(selAllAct, SIGNAL(triggered()), this, SLOT(selAll()));

    // ������Ԥ������ť
    prePrcsAct = new QAction(QIcon(":/icons/preprocess.png"),
                             tr("&Pre-compile"), this);
    prePrcsAct->setShortcut(tr("Ctrl+P"));
    prePrcsAct->setStatusTip(tr("Pre-compile files..."));
    prePrcsAct->setEnabled(false);
    connect(prePrcsAct, SIGNAL(triggered()), this, SLOT(preProcess()));

    // ��������ȫ���롱��ť
    vtlCdPrcsAct = new QAction(QIcon(":/icons/vitalcoded.png"),
                               tr("&Vital Code"), this);
    vtlCdPrcsAct->setShortcut(tr("Ctrl+V"));
    vtlCdPrcsAct->setStatusTip(tr("Vital code files..."));
   // vtlCdPrcsAct->setEnabled(false);
    connect(vtlCdPrcsAct, SIGNAL(triggered()), this, SLOT(vitalCodedPrcs()));

    // ���������ԡ���ť
    settingAct = new QAction(tr("&Setting"), this);
    settingAct->setStatusTip(tr("Open the Setting panel..."));
    settingAct->setEnabled(true);
    connect(settingAct, SIGNAL(triggered()), this, SLOT(showSetting()));

    // �������رմ��ڡ���ť
    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setShortcut(tr("Alt+F4"));
    closeAct->setStatusTip(tr("Close the activated subwindow..."));
    connect(closeAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeActiveSubWindow()));

    // �������ر����д��ڡ���ť
    closeAllAct = new QAction(tr("Close &All "), this);
    closeAllAct->setStatusTip(tr("Close all subwindows..."));
    connect(closeAllAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeAllSubWindows()));

    // ������ƽ�����д��ڡ���ť
    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile all subwindows..."));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    // ������������д��ڡ���ť
    cascadeAct = new QAction(tr("&Cascade "), this);
    cascadeAct->setStatusTip(tr("Cascade all subwindows..."));
    connect(cascadeAct, SIGNAL(triggered()),
            mdiArea, SLOT(cascadeSubWindows()));

    // ������������һ���ڡ���ť
    nextAct = new QAction(tr("&Next"), this);
    nextAct->setStatusTip(tr("Activate the next subwindow..."));
    nextAct->setShortcut(QKeySequence::NextChild);
    connect(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    // ������������һ���ڡ���ť
    previousAct = new QAction(tr("&Previous"), this);
    previousAct->setStatusTip(tr("Activate the previous subwindow..."));
    previousAct->setShortcut(QKeySequence::PreviousChild);
    connect(previousAct, SIGNAL(triggered()),
            mdiArea, SLOT(activatePreviousSubWindow()));

    // �������ָ�������ť�������Ե��
    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    // ���������ڡ���ť
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("About VCP..."));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

// ���ܣ��������������õ��Ķ���˵���
// ��������
// ���أ���
void MainWindow::createMenus()
{
    // �������ļ����˵��������ڴ˲˵�������ӡ��½��ļ����������ļ������������ļ����������Ϊ����
    // ������ȫ����������ӡ�ļ��������˳�ϵͳ����ť
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newFileAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveAllAct);
    fileMenu->addSeparator();
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    // �������༭���˵��������ڴ˲˵�������ӡ������������������������С�������������
    // ��ճ��������ɾ��������ѡ��ȫ������ť
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(unDoAct);
    editMenu->addAction(reDoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(delAct);
    editMenu->addAction(selAllAct);

    // ��������Ŀ���˵��������ڴ˲˵�������ӡ�Ԥ����������ȫ���롱��ť
    projMenu = menuBar()->addMenu(tr("&Project"));
    projMenu->addAction(prePrcsAct);
    projMenu->addAction(vtlCdPrcsAct);

    // ��������ͼ���˵���
    viewMenu = menuBar()->addMenu(tr("&View"));

    // ���������ԡ��˵���
    propertyMenu = menuBar()->addMenu(tr("Pr&operty"));
    propertyMenu->addAction(settingAct);

    // ���������ڡ��˵���
    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    // �������������˵���
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

// ���ܣ��������������õ��Ķ��������
// ��������
// ���أ���
void MainWindow::createToolBars()
{
    // �������ļ������������ڴ˹���������ӡ��½��ļ����������ļ����������桱������ӡ����ť
    fileTlBar = addToolBar(tr("File"));
    fileTlBar->addAction(newFileAct);
    fileTlBar->addAction(openAct);
    fileTlBar->addAction(saveAct);
    fileTlBar->addAction(printAct);

    // �������༭�����������ڴ˹���������ӡ�������������������ť
    editTlBar = addToolBar(tr("Edit"));
    editTlBar->addAction(unDoAct);
    editTlBar->addAction(reDoAct);

    // ��������Ŀ�����������ڴ˹���������ӡ�Ԥ����������ȫ���롱��ť
    projTlBar = addToolBar(tr("Proj"));
    projTlBar->addAction(prePrcsAct);
    projTlBar->addAction(vtlCdPrcsAct);
}

// ���ܣ��������������õ���״̬��
// ��������
// ���أ���
void MainWindow::createStatusBar()
{
    updateStatusBar(tr("Ready..."));
}

// ���ܣ��������������õ��Ķ��ļ��༭��
// ��������
// ���أ���
void MainWindow::createMdiArea()
{
    mdiArea = new QMdiArea(this);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);  // ����������Ϊ�����ڵ����Ĳ���
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateFileAndEditMenus()));
}

// ���ܣ��������������õ����ĸ��ļ��б��ͣ������
// ��������
// ���أ���
void MainWindow::createDockWindows()
{
    // ������Ԥ����ǰ�ļ�������
    QDockWidget* dockFlLst
            = new QDockWidget(tr("File List before Pre-compiling"), this);
    dockFlLst->setAllowedAreas(
            Qt::LeftDockWidgetArea |
            Qt::RightDockWidgetArea);  // ����ͣ����������
    addFlBtn = createButton(tr("Add files"), SLOT(addFls()));  // ����ļ���ť
    delFlBtn = createButton(tr("Delete files"), SLOT(delFls()));  // ɾ���ļ���ť
    moveUpBtn = createButton(tr("Move up"), SLOT(moveUp()));  // ���ư�ť
    moveDownBtn = createButton(tr("Move down"), SLOT(moveDown()));  // ���ư�ť
    fileList = new QListWidget;
    fileList->setFont(editorFont);
    connect(fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(openItemOfLst(QListWidgetItem*)));
    connect(fileList, SIGNAL(currentRowChanged(int)),
            this, SLOT(updateDockBtns()));
    connect(fileList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(updateDockBtns()));
    QWidget* dockContents1 = new QWidget();  // ���ڲ����ĸ���ť���ļ��б�ؼ�������
    QGridLayout* dockLayout1 = new QGridLayout(dockContents1);
    dockLayout1->setMargin(3);
    dockLayout1->addWidget(addFlBtn, 0, 0);
    dockLayout1->addWidget(delFlBtn, 0, 1);
    dockLayout1->addWidget(moveUpBtn, 0, 2);
    dockLayout1->addWidget(moveDownBtn, 0, 3);
    dockLayout1->addWidget(fileList, 1, 0, 1, 4);
    dockFlLst->setWidget(dockContents1);
    addDockWidget(Qt::LeftDockWidgetArea, dockFlLst);  // ��ʼͣ�����
    viewMenu->addAction(dockFlLst->toggleViewAction()); // ��ӵ���ͼ�˵���
    updateDockBtns();

    // ������Ԥ����֮���ļ�������
    QDockWidget* dockAfterPreLst
            = new QDockWidget(tr("File List after Pre-compiling"), this);
    dockAfterPreLst->setAllowedAreas(
            Qt::LeftDockWidgetArea |
            Qt::RightDockWidgetArea);  // ����ͣ����������
    afterPreList = new QListWidget;
    afterPreList->setFont(editorFont);
    connect(afterPreList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(openItemOfLst(QListWidgetItem*)));
    refreshAfterPreBtn = createButton(tr("Refresh"),
                                      SLOT(refreshAfterPreList()));
    QWidget* dockContents2 = new QWidget();
    QGridLayout* dockLayout2 = new QGridLayout(dockContents2);
    dockLayout2->setMargin(3);
    dockLayout2->addWidget(refreshAfterPreBtn, 0, 3);
    dockLayout2->addWidget(afterPreList, 1, 0, 1, 4);
    dockAfterPreLst->setWidget(dockContents2);
    addDockWidget(Qt::LeftDockWidgetArea, dockAfterPreLst);  // ��ʼͣ�����
    viewMenu->addAction(dockAfterPreLst->toggleViewAction());   // ��ӵ���ͼ�˵���

    // ��������ȫ������ļ�������
    QDockWidget* dockFinalFlsLst
            = new QDockWidget(tr("File List after Vital-coding"), this);
    dockFinalFlsLst->setAllowedAreas(
            Qt::LeftDockWidgetArea |
            Qt::RightDockWidgetArea);  // ����ͣ����������
    finalFlsList = new QListWidget;
    finalFlsList->setFont(editorFont);
    connect(finalFlsList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(openItemOfLst(QListWidgetItem*)));
    refreshFinalBtn = createButton(tr("Refresh"),
                                      SLOT(refreshFinalList()));
    QWidget* dockContents3 = new QWidget();
    QGridLayout* dockLayout3 = new QGridLayout(dockContents3);
    dockLayout3->setMargin(3);
    dockLayout3->addWidget(refreshFinalBtn, 0, 3);
    dockLayout3->addWidget(finalFlsList, 1, 0, 1, 4);
    dockFinalFlsLst->setWidget(dockContents3);
    addDockWidget(Qt::RightDockWidgetArea, dockFinalFlsLst);  // ��ʼͣ�����
    viewMenu->addAction(dockFinalFlsLst->toggleViewAction());  // ��ӵ���ͼ�˵���

    // �����������Ϣ��ʾ������
    QDockWidget* dockRstLst = new QDockWidget(tr("Result/Error"), this);
    dockRstLst->setAllowedAreas(Qt::BottomDockWidgetArea);  // ֻ����ͣ���±�
    rsltList = new QListWidget;
    rsltList->setFont(editorFont);
    connect(rsltList, SIGNAL(itemChanged(QListWidgetItem*)),
            rsltList, SLOT(scrollToBottom()));
    dockRstLst->setWidget(rsltList);
    addDockWidget(Qt::BottomDockWidgetArea, dockRstLst);  // ��ʼͣ���±�
    viewMenu->addAction(dockRstLst->toggleViewAction());  // ��ӵ���ͼ�˵���
}

// ���ܣ�������ť�ؼ�
// ������text - ��ť����, member - �뵥�������󶨵Ĵ������
// ���أ�������QPushButton����ָ��
QPushButton* MainWindow::createButton(const QString& text, const char* member)
{
    QPushButton* btn = new QPushButton(text, this);
    connect(btn, SIGNAL(clicked()), this, member);
    return btn;
}

// ���ܣ��������ļ��Ӵ���
// ��������
// ���أ�������MdiChild����ָ��
MdiChild* MainWindow::createMdiChild()
{
    MdiChild* child = new MdiChild(mdiArea);

    child->setFont(editorFont); // ��������

    mdiArea->addSubWindow(child);  // �����Ӵ�����ӵ����ļ��༭����

    connect(child, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));  // �󶨿����ź�
    connect(child, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));  // �󶨼����ź�
    connect(child, SIGNAL(undoAvailable(bool)),
            unDoAct, SLOT(setEnabled(bool)));  // �󶨳����ź�
    connect(child, SIGNAL(redoAvailable(bool)),
            reDoAct, SLOT(setEnabled(bool)));  // �������ź�

    return child;
}

// ���ܣ���ָ���б�ؼ�����ʾ�ļ���
// ������files - ����ʾ���ļ����б�listWidget - ��ʾ�ļ����б���б�ؼ�
// ���أ���
void MainWindow::showFiles(const QStringList& files, QListWidget* listWidget)
{
    listWidget->addItems(files);
}

// ���ܣ�������fileNameΪ�ļ������ļ����ڶ��ļ��༭����һ���Ӵ�������ʾ�����Ѵ���һ���Ӵ��ڴ�
//      �˴��ļ�����ֻ������Ӵ��ڡ��������ھʹ���һ���µ��Ӵ��ڣ����Ӵ�����ʾ�ļ���
// ������fileName - �����ص��ļ����ļ���
// ���أ���
void MainWindow::loadFile(const QString& fileName)
{
    if (QMdiSubWindow* existing = findMdiChild(fileName))
    {
        if (existing->widget())
        // ���ҵ��򿪴��ļ����Ӵ��ڣ��򼤻���
        mdiArea->setActiveSubWindow(existing);
    }
    else
    {
        // û���ҵ���Ӧ�Ӵ��ڣ������Ӵ���
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // ���ļ�ʧ��ʱ����
            QMessageBox::warning(this, tr("Error!"),
                                 tr("Failed in opening file '%1':\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        }
        else
        {
            // ���ļ��ɹ��������Ӵ��ڼ����ļ�
            MdiChild* child = createMdiChild();
            child->openFile(&file);
            updateStatusBar(tr("Succeed in loading file..."));
            child->show();
            file.close();
        }
    }
}

// ���ܣ���ȡ�ڶ��ļ��༭���е�ǰ������Ĵ���
// ��������
// ���أ������ڱ�������Ӵ����򷵻ش��Ӵ��ڶ���ָ�룬���򷵻�0��
MdiChild* MainWindow::getActiveMdiChild()
{
    return (mdiArea->activeSubWindow())?
            qobject_cast<MdiChild*>(mdiArea->activeSubWindow()->widget()) : 0;
}

// ���ܣ����Ҵ�ָ���ļ������ļ����Ӵ���
// ������fileName - ָ���ļ����ļ���
// ���أ����ҵ��򷵻ش��Ӵ��ڵĶ���ָ�룬���򷵻�0��
QMdiSubWindow* MainWindow::findMdiChild(const QString& fileName)
{
    foreach (QMdiSubWindow* window, mdiArea->subWindowList())
    {
        MdiChild* mdi = qobject_cast<MdiChild*>(window->widget());
        if(mdi->getCurrentFile() == fileName) return window;
    }
    return 0;
}

// ���ܣ�����ϢstrItem�ڽ����Ϣ��ʾ�б�����ʾ
// ������strItem - ����ʾ����Ϣ
// ���أ���
void MainWindow::setRsltLst(const QString& strItem)
{
    QStringList tmp = strItem.split('\n');
    rsltList->addItems(tmp);
    rsltList->addItem(QString(100, '-'));
    rsltList->scrollToBottom();
}

// ���ܣ�����ϢstatusMsg��״̬������ʾ
// ������statusMsg - ����ʾ����Ϣ
// ���أ���
void MainWindow::updateStatusBar(const QString& statusMsg)
{
    statusBar()->showMessage(statusMsg, kTIME_OUT);
}

// ���ܣ�����Ŀ¼
// ������dirStr - Ŀ¼������
// ���أ������ɹ��򷵻�true
bool MainWindow::createDir(const QString& dirStr)
{
    if (dirStr.isEmpty())
    {
        setRsltLst(tr("Name of directory to be created cannot be empty!"));
        return false;
    }
    QDir dir = QDir(dirStr);
    if (!dir.exists())
    {
        // ����Ŀ¼�����ڣ��򴴽���
        if (!dir.mkdir("."))
        {
            // ������ʧ�ܣ��򱨴��ҷ���false
            setRsltLst(tr("Failed in creating directory [ ")
                       + dir.absolutePath() + tr(" ] !"));
            return false;
        }
        else
        {
            // �������ɹ�
            setRsltLst(tr("Succeed in creating directory [ ")
                       + dir.absolutePath() + tr(" ] !"));
        }
    }
    return true;
}

// ���ܣ���listWidget�е��ļ���ƴ���������м��ÿո����
// ������listWidget - �ļ������б�
// ���أ�ƴ�Ӻ���ļ����ַ���
QString MainWindow::cmbneFlNms(const QListWidget* listWidget)
{
    QString tmp = "";
    for(int i = 0; i < listWidget->count(); ++i)
    {
        QString fileNameWithPath = fileList->item(i)->text();
        QString fileName = fileNameWithPath.mid(
                fileNameWithPath.lastIndexOf('/') + 1);  // ȥ��·�����ļ���
        tmp += QString("%1/%2 ").arg(afterPreDir).arg(fileName);
    }
    return tmp;
}

// ���ܣ���Ŀ¼dirName��ȡ���ļ�������Щ�ļ����ļ��������filesList��
// ������filesList - ����ļ������б�dirName - ȡ�ļ���Ŀ¼��
// ���أ���
void MainWindow::getFilesFromDir(QStringList& filesList, const QString& dirName)
{
    QDir flDir = QDir(dirName);
    flDir.setFilter(QDir::Files);
    QStringList filters;
    filters << "*.cpp" << "*.cxx" << "*.cc" << "*.c" << "*.h"; // ���ˣ�ֻȡc/c++�ļ�
    flDir.setNameFilters(filters);
    QFileInfoList list = flDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        filesList << fileInfo.absoluteFilePath();
    }
}

// ���ܣ��������ļ��ж�ȡ������Ϣ
// ��������
// ���أ���
void MainWindow::getConfig()
{
    QDir appDir("./");
    QSettings settings("vcp_config.ini", QSettings::IniFormat);
    settings.setPath(QSettings::IniFormat, QSettings::UserScope,
                     appDir.absolutePath());

    // ��ȡԤ������ļ��б���Ŀ¼
    settings.beginGroup("FilesAfterPrecompile");
    afterPreDir = settings.value(
            "directory", appDir.absolutePath() + "/pre_compile").toString();
    settings.endGroup();

    // ��ȡ��ȫ������ļ��б���Ŀ¼
    settings.beginGroup("FilesAfterVitalCoded");
    afterVitalDir = settings.value(
            "directory", appDir.absolutePath() + "/redcode").toString();
    settings.endGroup();

    // ��ȡ��ں�����
    settings.beginGroup("Entry-pointFunction");
    entryFuncName = settings.value("name", "main").toString();
    settings.endGroup();

    // ��ȡ�༭������
    settings.beginGroup("FontView");
    editorFont = settings.value("family", QFont("Cambria")).value<QFont>();
    settings.endGroup();
    settings.beginGroup("FontView");
    editorFont.setPointSize(settings.value("size", 12).toInt());
    settings.endGroup();
}

// ���ܣ��ж��ļ�filaName��Ԥ�������Ƿ񴴽��ɹ�
// ������fileName - ����ļ����ļ���
// ���أ���������ɹ��򷵻�true
bool MainWindow::isFileCreated(QString fileName)
{
    if (!isFileExists(afterPreDir, fileName))
    {
        setRsltLst(tr("File '") + fileName +
                   tr("' not created under directory [ ") +
                   afterPreDir + tr(" ]!"));
        return false;
    }
    return true;
}

// ���ܣ��ж�Ŀ¼dir�µ��ļ���ΪfileName���ļ��Ƿ����
// ������dir - ������Ŀ¼dir��fileName - �����ļ���
// ���أ���������򷵻�true
bool MainWindow::isFileExists(QString dir, QString fileName)
{
    QFile* file = new QFile(QDir(dir).absoluteFilePath(fileName));
    bool result = true;
    if (!file->exists())
    {
        result = false;
    }
    file->close();
    delete file;
    return result;
}

// ���ܣ��ж��ļ�fileName�Ƿ���Ҫ��Ԥ�����ȫ����ǰ���б���
// ������fileName - �����ļ����ļ���
// ���أ���
void MainWindow::saveBeforeProcess(QString fileName)
{
    if (QMdiSubWindow* existing = findMdiChild(fileName))
    {
        MdiChild* mdi = qobject_cast<MdiChild*>(existing->widget());
        if (mdi->document()->isModified())
        {
            QMessageBox::StandardButton btn;
            btn = QMessageBox::warning(this, tr("Attention!"),
                                       tr("File '%1' has been modified,\n"
                                          "Do you want to save it?")
                                       .arg(mdi->getCurrentFile()),
                                       QMessageBox::Yes |
                                       QMessageBox::No);
            switch(btn)
            {
                case QMessageBox::Yes:
                    mdi->save();
                    break;
                case QMessageBox::No:
                default:
                    break;
            }
        }
    }
}
