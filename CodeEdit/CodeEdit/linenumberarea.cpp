#include "linenumberarea.h"
#include "codeedit.h"

LineNumberArea::LineNumberArea(CodeEdit *editor) : QWidget(editor), codeeditor(editor)
{

}
//用于提供控件的首选大小，QSize(int width, int height)
QSize LineNumberArea::sizeHint() const
{
    return QSize(codeeditor->lineNumberAreaWidth(), 0);
}
//用于在窗口或控件需要重新绘制时执行绘图操作。
//QPaintEvent *event：包含关于需要重绘的区域的信息，例如需要绘制的矩形区域（通过 event->rect() 获取）。
void LineNumberArea::paintEvent(QPaintEvent *event)
{
    codeeditor->lineNumberAreaPaintEvent(event);
}
