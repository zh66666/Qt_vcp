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

// 功能：MainWindow构造函数，用于构建界面的整体框架
// 参数：parent - 父窗口指针
// 返回：无
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    needPreCmpl = true;  // 将是否需要进行预处理标志位置为true，即需要进行预处理
    lastOpenDir = "";  // 用于记录上次打开的路径名

    getConfig(); // 从配置文件中读取配置信息

    createMdiArea();  // 创建多文件编辑区域
    createActions();  // 创建按钮
    createMenus();  // 创建目录栏
    createToolBars();  // 创建工具栏
    createStatusBar();  // 创建状态栏
    createDockWindows();  // 创建预处理前文件列表等多个可停靠窗口

    configDialog = new ConfigDialog();
    connect(configDialog,
            SIGNAL(changeSettings(QString, QString,
                                  QString, QString, QFont)),
            this,
            SLOT(getChangedConfig(QString, QString,
                                  QString, QString, QFont)));

    windowMapper = new QSignalMapper(this);  // 用于绑定从窗口菜单栏发出的多个信号
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));  // 将多个信号与
                                                // setActiveSubWindow函数绑定在一起

    updateFileAndEditMenus();  // 更新菜单栏的状态

    setWindowTitle(tr("Vital Coded Processor"));  // 设置主窗口的名字
    setWindowIcon(QIcon(":/icons/mainwindow.png"));  // 设置主窗口的图标
    showMaximized();  // 将主窗口最大化
}

// 功能：MainWindow析构函数
// 参数：无
// 返回：无
MainWindow::~MainWindow()
{
}

// 功能：主窗口关闭事件处理函数，当主窗口接收到窗口关闭请求时执行
// 参数：event - 窗口关闭请求事件
// 返回：无
void MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();  // 将多文件编辑区的子窗口全部关闭

    // 判断多文件编辑区的子窗口是否全部关闭
    getActiveMdiChild() ?
            event->ignore() :  // 若仍有打开的子窗口，则忽略请求事件，既不关闭主窗口
            event->accept();   // 否则接受事件，即关闭主窗口
}

// 功能：创建新文件，当用户点击菜单栏或者工具栏中“新建文件”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::newFile()
{
    MdiChild* child = createMdiChild();  // 创建一个MdiChild对象用于创建一个新文件
    child->newFile(); // 创建新文件
    child->show(); // 显示新文件子窗口
}

// 功能：打开存在文件，当用户点击菜单栏或者工具栏中“打开文件”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::openFile()
{
    QString fileName
            = QFileDialog::getOpenFileName(
                    this, tr("Open File"),
                    lastOpenDir.isEmpty() ? "." : lastOpenDir,
                    tr("C/C++ File (*.c *.cc *.cxx *.cpp *.h)"));  // 调用打开窗口
                                               // 对话框，并取得用户打开的文件的文件名
    if (!fileName.isEmpty())
    {
        // 如果打开文件的文件名不为空
        lastOpenDir = fileName.mid(0, fileName.lastIndexOf('/'));//取出文件名的目录
        loadFile(fileName);  // 在多文件编辑区显示打开文件
    }
}

// 功能：保存文件，当用户点击菜单栏或者工具栏中“保存”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::save()
{
    if (getActiveMdiChild() && getActiveMdiChild()->save())
    {
        // 如果多文件编辑区存在打开文件子窗口，且子窗口保存成功
        updateStatusBar(tr("Save file successfully..."));  // 更新状态栏显示信息
    }
}

// 功能：另存为文件，当用户点击菜单栏或者工具栏中“另存为”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::saveAs()
{
    if (getActiveMdiChild() && getActiveMdiChild()->saveAs())
    {
        // 如果多文件编辑区存在打开文件子窗口，且子窗口保存成功
        updateStatusBar(tr("Save file successfully..."));  // 更新状态栏显示信息
    }
}

// 功能：将文件全部保存，当用户点击菜单栏或者工具栏中“保存全部”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::saveAll()
{
    // 遍历多文件编辑区中所有子窗口，逐个保存
    foreach (QMdiSubWindow* windows, mdiArea->subWindowList())
    {
        MdiChild* child = qobject_cast<MdiChild*>(windows->widget());// 获取子窗口
        if(child->save())
        {
            // 如果子窗口保存成功
            updateStatusBar(tr("Save file successfully..."));  // 更新状态栏显示信息
        }
    }
}

// 功能：打印文件，当用户点击菜单栏或者工具栏中“打印”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::print()
{
    if (getActiveMdiChild())
    {
        // 如果多文件编辑区内存在打开的文件
        getActiveMdiChild()->print(); // 打印文件
        updateStatusBar(tr("Ready to print..."));  // 更新状态栏显示信息
    }
}

// 功能：撤销操作，当用户点击菜单栏或者工具栏中“撤销”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::unDo()
{
    if (getActiveMdiChild())
    {
        // 如果多文件编辑区内存在打开的文件
        getActiveMdiChild()->document()->undo();
    }
}

// 功能：重做操作，当用户点击菜单栏或者工具栏中“重做”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::reDo()
{
    if (getActiveMdiChild())
    {
        // 如果多文件编辑区内存在打开的文件
        getActiveMdiChild()->document()->redo();
    }
}

// 功能：剪切操作，当用户点击菜单栏或者工具栏中“剪切”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::cut()
{
    if (getActiveMdiChild())
    {
        // 如果多文件编辑区内存在打开的文件
        getActiveMdiChild()->cut();
    }
}

// 功能：拷贝操作，当用户点击菜单栏或者工具栏中“拷贝”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::copy()
{
    if (getActiveMdiChild())
    {
        // 如果多文件编辑区内存在打开的文件
        getActiveMdiChild()->copy();
    }
}

