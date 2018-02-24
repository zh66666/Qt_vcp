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
    MdiChild(QWidget* parent = 0);  // ���캯��

    void newFile();  // �½��ļ�
    void openFile(QFile* file);  // ���ļ�
    bool needSave();  // �ж��Ƿ���Ҫ����
    bool save();  // �����ļ�
    bool saveAs();  // �ļ����Ϊ
    void print();  // ��ӡ�ļ�
    QString getCurrentFile() const {return curFile;}  // ��ȡ��ǰ���ļ����ļ���
    int lineNumAreaWidth();  // ��ȡ��ǰ���к���ʾ����Ŀ��
    void lineNumAreaPaintEvent(QPaintEvent* event);  // �к���ʾ������ƺ���

protected:
    void closeEvent(QCloseEvent* event);  // ���ڹر��¼�
    void resizeEvent(QResizeEvent* e);  // ���ڴ�С�ı��¼�

private slots:
    void documentBeModified();  // �������ĵ����޸�ʱ����
    void updtLnNmAreaWdth(int newBlockCount);  // ��Ҫ�����к���ʾ������ʱ����
    void hghLtCrrtLn();  // ������ǰ��
    void updtLnArea(const QRect&, int);  // �����к���ʾ����

private:
    bool saveFile(const QString& fileName);  // �����ļ���ΪfileName���ļ�    
    void setCurrentFile(const QString& fileName);  // ���õ�ǰ���ڵĴ��ļ����ļ���

    QString curFile;  // ���浱ǰ���ڵĴ��ļ����ļ���
    bool isUntitled;  // ��־�Ƿ�����

    LineNumArea* lineNumArea;  // �к���ʾ����
    Highlighter* highlighter;  // �﷨����
};

#endif // MDICHILD_H
