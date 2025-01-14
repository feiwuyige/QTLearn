#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE


class highlight : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    highlight(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;
    void applyCommentHighlighting(const QString &text);
    void applySingleLineCommentHighlighting(const QString &text);

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // HIGHLIGHT_H
