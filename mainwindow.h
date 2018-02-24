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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtGui/QMainWindow>
#include <QDir>
#include "mdichild.h"
#include "ui_config.h"
#include "configdialog.h"

class MdiChild; // 定义在 mdichild.cpp中
QT_BEGIN_NAMESPACE
class QAction;
class QMdiArea;
class QListWidget;
class QMenu;
class QLineEdit;
class QLabel;
class QPushButton;
class QListWidgetItem;
class QSignalMapper;
class QMdiSubWindow;
class QDir;
class QUiLoader;
class QComboBox;
class QSpinBox;
class QDialogButtonBox;
QT_END_NAMESPACE

const int kTIME_OUT = 3000; // 状态栏消息显示时间

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void newFile(); // 新建文件
    void openFile(); // 打开文件
    void save(); // 保存文件
    void saveAs(); // 文件另存为
    void saveAll(); // 保存所有文件
    void print(); // 打印
    void about(); // 显示”关于“信息
    void unDo(); // 撤销
    void reDo(); // 重做
    void cut(); // 剪切
    void copy(); // 复制
    void paste(); // 粘贴
    void del(); // 删除选中文字
    void selAll(); // 全选
    void preProcess(); // ”预处理“操作
    void vitalCodedPrcs(); // ”安全编码“操作
    void showSetting(); // 显示属性
    void updateFileAndEditMenus(); // 更新所有菜单
    void updateWindowMenu(); // 更新”窗口“菜单
    void updateProjMenu(); // 更新”项目“菜单
    void updateDockBtns(); // 更新”预处理前文件“中的四个按钮状态
    void openItemOfLst(QListWidgetItem* item); // 打开三个文件列表的某一项
    void setActiveSubWindow(QWidget* window); // 设置多文件编辑区被激活的子窗口
    void addFls(); // ”预处理前文件“中”添加文件“按钮操作
    void delFls(); // ”预处理前文件“中”删除文件“按钮操作
    void moveUp(); // ”预处理前文件“中”上移“按钮操作
    void moveDown(); // ”预处理前文件“中”下移“按钮操作
    void refreshAfterPreList(); // 刷新“预处理后文件列表”
    void refreshFinalList();  // 刷新“安全编码后文件列表”
    void getChangedConfig(QString mode, QString afterPreDir,
                          QString afterVitalDir, QString entryFuncName,
                          QFont editorFont); // 更新配置信息

private:
    void createActions(); // 创建用于组成菜单和工具栏的QAction对象
    void createMenus(); // 创建菜单栏
    void createToolBars(); // 创建工具栏
    void createStatusBar(); // 创建状态栏
    void createMdiArea(); // 创建多文档编辑区
    void createDockWindows(); // 创建三个文件列表和”结果信息显示“列表的父窗口
    QPushButton* createButton(const QString& text, const char* member);//创建按钮
    MdiChild* createMdiChild(); // 创建MidChild对象指针，一个MidChild对象对应于”文件
                                // 编辑区“的一个子窗口
    void loadFile(const QString& fileName); // 给定文件名载入文件
    void showFiles(const QStringList& files,
                   QListWidget* listWidget); // 在指定列表中显示文件files
    MdiChild* getActiveMdiChild(); // 获取当前”文件编辑区“中被激活的子窗口
    QMdiSubWindow* findMdiChild(const QString& fileName); // 获取打开以fileName为
                                                          // 文件名的文件的子窗口
    void setRsltLst(const QString& strItem); // 显示信息strItem
    void updateStatusBar(const QString& statusMsg); // 更新状态栏
    bool createDir(const QString& dirStr); // 创建以dirStr为相对路径名的目录
    QString cmbneFlNms(
            const QListWidget* listWidget); // 将listWidget中的文件名
                                            // 拼接到一个QString中，用空格隔开
    void getFilesFromDir(QStringList& filesList,
                         const QString& dirName); // 从指定的目录中取出所有文件
    void getConfig();  // 从配置文件中读取配置信息
    bool isFileCreated(QString fileName);  // 判断文件filaName在预处理中是否创建成功
    bool isFileExists(QString dir, QString fileName);  // 判断dir下的文件fileName
                                                       // 是否存在
    void saveBeforeProcess(QString fileName); // 判断文件fileName是否需要在预处理
                                              // 或安全编码前进行保存

    bool needPreCmpl; // 是否需要进行预处理操作的标志
    QString runMode;  // 配置的运行模式
    QString afterPreDir;  // 配置的预处理后的文件目录
    QString afterVitalDir;  // 配置的安全编码的文件目录
    QString entryFuncName;  // 配置的入口函数名
    QFont editorFont;  // 配置的编辑器的字体

    QDir currentDir; // 存放打开文件时正在访问的路径名
    QSignalMapper* windowMapper; // 建立消息映射
    QString lastOpenDir; // 记录最后打开的文件路径

    QMdiArea* mdiArea; // 多文件编辑区域
    QListWidget* fileList; // 预处理前的原始文件列表
    QListWidget* afterPreList; // 预处理后的文件列表
    QListWidget* finalFlsList; // 安全编码后的文件列表
    QListWidget* rsltList; // 结果信息列表

    QMenu* fileMenu; // “文件”菜单栏
    QMenu* editMenu; // “编辑”菜单栏
    QMenu* projMenu; // “项目”菜单栏
    QMenu* viewMenu; // “视图”菜单栏
    QMenu* propertyMenu; // “设置”菜单栏
    QMenu* windowMenu; // “窗口”菜单栏
    QMenu* helpMenu; // “帮助”菜单栏

    QToolBar* fileTlBar; // “文件”工具栏
    QToolBar* editTlBar; // “编辑”工具栏
    QToolBar* projTlBar; // “项目”工具栏

    QAction* newFileAct; // “新建文件”按钮
    QAction* openAct; // “打开文件”按钮
    QAction* saveAct; // “保存文件”按钮
    QAction* saveAsAct; // “另存为文件”按钮
    QAction* saveAllAct; // “保存全部文件”按钮
    QAction* printAct; // “打印文件”按钮
    QAction* quitAct; // “退出系统”按钮
    QAction* unDoAct; // “撤销”按钮
    QAction* reDoAct; // “重做”按钮
    QAction* cutAct; // “剪切”按钮
    QAction* copyAct; // “复制”按钮
    QAction* pasteAct; // “粘贴”按钮
    QAction* delAct; // “删除”按钮
    QAction* selAllAct; // “选择全部”按钮
    QAction* prePrcsAct; // “预处理”按钮
    QAction* vtlCdPrcsAct; // “安全编码”按钮
    QAction* settingAct; // “设置”按钮
    QAction* closeAct; // “关闭当前窗口”按钮
    QAction* closeAllAct; // “关闭所有窗口”按钮
    QAction* tileAct; // “平铺所有窗口”按钮
    QAction* cascadeAct; // “层叠所有窗口”按钮
    QAction* nextAct; // “下一窗口”按钮
    QAction* previousAct; // “前一窗口”按钮
    QAction* separatorAct; // 分隔栏
    QAction* aboutAct; // “关于”按钮

    QPushButton* addFlBtn; // “添加文件”按钮
    QPushButton* delFlBtn; // “删除文件”按钮
    QPushButton* moveUpBtn; // “上移”按钮
    QPushButton* moveDownBtn; // “下移”按钮
    QPushButton* refreshAfterPreBtn;  // 刷新“预处理后文件列表”按钮
    QPushButton* refreshFinalBtn;  // 刷新“安全编码后文件列表”按钮

    ConfigDialog* configDialog;  // 配置信息对话框
};

#endif // MAINWINDOW_H
