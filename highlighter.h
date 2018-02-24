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
    // 定义的高亮规则
    struct HighlightingRule
    {
        QRegExp pattern;  // 匹配字符串的正则表达式
        QTextCharFormat format;  // 该字符串的高亮格式
    };

    QList<HighlightingRule> hghlghtingRls;

    QRegExp cmntStrtExp;  // 匹配多行注释的开始
    QRegExp cmntEdExp;  // 匹配多行注释的结束
    QRegExp sglCmntStrtExp;  // 匹配单行注释
    QTextCharFormat keywordFmt;  // 关键字的高亮格式
    QTextCharFormat numberFmt;  // 数值的高亮格式
    QTextCharFormat classFmt;  // 类类型的高亮格式
    QTextCharFormat commentFmt;  // 注释的高亮格式
    QTextCharFormat functionFmt;  // 函数的高亮格式
    QTextCharFormat stringFmt;  // 字符串的高亮格式
};

#endif // HIGHLIGHTER_H