// 功能：粘贴操作，当用户点击菜单栏或者工具栏中“粘贴”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::paste()
{
    if (getActiveMdiChild())
    {
        // 如果多文件编辑区内存在打开的文件
        getActiveMdiChild()->paste();
    }
}

// 功能：删除操作，当用户点击菜单栏或者工具栏中“删除”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::del()
{
    if (getActiveMdiChild())
    {
        // 如果多文件编辑区内存在打开的文件
        QTextCursor tmp = getActiveMdiChild()->textCursor();
        tmp.removeSelectedText();
        getActiveMdiChild()->setTextCursor(tmp);
    }
}

// 功能：全部选择操作，当用户点击菜单栏或者工具栏中“全部选择”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::selAll()
{

    if (getActiveMdiChild())
    {
        // 如果多文件编辑区内存在打开的文件
        getActiveMdiChild()->selectAll();
    }
}

// 功能：预处理操作，当用户点击菜单栏或者工具栏中“预处理”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::preProcess()
{
    if (createDir(afterPreDir) &&  // 创建在安全编码中用到的文件夹，位置为当前目录
        createDir(afterVitalDir))
    {
        // 如果上述文件夹都成功创建
        afterPreList->clear();  // 清空预处理后文件列表
        QStringList flNmList;  // 用于存放预处理后文件列表

        // 遍历预处理前的文件列表，逐一进行预处理
        for (int i = 0; i < fileList->count(); i++)
        {
            QString orgFlName = fileList->item(i)->text();  // 取出预处理前文件列表
                                              // 中的文件的带路径的文件名，即原文件名
            if (!orgFlName.isEmpty())
            {
                saveBeforeProcess(orgFlName);

                // 若原文件名不为空
                QString fileName = orgFlName.mid(
                        orgFlName.lastIndexOf('/') + 1);  // 去掉路径的文件名
                QString saveFlName = QDir(afterPreDir).
                                     absoluteFilePath(fileName);  // 预处理后的文件
                               // 的带路径的文件名，将路径更改为./pre_compile/fileName

                QString qStr = QString(
                        "./vcp_bin/mcpp.exe -C -W 0 \"%1\" -o \"%2\"")
                        .arg(orgFlName).arg(saveFlName);  // 预处理命令

                QProcess* process = new QProcess(this);  // 运行预处理命令的Process
                process->start(qStr);  // 开始预处理命令process
                QApplication::setOverrideCursor(Qt::WaitCursor);  // 设置鼠标状态
                process->waitForFinished();  // 等待预处理命令执行完成
                QString errStr = process->readAllStandardError(); // 读取所有错误
                if (1)
                {
                    // 若执行预处理过程中没有发生错误
                    flNmList << saveFlName;  // 将预处理后文件的带路径的文件名加入到
                                             // 预处理后文件列表
                    setRsltLst(tr("Succeed in pre-compiling file [ %1 ]!\n"
                                  "   Create a new file [ %2 ]!")
                               .arg(orgFlName)
                               .arg(saveFlName)); // 在结果信息列表中显示预处理成功信息
                    needPreCmpl = false;  // 将是否需要进行预处理的标志位设置为false
                }
                else
                {
                    // 若执行预处理过程中发生错误
                    setRsltLst(tr("Failed in pre-compiling file [ %1 ]:\n"
                                  "   %2")
                               .arg(orgFlName)
                               .arg(errStr)); // 在结果信息列表中显示预处理成功信息
                    needPreCmpl = true; // 将是否需要进行预处理的标志位设置为false
                }
                QApplication::restoreOverrideCursor();
                process->close();
            }
        }

        showFiles(flNmList, afterPreList);  // 在预处理后文件列表中显示经过预处理的文件
        refreshAfterPreList();
        updateProjMenu();  // 更新“工程”按钮状态
    }
}

// 功能：安全编码操作，当用户点击菜单栏或者工具栏中“安全编码”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::vitalCodedPrcs()
{
    if (!needPreCmpl)
    {
        // 若不需要先进行预处理操作
        if (entryFuncName.trimmed().isEmpty())
        {
            // 若未输入入口函数名
            setRsltLst(tr("Please enter Entrance Function Name!")); // 显示错误信息
            return;
        }
        QStringList flNmList;  // 存放安全编码后的文件列表
        finalFlsList->clear();  // 将安全编码后文件列表中显示的文件列表清空

        for(int i = 0; i < afterPreList->count(); ++i)
        {
            QString flNm = afterPreList->item(i)->text();
            saveBeforeProcess(flNm);
        }

        QString cmbndFlNm = cmbneFlNms(fileList); // 将预处理前文件列表中的文件名
                                                      // 拼接起来，用空格隔开
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

            // 若拼接后的文件名不为空
            FILE *fp;
            fp=fopen("./new_vcpcode/Debug/aaa．txt","w+");

           QString qStr = QString("./vcp2017.exe %1 %2 %3 -o %4")
                          .arg(runMode == "Debug" ? "-D" : "-N")
                          .arg(entryFuncName.trimmed())
                        .arg(cmbndFlNm.trimmed())
                           .arg(afterVitalDir);  // 安全编码命令

            QProcess* process = new QProcess(this);  // 运行安全编码命令的Process
            process->start(qStr);  // 开始运行安全编码命令
            QApplication::setOverrideCursor(Qt::WaitCursor); // 设置鼠标状态为等待
            process->waitForFinished();  // 等待安全编码命令结束
            QString outputStr = process->readAllStandardOutput();//读取所有标准输出
            if (!outputStr.isEmpty())
            {
                // 若标准输出不为空
                setRsltLst(outputStr);
            }
            QString errStr = process->readAllStandardError(); // 读取所有错误信息
            if (errStr.isEmpty())
            {
                // 若执行安全编码的过程中没有发生错误
                setRsltLst(tr("Succeed in vital coding files: [\n%1\n]!")
                           .arg(cmbndFlNm.replace(QString(" "),
                                                  QString("\n")))); // 显示成功信息
                updateStatusBar(
                        tr("Succeed in vital code process..."));  // 更新状态栏
            }
            else
            {
                // 若执行安全编码的过程中发生错误
                setRsltLst(tr("Failed in vital coding files [\n%1\n]! : '%2'!")
                           .arg(cmbndFlNm.replace(QString(" "), QString("\n")))
                           .arg(errStr)); // 显示错误信息
                updateStatusBar(
                        tr("Failed in vital code process..."));  // 更新状态栏
            }
            QApplication::restoreOverrideCursor();
            process->close();

            getFilesFromDir(flNmList, afterVitalDir);
            showFiles(flNmList, finalFlsList); // 显示安全编码后的文件列表
            refreshFinalList();
        }
        else
        {
            // 若拼接后的文件名为空，说明预处理后文件列表中无文件用来安全编码，则显示错误信息
            setRsltLst(tr("NO file in the file list after pre-compiling!"));
        }
    }
    else
    {
        // 若需要先进行预处理操作，提示用户先进行预处理操作
        setRsltLst(tr("Need to pre-compile first!"));
    }

}

