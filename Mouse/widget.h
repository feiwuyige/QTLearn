#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
/*
该demo的功能：
 1. 单机鼠标左键更换光标
 2. 单机鼠标右键更换光标
 3. 双击鼠标左键窗口最大化
 4. 再次双击窗口恢复
 5. 鼠标移动时窗口跟着移动
 6. 鼠标滚轮影响窗口文字大小
*/
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
protected:
    // Event handlers
    //bool event(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QPoint offset; //记录鼠标偏移
};
#endif // WIDGET_H
