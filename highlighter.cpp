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

// 功能：构造函数
// 参数：parent - 父节点
// 返回：无
Highlighter::Highlighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // 设置关键字的高亮格式
    keywordFmt.setForeground(Qt::blue);
    keywordFmt.setFontWeight(QFont::Bold);
    QStringList keywordPttns;  // 存储关键字的匹配正则表达式
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

    // 设置数值的高亮格式
    numberFmt.setForeground(Qt::red);
    rule.pattern = QRegExp("\\b\\d+\\b");  // 存储匹配数值的正则表达式
    rule.format = numberFmt;
    hghlghtingRls.append(rule);

    // 设置字符串的高亮格式
    stringFmt.setForeground(Qt::magenta);
    rule.pattern = QRegExp("\".*\"");  // 存储匹配字符串的正则表达式，例如“abc”
    rule.format = stringFmt;
    hghlghtingRls.append(rule);

    // 设置注释的高亮格式
    // 包括单行和多行注释
    commentFmt.setForeground(Qt::darkGreen);
    commentFmt.setFontItalic(true);
    cmntStrtExp = QRegExp("/\\*");  // 存储匹配“/*”的正则表达式
    cmntEdExp = QRegExp("\\*/");  // 存储匹配“*/”的正则表达式
    sglCmntStrtExp = QRegExp("//");  // 存储匹配“//”的正则表达式
}

// 功能：高亮给定的字符块
// 参数：text - 需要高亮的字符块
// 返回：无
void Highlighter::highlightBlock(const QString& text)
{
    // 遍历高亮规则列表中的每个高亮规则，使用正则表达式匹配字符串，再使用相应的高亮格式来设置
    // 匹配到的字符串，此循环不能完成注释的高亮工作，注释的高亮工作需在循环结束后特殊处理
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

    // 高亮单行注释
    int sglInd = sglCmntStrtExp.indexIn(text);
    setFormat(sglInd, text.length() - sglInd, commentFmt);

    // 高亮多行注释
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