// 功能：显示属性，当用户点击菜单栏或者工具栏中“属性”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::showSetting()
{


//    QString qStr = QString(
//         "vcp2017.exe -N 111 C:/Users/zhouhang/Desktop/vcp2017/debug/test_operation.c -o C:/Users/zhouhang/Desktop/vcp2017/Debug/TestFile/FileTest_Output/VitalCode " );  // 预处理命令
// C:\\Users\\zhouhang\\Desktop\\helloworld\\helloworld.exe
//C:\\Users\\zhouhang\\Desktop\\vcp2017.exe
//    //C:\\Users\\zhouhang\\Desktop\\process\\process.exe
//   QProcess* process = new QProcess(this);  // 运行预处理命令的Process
//    QString qstr2=QString("-N 111 C:/Users/zhouhang/Desktop/vcp2017/debug/test_operation.c -o C:/Users/zhouhang/Desktop/vcp2017/Debug/TestFile/FileTest_Output/VitalCode");
//    QStringList list1=qstr2.split(" ");
//    QStringList arguments;
//    arguments  <<  "-style"   << "fusion";
//system("C:\\Users\\zhouhang\\Desktop\\vcp2017.exe -N 111 C:/Users/zhouhang/Desktop/vcp2017/debug/test_operation.c -o C:/Users/zhouhang/Desktop/vcp2017/Debug/TestFile/FileTest_Output/VitalCode");
//system("pause");
// process->execute(qStr,arguments);
  // process->start(qStr);  // 开始预处理命令process
    //QProcess::startDetached(qStr,list1);
    // 设置鼠标状态/
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

// 功能：显示关于信息，当用户点击菜单栏或者工具栏中“关于”按钮时调用。
// 参数：无
// 返回：无
void MainWindow::about()
{
    QMessageBox::about(this, tr("About"),
                       tr("Vital Coded Processor\nVersion 2.0\n") +
                       tr("Copyright 2009 - 2010 TJU.\nAll rights reserved\n") +
                       tr("Date: 2010-01-13"));
}

// 功能：打开文件列表中的文件，在多文件编辑区中新建子窗口显示，当用户双击预处理前文件列表、
// 预处理后文件列表、安全编码后文件列表中的文件时调用。
// 参数：item - 列表中的一个元素
// 返回：无
void MainWindow::openItemOfLst(QListWidgetItem* item)
{
    loadFile(currentDir.absoluteFilePath(item->text()));
}

// 功能：将由参数指定的子窗口设定为当前活动窗口。
// 参数：window - 欲设定为活动窗口的窗口指针
// 返回：无
void MainWindow::setActiveSubWindow(QWidget* window)
{
    if (window)
    {
        // 若欲设定为活动窗口的窗口指针不为0
        mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
    }
}

// 功能：更新文件和编辑菜单栏和工具栏中各个按钮的状态。初始化主窗口及切换当前活动子窗口时调用。
// 参数：无
// 返回：无
void MainWindow::updateFileAndEditMenus()
{
    MdiChild* actChld = getActiveMdiChild();  // 获取当前活动的子窗口
    bool hasMdiChild = (actChld != 0);  // 标示是否存在活动的子窗口的标示符

    saveAct->setEnabled(hasMdiChild);  // 设置保存按钮的状态
    saveAsAct->setEnabled(hasMdiChild);  // 设置另存为按钮的状态
    saveAllAct->setEnabled(hasMdiChild);  // 设置全部保存按钮的状态
    printAct->setEnabled(hasMdiChild);  // 设置打印按钮的状态
    pasteAct->setEnabled(hasMdiChild);  // 设置粘贴按钮的状态

    bool hasContent = (actChld &&
                       actChld->toPlainText().count() != 0);  // 标示是否存在活动的
                                        // 子窗口，且此子窗口显示的文件是否有内容的标示符
    selAllAct->setEnabled(hasContent);  // 设置选择全部按钮的状态

    reDoAct->setEnabled(
            (actChld &&
             actChld->document()->isRedoAvailable()));  // 设置撤销按钮的状态
    unDoAct->setEnabled(
            (actChld &&
             actChld->document()->isUndoAvailable()));  // 设置重做按钮的状态

    bool hasSelection = (actChld &&
                         actChld->textCursor().hasSelection());  // 标示是否存在活
                                           // 动的的子窗口，且此子窗口是否有被选中的文字
    copyAct->setEnabled(hasSelection);  // 设置拷贝按钮的状态
    cutAct->setEnabled(hasSelection);  // 设置剪切按钮的状态
    delAct->setEnabled(hasSelection);  // 设置删除按钮的状态
}

// 功能：更新窗口菜单栏各个按钮的状态。每次显示窗口菜单栏时调用。
// 参数：无
// 返回：无
void MainWindow::updateWindowMenu()
{
    windowMenu->clear();  // 清空窗口菜单栏的所有按钮
    windowMenu->addAction(closeAct);  // 在窗口菜单栏里添加关闭当前窗口按钮
    closeAct->setEnabled(getActiveMdiChild() != 0);  // 设置关闭当前窗口按钮的状态
    windowMenu->addAction(closeAllAct);  // 在窗口菜单栏里添加关闭所有窗口按钮
    closeAllAct->setEnabled(getActiveMdiChild() != 0);  // 设置关闭所有窗口按钮的状态
    windowMenu->addSeparator();  // 在窗口菜单栏里添加分隔符
    windowMenu->addAction(tileAct);  // 在窗口菜单栏里添加平铺所有窗口按钮
    tileAct->setEnabled(getActiveMdiChild() != 0);  // 设置平铺所有窗口按钮的状态
    windowMenu->addAction(cascadeAct);  // 在窗口菜单栏里添加层叠所有窗口按钮
    cascadeAct->setEnabled(getActiveMdiChild() != 0);  // 设置层叠所有窗口按钮的状态
    windowMenu->addSeparator();  // 在窗口菜单栏里添加分隔符
    windowMenu->addAction(nextAct);  // 在窗口菜单栏里添加下一个窗口按钮
    nextAct->setEnabled(getActiveMdiChild() != 0);  // 设置下一个窗口按钮的状态
    windowMenu->addAction(previousAct);  // 在窗口菜单栏里添加上一个窗口按钮
    previousAct->setEnabled(getActiveMdiChild() != 0);  // 设置上一个窗口按钮的状态
    windowMenu->addAction(separatorAct);  // 在窗口菜单栏里添加分隔符

    QList<QMdiSubWindow*> windows = mdiArea->subWindowList(); // 取出所有子窗口
    separatorAct->setVisible(!windows.isEmpty());   // 设置最后一个分隔符的状态
                                    // 若多文件编辑区有打开的子窗口，则最后一个分隔符可见

    // 遍历所有子窗口
    for(int i = 0; i < windows.size(); i++)
    {
        MdiChild* child =
                qobject_cast<MdiChild*>(windows.at(i)->widget());  // 第i个子窗口

        // 为第i个子窗口（i从0开始）在窗口菜单中设定名字，此名字格式为一个数字（i+1）加此子窗口
        // 打开的文件的文件名，中间用空格隔开，例如：第0个子窗口的名字为“1 E:/proj/test.cc”，
        // 前9个子窗口设定了快捷键，第i个窗口的快捷键为i+1
        QString windowName
                = (i < 9)? tr("&%1 %2").arg(i + 1).arg(child->getCurrentFile())
                  : tr("%1 %2").arg(i + 1).arg(child->getCurrentFile());

        QAction* windowAction =
                windowMenu->addAction(windowName);//将上述名字作为按钮添加在窗口菜单中
        windowAction->setCheckable(true);  // 设定子窗口名字按钮的可勾选性
        windowAction->setChecked(
                child == getActiveMdiChild()); // 若此子窗口为当前活动窗口，
                                               // 则在其名字前面加上勾

        // 为添加的子窗口名字按钮的点击动作添加响应操作，即点击某一名字按钮后，随即把此子窗口切换
        // 为当前活动子窗口
        connect(windowAction, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(windowAction, windows.at(i));
    }
}

// 功能：更新项目菜单栏和工具栏中各个按钮的状态。
// 参数：无
// 返回：无
void MainWindow::updateProjMenu()
{
    prePrcsAct->setEnabled(fileList->count()); // 设置预处理按钮的状态为预处理前文件列
                               // 表中的文件的有无，即预处理前文件列表中的文件个数不为零时，
                               //预处理按钮才为有效状态

    vtlCdPrcsAct->setEnabled(!needPreCmpl);  // 设置安全编码按钮的状态，若无需进行预处理
                                             // 则安全编码按钮有效
}

// 功能：更新预处理前文件列表中添加文件、删除文件、上移和下移四个按钮的状态。
// 参数：无
// 返回：无
void MainWindow::updateDockBtns()
{
    addFlBtn->setEnabled(true);  // 添加文件按钮始终为有效

    delFlBtn->setEnabled(
            fileList->count() != 0
            && fileList->selectedItems().count() != 0); // 删除文件按钮的状态根据
                                                        // 预处理前文件列表中的文件
                                                        // 个数和被选中的文件而定

    moveUpBtn->setEnabled(
            fileList->count() != 0
            && fileList->currentRow() != 0);  // 上移按钮的状态根据预处理前文件列表中的
                                              //文件个数和被选中的文件所处的位置而定

    moveDownBtn->setEnabled(
            fileList->count() != 0
            && fileList->currentRow() != fileList->count() - 1);  // 与上移按钮类似
}

// 功能：往预处理前文件列表中添加文件。当用户点击添加文件按钮时调用。
// 参数：无
// 返回：无
void MainWindow::addFls()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(
            this, tr("Choose files to add..."),
            lastOpenDir.isEmpty() ? "." : lastOpenDir,
            tr("C/C++ File (*.c *cc *cxx *.cpp)"));  // 从打开文件对话框中获取
                                                     // 打开文件的带路径的文件名

    if(!fileNames.isEmpty())
    {
        // 若文件名不为空
        lastOpenDir = fileNames.at(0).mid(
                0, fileNames.at(0).lastIndexOf('/')); // 将此文件名的路径存为最后一次
                                                   // 打开的路径，以供下次打开文件使用
        showFiles(fileNames, fileList);  // 更新预处理前文件列表的文件显示
        needPreCmpl = true;  // 将需要先进行预处理的标示符置为true
        updateProjMenu();  // 更新工程菜单的状态
        updateDockBtns();  // 更新预处理前文件列表中四个按钮状态
    }
}

// 功能：从预处理前文件列表中删除文件。当用户点击删除文件按钮时调用。
// 参数：无
// 返回：无
void MainWindow::delFls()
{
    delete fileList->takeItem(fileList->currentRow());  // 删除所选文件
    updateDockBtns();  // 更新预处理前文件列表中四个按钮状态
    needPreCmpl = true;  // 将需要先进行预处理的标示符置为true
    updateProjMenu();  // 更新工程菜单的状态
}

// 功能：将预处理前文件列表中某一指定文件上移一个位置。当用户点击上移按钮时调用。
// 参数：无
// 返回：无
void MainWindow::moveUp()
{
    int orgRow = fileList->currentRow();  // 此文件原来的位置
    fileList->insertItem(
            orgRow - 1, fileList->takeItem(orgRow));  // 从原来位置上删除，
                                                      // 插入到上一个位置
    fileList->setCurrentRow(orgRow - 1);  // 将原来位置的上一个位置设置为当前位置
    needPreCmpl = true;  // 将需要先进行预处理的标示符置为true
    updateProjMenu();  // 更新工程菜单的状态
}

// 功能：将预处理前文件列表中某一指定文件下移一个位置。当用户点击下移按钮时调用。
// 参数：无
// 返回：无
void MainWindow::moveDown()
{
    int orgRow = fileList->currentRow();  // 此文件原来的位置
    fileList->insertItem(
            orgRow + 1, fileList->takeItem(orgRow));  // 从原来位置上删除，
                                                      // 插入到下一个位置
    fileList->setCurrentRow(orgRow + 1);  // 将原来位置的下一个位置设置为当前位置
    needPreCmpl = true;  // 将需要先进行预处理的标示符置为true
    updateProjMenu();  // 更新工程菜单的状态
}

// 功能：刷新“预处理后文件列表”。当用户点击“预处理后文件列表”中的刷新按钮时调用。
// 参数：无
// 返回：无
void MainWindow::refreshAfterPreList()
{
    afterPreList->clear();
    QStringList list;
    getFilesFromDir(list, afterPreDir);
    showFiles(list, afterPreList);
}

// 功能：刷新“安全编码后文件列表”。当用户点击“安全编码后文件列表”中的刷新按钮时调用。
// 参数：无
// 返回：无
void MainWindow::refreshFinalList()
{
    finalFlsList->clear();
    QStringList list;
    getFilesFromDir(list, afterVitalDir);
    showFiles(list, finalFlsList);
}

// 功能：获取更新过的配置信息。当用户关闭配置对话框时调用。
// 参数：无
// 返回：无
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

    // 更新所有打开子窗口的字体
    foreach (QMdiSubWindow* window, mdiArea->subWindowList())
    {
        MdiChild* mdi = qobject_cast<MdiChild*>(window->widget());
        mdi->setFont(editorFont);
    }
}

