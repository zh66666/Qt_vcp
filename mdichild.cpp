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

// ���ܣ����캯��
// ��������
// ���أ���
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

// ���ܣ��½��ļ�
// ��������
// ���أ���
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

// ���ܣ����ļ�
// ��������
// ���أ���
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

// ���ܣ��ĵ����޸�
// ��������
// ���أ���
void MdiChild::documentBeModified()
{
    setWindowModified(document()->isModified());
}

// ���ܣ������ļ�
// ��������
// ���أ���
bool MdiChild::save()
{
    return isUntitled ? saveAs() : saveFile(curFile);
}

// ���ܣ����Ϊ�ļ�
// ��������
// ���أ���
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

// ���ܣ�����ָ���ļ������ļ�
// ������fileName - �ļ���
// ���أ���
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

// ���ܣ���ӡ�ļ�
// ��������
// ���أ���
void MdiChild::print()
{
#ifndef QT_NO_PRINTDIALOG
    QPrinter printer;
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() != QDialog::Accepted) return;

    document()->print(&printer);
#endif
}

// ���ܣ����ڹر��¼�
// ��������
// ���أ���
void MdiChild::closeEvent(QCloseEvent* event)
{
    (needSave()) ? event->ignore() : event->accept();
}

// ���ܣ����õ�ǰ���ڴ򿪵��ļ��ļ���
// ������fileName - �ļ���
// ���أ���
void MdiChild::setCurrentFile(const QString& fileName)
{
    curFile = fileName;
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(fileName + "[*]");
}

// ���ܣ��Ƿ���Ҫ����
// ��������
// ���أ�����Ҫ����true
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

// ���ܣ���ȡ�к���ʾ����Ŀ��
// ��������
// ���أ��к���ʾ����Ŀ��
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

// ���ܣ����ڴ�С�ı��¼�
// ������e - ��С�ı��¼�
// ���أ���
void MdiChild::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect r = contentsRect();

    lineNumArea->setGeometry(QRect(r.left(), r.top(),
                                   lineNumAreaWidth(), r.height()));
}

// ���ܣ������к���ʾ����Ŀ��
// ������newBlockCount - δ�õ�
// ���أ���
void MdiChild::updtLnNmAreaWdth(int newBlockCount)
{
    setViewportMargins(lineNumAreaWidth(), 0, 0, 0);
}

// ���ܣ������к���ʾ����
// ������rect - �к���ʾ����dy - ��ֱ�����߶�
// ���أ���
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

// ���ܣ�������ǰ��
// ��������
// ���أ���
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

// ���ܣ������к���ʾ����
// ��������
// ���أ���
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
