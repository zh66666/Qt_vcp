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

#include "highlighter.h"

// ���ܣ����캯��
// ������parent - ���ڵ�
// ���أ���
Highlighter::Highlighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // ���ùؼ��ֵĸ�����ʽ
    keywordFmt.setForeground(Qt::blue);
    keywordFmt.setFontWeight(QFont::Bold);
    QStringList keywordPttns;  // �洢�ؼ��ֵ�ƥ��������ʽ
    keywordPttns << "\\bauto\\b" << "\\bbreak\\b" << "\\bcase\\b" << "\\bchar\\b"
            << "\\bconst\\b" << "\\bcontinue\\b"  << "\\bdefault\\b" << "\\bdo\\b"
            << "\\bdouble\\b" << "\\belse\\b" << "\\benum\\b" << "\\bextern\\b"
            << "\\bfloat\\b" << "\\bfor\\b" << "\\bgoto\\b" << "\\bif\\b"
            << "\\bint\\b" << "\\blong\\b" << "\\bregister\\b" << "\\breturn\\b"
            << "\\bshort\\b" << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b"
            << "\\bstruct\\b" << "\\bswitch\\b" << "\\btypedef\\b" << "\\bunion\\b"
            << "\\bunsigned\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwhile\\b"
            << "^#\\w*|\\s+#\\w*";
    foreach(const QString& pttn, keywordPttns)
    {
        rule.pattern = QRegExp(pttn);
        rule.format = keywordFmt;
        hghlghtingRls.append(rule);
    }

    // ������ֵ�ĸ�����ʽ
    numberFmt.setForeground(Qt::red);
    rule.pattern = QRegExp("\\b\\d+\\b");  // �洢ƥ����ֵ��������ʽ
    rule.format = numberFmt;
    hghlghtingRls.append(rule);

    // �����ַ����ĸ�����ʽ
    stringFmt.setForeground(Qt::magenta);
    rule.pattern = QRegExp("\".*\"");  // �洢ƥ���ַ�����������ʽ�����硰abc��
    rule.format = stringFmt;
    hghlghtingRls.append(rule);

    // ����ע�͵ĸ�����ʽ
    // �������кͶ���ע��
    commentFmt.setForeground(Qt::darkGreen);
    commentFmt.setFontItalic(true);
    cmntStrtExp = QRegExp("/\\*");  // �洢ƥ�䡰/*����������ʽ
    cmntEdExp = QRegExp("\\*/");  // �洢ƥ�䡰*/����������ʽ
    sglCmntStrtExp = QRegExp("//");  // �洢ƥ�䡰//����������ʽ
}

// ���ܣ������������ַ���
// ������text - ��Ҫ�������ַ���
// ���أ���
void Highlighter::highlightBlock(const QString& text)
{
    // �������������б��е�ÿ����������ʹ��������ʽƥ���ַ�������ʹ����Ӧ�ĸ�����ʽ������
    // ƥ�䵽���ַ�������ѭ���������ע�͵ĸ���������ע�͵ĸ�����������ѭ�����������⴦��
    foreach(const HighlightingRule& rule, hghlghtingRls)
    {
        QRegExp exp = rule.pattern;
        int index = text.indexOf(exp);
        while(index >= 0)
        {
            int length = exp.matchedLength();
            setFormat(index, length, rule.format);
            index = exp.indexIn(text, index + length);
        }
    }

    // ��������ע��
    int sglInd = sglCmntStrtExp.indexIn(text);
    setFormat(sglInd, text.length() - sglInd, commentFmt);

    // ��������ע��
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
    {
        startIndex = text.indexOf(cmntStrtExp);
    }
    while (startIndex >= 0)
    {
        int endIndex = text.indexOf(cmntEdExp, startIndex);
        int commentLength;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex
                            + cmntEdExp.matchedLength();
        }
        setFormat(startIndex, commentLength, commentFmt);
        startIndex = text.indexOf(cmntStrtExp,
                                  startIndex + commentLength);
    }
}
