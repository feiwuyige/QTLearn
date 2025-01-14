#include "codeedit.h"
#include "linenumberarea.h"
#include <QPainter>
#include <QTextBlock>
CodeEdit::CodeEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    //当block count发生改变的时候，发出信号
    connect(this, &CodeEdit::blockCountChanged, this, &CodeEdit::updateLineNumberAreaWidth);
    //当text document 需要一个关于具体的 rect 的更新时发送信号
    connect(this, &CodeEdit::updateRequest, this, &CodeEdit::updateLineNumberArea);
    //光标改变的时候，发送信号
    connect(this, &CodeEdit::cursorPositionChanged, this, &CodeEdit::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void CodeEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while(block.isValid() && top <= event->rect().bottom()){
        if(block.isVisible() && bottom >= event->rect().top()){
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignHCenter, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

//返回行号区域的宽度
//计算出当前最大行号的位数，然后乘以当前字体的一个字符的宽度
//再加上3作为最终的行号区域的宽度
int CodeEdit::lineNumberAreaWidth()
{
    int digits = 1;
    //blockCount()返回document中的text block的数量
    //一个 Text Block 是由 \n（换行符）分隔的文本区域。
    int max = qMax(1, blockCount());
    while(max >= 10){
        max = max / 10;
        ++digits;
    }
    //fontMetrics: return the font metrics for the painter
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

//当用户调整窗口大小时，Qt 自动调用 resizeEvent。
//调用父类的 resizeEvent，确保默认行为被正确执行。
//根据新的内容区域大小，调整行号区域的宽度和高度。
void CodeEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(),cr.height()));
}

//设置滚动区域
//左侧行号不会滚动
void CodeEdit::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    //设置滚动区域周围的边距
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    if(!isReadOnly()){
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::yellow).lighter(160);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}


void CodeEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    //如果滚动，将行号区域也进行滚动
    if(dy){
        lineNumberArea->scroll(0, dy);
    }
    //否则，更新窗口， x y w h
    else{
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }
    //检测更新区域 rect 是否覆盖了视图区域
    if(rect.contains(viewport()->rect())){
        updateLineNumberAreaWidth(0);
    }
}