// 功能：创建主窗口中用到的所有按钮
// 参数：无
// 返回：无
void MainWindow::createActions()
{
    // 创建“新建文件”按钮
    newFileAct = new QAction(QIcon(":/icons/new.png"), tr("&New..."), this);
    newFileAct->setShortcuts(QKeySequence::New);
    newFileAct->setStatusTip(tr("Create a new file..."));
    connect(newFileAct, SIGNAL(triggered()), this, SLOT(newFile()));

    // 创建“打开文件”按钮
    openAct = new QAction(QIcon(":/icons/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a file..."));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    // 创建“保存文件”按钮
    saveAct = new QAction(QIcon(":/icons/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save a open file..."));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    // 创建“另存为文件”按钮
    saveAsAct = new QAction(tr("Save &As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save as..."));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    // 创建“保存全部”按钮
    saveAllAct = new QAction(tr("Save a&ll"), this);
    saveAllAct->setShortcut(tr("Ctrl+Shift+S"));
    saveAllAct->setStatusTip(tr("Save all open files..."));
    connect(saveAllAct, SIGNAL(triggered()), this, SLOT(saveAll()));

    // 创建“打印文件”按钮
    printAct = new QAction(QIcon(":/icons/print.png"), tr("&Print"), this);
    printAct->setStatusTip(tr("Print files..."));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    // 创建“退出系统”按钮
    quitAct = new QAction(tr("E&xit"), this);
    quitAct->setShortcut(tr("Ctrl+Q"));
    quitAct->setStatusTip(tr("Quit this application..."));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    // 创建“撤销”按钮
    unDoAct = new QAction(QIcon(":/icons/undo.png"), tr("&Undo"), this);
    unDoAct->setShortcuts(QKeySequence::Undo);
    unDoAct->setStatusTip(tr("Undo..."));
    connect(unDoAct, SIGNAL(triggered()), this, SLOT(unDo()));

    // 创建“重做”按钮
    reDoAct = new QAction(QIcon(":/icons/redo.png"), tr("&Redo"), this);
    reDoAct->setShortcuts(QKeySequence::Redo);
    reDoAct->setStatusTip(tr("Redo..."));
    connect(reDoAct, SIGNAL(triggered()), this, SLOT(reDo()));

    // 创建“剪切”按钮
    cutAct = new QAction(QIcon(":/icons/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the selected text to clipboard..."));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    // 创建“拷贝”按钮
    copyAct = new QAction(QIcon(":/icons/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the selected text to clipboard..."));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    // 创建“粘贴”按钮
    pasteAct = new QAction(QIcon(":/icons/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the text from clipboard..."));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    // 创建“删除”按钮
    delAct = new QAction(tr("&Delete"), this);
    delAct->setShortcuts(QKeySequence::Delete);
    delAct->setStatusTip(tr("Delete the selected text..."));
    connect(delAct, SIGNAL(triggered()), this, SLOT(del()));

    // 创建“选择全部”按钮
    selAllAct = new QAction(tr("Select a&ll"), this);
    selAllAct->setShortcuts(QKeySequence::SelectAll);
    selAllAct->setStatusTip(tr("Select all text..."));
    connect(selAllAct, SIGNAL(triggered()), this, SLOT(selAll()));

    // 创建“预处理”按钮
    prePrcsAct = new QAction(QIcon(":/icons/preprocess.png"),
                             tr("&Pre-compile"), this);
    prePrcsAct->setShortcut(tr("Ctrl+P"));
    prePrcsAct->setStatusTip(tr("Pre-compile files..."));
    prePrcsAct->setEnabled(false);
    connect(prePrcsAct, SIGNAL(triggered()), this, SLOT(preProcess()));

    // 创建“安全编码”按钮
    vtlCdPrcsAct = new QAction(QIcon(":/icons/vitalcoded.png"),
                               tr("&Vital Code"), this);
    vtlCdPrcsAct->setShortcut(tr("Ctrl+V"));
    vtlCdPrcsAct->setStatusTip(tr("Vital code files..."));
   // vtlCdPrcsAct->setEnabled(false);
    connect(vtlCdPrcsAct, SIGNAL(triggered()), this, SLOT(vitalCodedPrcs()));

    // 创建“属性”按钮
    settingAct = new QAction(tr("&Setting"), this);
    settingAct->setStatusTip(tr("Open the Setting panel..."));
    settingAct->setEnabled(true);
    connect(settingAct, SIGNAL(triggered()), this, SLOT(showSetting()));

    // 创建“关闭窗口”按钮
    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setShortcut(tr("Alt+F4"));
    closeAct->setStatusTip(tr("Close the activated subwindow..."));
    connect(closeAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeActiveSubWindow()));

    // 创建“关闭所有窗口”按钮
    closeAllAct = new QAction(tr("Close &All "), this);
    closeAllAct->setStatusTip(tr("Close all subwindows..."));
    connect(closeAllAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeAllSubWindows()));

    // 创建“平铺所有窗口”按钮
    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile all subwindows..."));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    // 创建“层叠所有窗口”按钮
    cascadeAct = new QAction(tr("&Cascade "), this);
    cascadeAct->setStatusTip(tr("Cascade all subwindows..."));
    connect(cascadeAct, SIGNAL(triggered()),
            mdiArea, SLOT(cascadeSubWindows()));

    // 创建“激活下一窗口”按钮
    nextAct = new QAction(tr("&Next"), this);
    nextAct->setStatusTip(tr("Activate the next subwindow..."));
    nextAct->setShortcut(QKeySequence::NextChild);
    connect(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    // 创建“激活上一窗口”按钮
    previousAct = new QAction(tr("&Previous"), this);
    previousAct->setStatusTip(tr("Activate the previous subwindow..."));
    previousAct->setShortcut(QKeySequence::PreviousChild);
    connect(previousAct, SIGNAL(triggered()),
            mdiArea, SLOT(activatePreviousSubWindow()));

    // 创建“分隔符”按钮，不可以点击
    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    // 创建“关于”按钮
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("About VCP..."));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

