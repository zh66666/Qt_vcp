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

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

QT_BEGIN_NAMESPACE
class QSyntaxHighlighter;
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlighter(QTextDocument* parent = 0);

protected:
    void highlightBlock(const QString& text);

private:
    // ����ĸ�������
    struct HighlightingRule
    {
        QRegExp pattern;  // ƥ���ַ�����������ʽ
        QTextCharFormat format;  // ���ַ����ĸ�����ʽ
    };

    QList<HighlightingRule> hghlghtingRls;

    QRegExp cmntStrtExp;  // ƥ�����ע�͵Ŀ�ʼ
    QRegExp cmntEdExp;  // ƥ�����ע�͵Ľ���
    QRegExp sglCmntStrtExp;  // ƥ�䵥��ע��
    QTextCharFormat keywordFmt;  // �ؼ��ֵĸ�����ʽ
    QTextCharFormat numberFmt;  // ��ֵ�ĸ�����ʽ
    QTextCharFormat classFmt;  // �����͵ĸ�����ʽ
    QTextCharFormat commentFmt;  // ע�͵ĸ�����ʽ
    QTextCharFormat functionFmt;  // �����ĸ�����ʽ
    QTextCharFormat stringFmt;  // �ַ����ĸ�����ʽ
};

#endif // HIGHLIGHTER_H
