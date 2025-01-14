#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H
#include <QWidget>

QT_BEGIN_NAMESPACE
class QPlainEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class CodeEdit;
class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    LineNumberArea(CodeEdit* editor);
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    CodeEdit* codeeditor;
};

#endif // LINENUMBERAREA_H