// 功能：创建主窗口中用到的多个菜单栏
// 参数：无
// 返回：无
void MainWindow::createMenus()
{
    // 创建“文件”菜单栏，即在此菜单栏中添加“新建文件”、“打开文件”、“保存文件”、“另存为”、
    // “保存全部”、“打印文件”及“退出系统”按钮
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

    // 创建“编辑”菜单栏，即在此菜单栏中添加“撤销”、“重做”、“剪切”、“拷贝”、
    // “粘贴”、“删除”及“选择全部”按钮
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(unDoAct);
    editMenu->addAction(reDoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(delAct);
    editMenu->addAction(selAllAct);

    // 创建“项目”菜单栏，即在此菜单栏中添加“预处理”及“安全编码”按钮
    projMenu = menuBar()->addMenu(tr("&Project"));
    projMenu->addAction(prePrcsAct);
    projMenu->addAction(vtlCdPrcsAct);

    // 创建“视图”菜单栏
    viewMenu = menuBar()->addMenu(tr("&View"));

    // 创建“属性”菜单栏
    propertyMenu = menuBar()->addMenu(tr("Pr&operty"));
    propertyMenu->addAction(settingAct);

    // 创建“窗口”菜单栏
    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    // 创建“帮助”菜单栏
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

// 功能：创建主窗口中用到的多个工具栏
// 参数：无
// 返回：无
void MainWindow::createToolBars()
{
    // 创建“文件”工具栏，在此工具栏中添加“新建文件”、“打开文件”、“保存”及“打印”按钮
    fileTlBar = addToolBar(tr("File"));
    fileTlBar->addAction(newFileAct);
    fileTlBar->addAction(openAct);
    fileTlBar->addAction(saveAct);
    fileTlBar->addAction(printAct);

    // 创建“编辑”工具栏，在此工具栏中添加“撤销”及“重做”按钮
    editTlBar = addToolBar(tr("Edit"));
    editTlBar->addAction(unDoAct);
    editTlBar->addAction(reDoAct);

    // 创建“项目”工具栏，在此工具栏中添加“预处理”及“安全编码”按钮
    projTlBar = addToolBar(tr("Proj"));
    projTlBar->addAction(prePrcsAct);
    projTlBar->addAction(vtlCdPrcsAct);
}

// 功能：创建主窗口中用到的状态栏
// 参数：无
// 返回：无
void MainWindow::createStatusBar()
{
    updateStatusBar(tr("Ready..."));
}

// 功能：创建主窗口中用到的多文件编辑区
// 参数：无
// 返回：无
void MainWindow::createMdiArea()
{
    mdiArea = new QMdiArea(this);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);  // 将此区设置为主窗口的中心部件
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateFileAndEditMenus()));
}

