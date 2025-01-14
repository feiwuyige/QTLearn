#include "highlight.h"



highlight::highlight(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    struct HighlightingRule rule;

    //关键字高亮
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        QStringLiteral("\\bchar\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bdouble\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
        QStringLiteral("\\bfriend\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bint\\b"),
        QStringLiteral("\\blong\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
        QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
        QStringLiteral("\\bshort\\b"), QStringLiteral("\\bsignals\\b"), QStringLiteral("\\bsigned\\b"),
        QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstruct\\b"),
        QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypename\\b"),
        QStringLiteral("\\bunion\\b"), QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvirtual\\b"),
        QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bbool\\b")
    };
    for(const QString &pattern : keywordPatterns){
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    //类高亮
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    // //单行注释高亮
    singleLineCommentFormat.setForeground(Qt::red);
    // rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    // rule.format = singleLineCommentFormat;
    // highlightingRules.append(rule);

    // //多行注释高亮
    multiLineCommentFormat.setForeground(Qt::red);
    // rule.pattern = QRegularExpression(QStringLiteral("/\\*[\\s\\S]*?\\*/"));
    // rule.format = multiLineCommentFormat;
    // highlightingRules.append(rule);

    //引用高亮，匹配""之间的字符
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //函数高亮
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
}

void highlight::highlightBlock(const QString &text)
{

    for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    //高亮多行注释
    applyCommentHighlighting(text);
    //高亮单行注释
    applySingleLineCommentHighlighting(text);
    //否则的话如果注释中有关键字，则不会按照注释的高亮规则来进行，所以要定义优先级
}

void highlight::applyCommentHighlighting(const QString &text)
{
    QRegularExpression startExpression("/\\*");
    QRegularExpression endExpression("\\*/");
    int startIndex = 0;
    if (previousBlockState() != 1) {
        startIndex = text.indexOf(startExpression);
    }
    while (startIndex >= 0) {
        QRegularExpressionMatch endMatch = endExpression.match(text, startIndex);
        int endIndex = endMatch.hasMatch() ? endMatch.capturedStart() : -1;
        int commentLength = (endIndex == -1) ? text.length() - startIndex : endIndex - startIndex + 2;

        setFormat(startIndex, commentLength, multiLineCommentFormat);

        if (endIndex == -1) {
            setCurrentBlockState(1); // 标记当前块为多行注释的状态
            break;
        }
        else {
            setCurrentBlockState(0); // 重置状态
        }
        startIndex = text.indexOf(startExpression, startIndex + commentLength);
        // 如果没有进入多行注释，确保当前块状态被重置
        if (startIndex == -1 && previousBlockState() != 1) {
            setCurrentBlockState(0); // 非注释块，重置状态
        }
    }
}



void highlight::applySingleLineCommentHighlighting(const QString &text) {

    QRegularExpression commentPattern("//[^\n]*");
    QRegularExpressionMatchIterator matches = commentPattern.globalMatch(text);

    while (matches.hasNext()) {
        QRegularExpressionMatch match = matches.next();
        setFormat(match.capturedStart(), match.capturedLength(), singleLineCommentFormat);
    }
}
