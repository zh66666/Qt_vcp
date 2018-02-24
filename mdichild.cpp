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

#include <QtGui>
#include "mdichild.h"

// 功能：构造函数
// 参数：无
// 返回：无
MdiChild::MdiChild(QWidget* parent)
        : QPlainTextEdit(parent)
{
    setWindowIcon(QIcon(":/icons/child.png"));
    setAttribute(Qt::WA_DeleteOnClose);
    setLineWrapMode(QPlainTextEdit::NoWrap);
    curFile = "";
    isUntitled = true;

    lineNumArea = new LineNumArea(this);

    updtLnNmAreaWdth(0);
    connect(this, SIGNAL(blockCountChanged(int)),
            this, SLOT(updtLnNmAreaWdth(int)));
    connect(this, SIGNAL(updateRequest(const QRect&,int)),
            this, SLOT(updtLnArea(const QRect&,int)));
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(hghLtCrrtLn()));

    hghLtCrrtLn();

    highlighter = new Highlighter(document());

}

// 功能：新建文件
// 参数：无
// 返回：无
void MdiChild::newFile()
{
    static int seqNum = 1;

    isUntitled = true;

    curFile = tr("untitled%1").arg(seqNum++);
    setWindowTitle(curFile + "[*]");

    // hard fix begin
    document()->setPlainText("\n");
    document()->setPlainText("");
    document()->setModified(false);
    // hard fix end

    connect(document(), SIGNAL(contentsChanged()),
            this, SLOT(documentBeModified()));
}

// 功能：打开文件
// 参数：无
// 返回：无
void MdiChild::openFile(QFile* file)
{
    QTextStream in(file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(file->fileName());

    connect(document(), SIGNAL(contentsChanged()),
            this, SLOT(documentBeModified()));
}

// 功能：文档被修改
// 参数：无
// 返回：无
void MdiChild::documentBeModified()
{
    setWindowModified(document()->isModified());
}

// 功能：保存文件
// 参数：无
// 返回：无
bool MdiChild::save()
{
    return isUntitled ? saveAs() : saveFile(curFile);
}

// 功能：另存为文件
// 参数：无
// 返回：无
bool MdiChild::saveAs()
{
    QString fileName
            = QFileDialog::getSaveFileName(this,
                                           tr("Save"),
                                           "./untitled",
                                           tr("C/C++ Files (*.c *.cpp *.h)"));
    if(fileName.isEmpty()) return false;
    else return saveFile(fileName);
}

// 功能：保存指定文件名的文件
// 参数：fileName - 文件名
// 返回：无
bool MdiChild::saveFile(const QString& fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Save Error"),
                             tr("Failed in saving file '%1':\n%2!")
                             .arg(fileName).arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << this->toPlainText();
    QApplication::restoreOverrideCursor();

    file.close();
    setCurrentFile(fileName);

    return true;
}

// 功能：打印文件
// 参数：无
// 返回：无
void MdiChild::print()
{
#ifndef QT_NO_PRINTDIALOG
    QPrinter printer;
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() != QDialog::Accepted) return;

    document()->print(&printer);
#endif
}

// 功能：窗口关闭事件
// 参数：无
// 返回：无
void MdiChild::closeEvent(QCloseEvent* event)
{
    (needSave()) ? event->ignore() : event->accept();
}

// 功能：设置当前窗口打开的文件文件名
// 参数：fileName - 文件名
// 返回：无
void MdiChild::setCurrentFile(const QString& fileName)
{
    curFile = fileName;
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(fileName + "[*]");
}

// 功能：是否需要保存
// 参数：无
// 返回：若需要返回true
bool MdiChild::needSave()
{
    if(document()->isModified())
    {
        QMessageBox::StandardButton msgBtn;
        msgBtn = QMessageBox::warning(this, tr("Attention!"),
                                      tr("Save file '%1'?").arg(curFile),
                                      QMessageBox::Save |
                                      QMessageBox::Discard |
                                      QMessageBox::Cancel);
        switch(msgBtn)
        {
            case QMessageBox::Save:
                return !save();
            case QMessageBox::Cancel:
                return true;
            default:
                return false;
        }
    }
    return false;
}

// 功能：获取行号显示区域的宽度
// 参数：无
// 返回：行号显示区域的宽度
int MdiChild::lineNumAreaWidth()
{
    int dfltDgts = 5;
    int extraDgts = 0;
    int blkCnt = qMax(1, blockCount());

    if(blkCnt >= 100000)
    {
        blkCnt /= 10000;
        while(blkCnt >= 10)
        {
            blkCnt /= 10;
            ++extraDgts;
        }
    }
    int width = 3 + fontMetrics().width(QLatin1Char('9')) * (dfltDgts + extraDgts);

    return width;
}

// 功能：窗口大小改变事件
// 参数：e - 大小改变事件
// 返回：无
void MdiChild::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect r = contentsRect();

    lineNumArea->setGeometry(QRect(r.left(), r.top(),
                                   lineNumAreaWidth(), r.height()));
}

// 功能：更新行号显示区域的宽度
// 参数：newBlockCount - 未用到
// 返回：无
void MdiChild::updtLnNmAreaWdth(int newBlockCount)
{
    setViewportMargins(lineNumAreaWidth(), 0, 0, 0);
}

// 功能：更新行号显示区域
// 参数：rect - 行号显示区域，dy - 垂直滚动高度
// 返回：无
void MdiChild::updtLnArea(const QRect& rect, int dy)
{
    if(dy)
    {
        lineNumArea->scroll(0, dy);
    }
    else
    {
        lineNumArea->update(0, rect.y(), lineNumArea->width(), rect.height());
    }

    if(rect.contains(viewport()->rect()))
    {
        updtLnNmAreaWdth(0);
    }
}

// 功能：高亮当前行
// 参数：无
// 返回：无
void MdiChild::hghLtCrrtLn()
{
    QList<QTextEdit::ExtraSelection> extraSels;

    if(!isReadOnly())
    {
        QTextEdit::ExtraSelection sel;
        QColor lineBkColor = QColor(Qt::green).lighter(180);
        sel.format.setBackground(lineBkColor);
        sel.format.setProperty(QTextFormat::FullWidthSelection, true);
        sel.cursor = textCursor();
        sel.cursor.clearSelection();
        extraSels.append(sel);
    }

    setExtraSelections(extraSels);
}

// 功能：绘制行号显示区域
// 参数：无
// 返回：无
void MdiChild::lineNumAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(lineNumArea);
    painter.fillRect(event->rect(), QColor(Qt::lightGray).lighter(120));

    QTextBlock block = firstVisibleBlock();
    int blockNum = block.blockNumber();
    int top =
            (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while(block.isValid() && top <= event->rect().bottom())
    {
        if(block.isVisible() && bottom >= event->rect().top())
        {
            QString strNum = QString::number(blockNum + 1);
            painter.setPen(Qt::darkGray);
            painter.drawText(0, top, lineNumArea->width(),
                             fontMetrics().height(),
                             Qt::AlignRight, strNum);
        }

        block = block.next();
        top = bottom;
        bottom += (int)blockBoundingRect(block).height();
        ++blockNum;
    }
}