// 功能：创建主窗口中用到的四个文件列表可停靠窗口
// 参数：无
// 返回：无
void MainWindow::createDockWindows()
{
    // 创建“预处理前文件”窗口
    QDockWidget* dockFlLst
            = new QDockWidget(tr("File List before Pre-compiling"), this);
    dockFlLst->setAllowedAreas(
            Qt::LeftDockWidgetArea |
            Qt::RightDockWidgetArea);  // 允许停靠左右两边
    addFlBtn = createButton(tr("Add files"), SLOT(addFls()));  // 添加文件按钮
    delFlBtn = createButton(tr("Delete files"), SLOT(delFls()));  // 删除文件按钮
    moveUpBtn = createButton(tr("Move up"), SLOT(moveUp()));  // 上移按钮
    moveDownBtn = createButton(tr("Move down"), SLOT(moveDown()));  // 下移按钮
    fileList = new QListWidget;
    fileList->setFont(editorFont);
    connect(fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(openItemOfLst(QListWidgetItem*)));
    connect(fileList, SIGNAL(currentRowChanged(int)),
            this, SLOT(updateDockBtns()));
    connect(fileList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(updateDockBtns()));
    QWidget* dockContents1 = new QWidget();  // 用于布局四个按钮与文件列表控件的容器
    QGridLayout* dockLayout1 = new QGridLayout(dockContents1);
    dockLayout1->setMargin(3);
    dockLayout1->addWidget(addFlBtn, 0, 0);
    dockLayout1->addWidget(delFlBtn, 0, 1);
    dockLayout1->addWidget(moveUpBtn, 0, 2);
    dockLayout1->addWidget(moveDownBtn, 0, 3);
    dockLayout1->addWidget(fileList, 1, 0, 1, 4);
    dockFlLst->setWidget(dockContents1);
    addDockWidget(Qt::LeftDockWidgetArea, dockFlLst);  // 初始停靠左边
    viewMenu->addAction(dockFlLst->toggleViewAction()); // 添加到视图菜单里
    updateDockBtns();

    // 创建“预处理之后文件”窗口
    QDockWidget* dockAfterPreLst
            = new QDockWidget(tr("File List after Pre-compiling"), this);
    dockAfterPreLst->setAllowedAreas(
            Qt::LeftDockWidgetArea |
            Qt::RightDockWidgetArea);  // 允许停靠左右两边
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
    addDockWidget(Qt::LeftDockWidgetArea, dockAfterPreLst);  // 初始停靠左边
    viewMenu->addAction(dockAfterPreLst->toggleViewAction());   // 添加到视图菜单里

    // 创建“安全编码后文件”窗口
    QDockWidget* dockFinalFlsLst
            = new QDockWidget(tr("File List after Vital-coding"), this);
    dockFinalFlsLst->setAllowedAreas(
            Qt::LeftDockWidgetArea |
            Qt::RightDockWidgetArea);  // 允许停靠左右两边
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
    addDockWidget(Qt::RightDockWidgetArea, dockFinalFlsLst);  // 初始停靠左边
    viewMenu->addAction(dockFinalFlsLst->toggleViewAction());  // 添加到视图菜单里

    // 创建“结果信息显示”窗口
    QDockWidget* dockRstLst = new QDockWidget(tr("Result/Error"), this);
    dockRstLst->setAllowedAreas(Qt::BottomDockWidgetArea);  // 只允许停靠下边
    rsltList = new QListWidget;
    rsltList->setFont(editorFont);
    connect(rsltList, SIGNAL(itemChanged(QListWidgetItem*)),
            rsltList, SLOT(scrollToBottom()));
    dockRstLst->setWidget(rsltList);
    addDockWidget(Qt::BottomDockWidgetArea, dockRstLst);  // 初始停靠下边
    viewMenu->addAction(dockRstLst->toggleViewAction());  // 添加到视图菜单里
}

