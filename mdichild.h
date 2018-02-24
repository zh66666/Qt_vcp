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
 **/

#ifndef MDICHILD_H
#define MDICHILD_H

#include <QPlainTextEdit>

#include "linenumarea.h"
#include "highlighter.h"

class LineNumArea;
class Highlighter;
QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QCloseEvent;
class QWidget;
class QRect;
class QPlainTextEdit;
class QFile;
QT_END_NAMESPACE

class MdiChild : public QPlainTextEdit
{
    Q_OBJECT

public:
    MdiChild(QWidget* parent = 0);  // 构造函数

    void newFile();  // 新建文件
    void openFile(QFile* file);  // 打开文件
    bool needSave();  // 判断是否需要保存
    bool save();  // 保存文件
    bool saveAs();  // 文件另存为
    void print();  // 打印文件
    QString getCurrentFile() const {return curFile;}  // 获取当前打开文件的文件名
    int lineNumAreaWidth();  // 获取当前的行号显示区域的宽度
    void lineNumAreaPaintEvent(QPaintEvent* event);  // 行号显示区域绘制函数

protected:
    void closeEvent(QCloseEvent* event);  // 窗口关闭事件
    void resizeEvent(QResizeEvent* e);  // 窗口大小改变事件

private slots:
    void documentBeModified();  // 窗口中文档被修改时调用
    void updtLnNmAreaWdth(int newBlockCount);  // 需要更新行号显示区域宽度时调用
    void hghLtCrrtLn();  // 高亮当前行
    void updtLnArea(const QRect&, int);  // 更新行号显示区域

private:
    bool saveFile(const QString& fileName);  // 保存文件名为fileName的文件    
    void setCurrentFile(const QString& fileName);  // 设置当前窗口的打开文件的文件名

    QString curFile;  // 保存当前窗口的打开文件的文件名
    bool isUntitled;  // 标志是否被命名

    LineNumArea* lineNumArea;  // 行号显示区域
    Highlighter* highlighter;  // 语法高亮
};

#endif // MDICHILD_H