// 功能：创建按钮控件
// 参数：text - 按钮名字, member - 与单击动作绑定的处理操作
// 返回：创建的QPushButton对象指针
QPushButton* MainWindow::createButton(const QString& text, const char* member)
{
    QPushButton* btn = new QPushButton(text, this);
    connect(btn, SIGNAL(clicked()), this, member);
    return btn;
}

// 功能：创建打开文件子窗口
// 参数：无
// 返回：创建的MdiChild对象指针
MdiChild* MainWindow::createMdiChild()
{
    MdiChild* child = new MdiChild(mdiArea);

    child->setFont(editorFont); // 设置字体

    mdiArea->addSubWindow(child);  // 将此子窗口添加到多文件编辑区中

    connect(child, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));  // 绑定拷贝信号
    connect(child, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));  // 绑定剪切信号
    connect(child, SIGNAL(undoAvailable(bool)),
            unDoAct, SLOT(setEnabled(bool)));  // 绑定撤销信号
    connect(child, SIGNAL(redoAvailable(bool)),
            reDoAct, SLOT(setEnabled(bool)));  // 绑定重做信号

    return child;
}

// 功能：在指定列表控件中显示文件名
// 参数：files - 欲显示的文件名列表，listWidget - 显示文件名列表的列表控件
// 返回：无
void MainWindow::showFiles(const QStringList& files, QListWidget* listWidget)
{
    listWidget->addItems(files);
}

// 功能：加载以fileName为文件名的文件，在多文件编辑区的一个子窗口中显示，若已存在一个子窗口打开
//      了此文件，则只激活该子窗口。若不存在就创建一个新的子窗口，在子窗口显示文件。
// 参数：fileName - 欲加载的文件的文件名
// 返回：无
void MainWindow::loadFile(const QString& fileName)
{
    if (QMdiSubWindow* existing = findMdiChild(fileName))
    {
        if (existing->widget())
        // 若找到打开此文件的子窗口，则激活它
        mdiArea->setActiveSubWindow(existing);
    }
    else
    {
        // 没有找到相应子窗口，创建子窗口
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // 打开文件失败时报错
            QMessageBox::warning(this, tr("Error!"),
                                 tr("Failed in opening file '%1':\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        }
        else
        {
            // 打开文件成功，创建子窗口加载文件
            MdiChild* child = createMdiChild();
            child->openFile(&file);
            updateStatusBar(tr("Succeed in loading file..."));
            child->show();
            file.close();
        }
    }
}

// 功能：获取在多文件编辑区中当前被激活的窗口
// 参数：无
// 返回：若存在被激活的子窗口则返回此子窗口对象指针，否则返回0。
MdiChild* MainWindow::getActiveMdiChild()
{
    return (mdiArea->activeSubWindow())?
            qobject_cast<MdiChild*>(mdiArea->activeSubWindow()->widget()) : 0;
}

// 功能：查找打开指定文件名的文件的子窗口
// 参数：fileName - 指定文件的文件名
// 返回：若找到则返回此子窗口的对象指针，否则返回0。
QMdiSubWindow* MainWindow::findMdiChild(const QString& fileName)
{
    foreach (QMdiSubWindow* window, mdiArea->subWindowList())
    {
        MdiChild* mdi = qobject_cast<MdiChild*>(window->widget());
        if(mdi->getCurrentFile() == fileName) return window;
    }
    return 0;
}

// 功能：将信息strItem在结果信息显示列表中显示
// 参数：strItem - 欲显示的信息
// 返回：无
void MainWindow::setRsltLst(const QString& strItem)
{
    QStringList tmp = strItem.split('\n');
    rsltList->addItems(tmp);
    rsltList->addItem(QString(100, '-'));
    rsltList->scrollToBottom();
}

// 功能：将信息statusMsg在状态栏中显示
// 参数：statusMsg - 欲显示的信息
// 返回：无
void MainWindow::updateStatusBar(const QString& statusMsg)
{
    statusBar()->showMessage(statusMsg, kTIME_OUT);
}

// 功能：创建目录
// 参数：dirStr - 目录的名字
// 返回：创建成功则返回true
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
        // 若此目录不存在，则创建它
        if (!dir.mkdir("."))
        {
            // 若创建失败，则报错且返回false
            setRsltLst(tr("Failed in creating directory [ ")
                       + dir.absolutePath() + tr(" ] !"));
            return false;
        }
        else
        {
            // 若创建成功
            setRsltLst(tr("Succeed in creating directory [ ")
                       + dir.absolutePath() + tr(" ] !"));
        }
    }
    return true;
}

// 功能：将listWidget中的文件名拼接起来，中间用空格隔开
// 参数：listWidget - 文件名的列表
// 返回：拼接后的文件名字符串
QString MainWindow::cmbneFlNms(const QListWidget* listWidget)
{
    QString tmp = "";
    for(int i = 0; i < listWidget->count(); ++i)
    {
        QString fileNameWithPath = fileList->item(i)->text();
        QString fileName = fileNameWithPath.mid(
                fileNameWithPath.lastIndexOf('/') + 1);  // 去掉路径的文件名
        tmp += QString("%1/%2 ").arg(afterPreDir).arg(fileName);
    }
    return tmp;
}

// 功能：从目录dirName中取出文件，将这些文件的文件名存放在filesList中
// 参数：filesList - 存放文件名的列表，dirName - 取文件的目录名
// 返回：无
void MainWindow::getFilesFromDir(QStringList& filesList, const QString& dirName)
{
    QDir flDir = QDir(dirName);
    flDir.setFilter(QDir::Files);
    QStringList filters;
    filters << "*.cpp" << "*.cxx" << "*.cc" << "*.c" << "*.h"; // 过滤，只取c/c++文件
    flDir.setNameFilters(filters);
    QFileInfoList list = flDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        filesList << fileInfo.absoluteFilePath();
    }
}

// 功能：从配置文件中读取配置信息
// 参数：无
// 返回：无
void MainWindow::getConfig()
{
    QDir appDir("./");
    QSettings settings("vcp_config.ini", QSettings::IniFormat);
    settings.setPath(QSettings::IniFormat, QSettings::UserScope,
                     appDir.absolutePath());

    // 读取预处理后文件列表存放目录
    settings.beginGroup("FilesAfterPrecompile");
    afterPreDir = settings.value(
            "directory", appDir.absolutePath() + "/pre_compile").toString();
    settings.endGroup();

    // 读取安全编码后文件列表存放目录
    settings.beginGroup("FilesAfterVitalCoded");
    afterVitalDir = settings.value(
            "directory", appDir.absolutePath() + "/redcode").toString();
    settings.endGroup();

    // 读取入口函数名
    settings.beginGroup("Entry-pointFunction");
    entryFuncName = settings.value("name", "main").toString();
    settings.endGroup();

    // 读取编辑器字体
    settings.beginGroup("FontView");
    editorFont = settings.value("family", QFont("Cambria")).value<QFont>();
    settings.endGroup();
    settings.beginGroup("FontView");
    editorFont.setPointSize(settings.value("size", 12).toInt());
    settings.endGroup();
}

// 功能：判断文件filaName在预处理中是否创建成功
// 参数：fileName - 检查文件的文件名
// 返回：如果创建成功则返回true
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

// 功能：判断目录dir下的文件名为fileName的文件是否存在
// 参数：dir - 待检查的目录dir，fileName - 检查的文件名
// 返回：如果存在则返回true
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

// 功能：判断文件fileName是否需要在预处理或安全编码前进行保存
// 参数：fileName - 检查的文件的文件名
// 返回：无
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
