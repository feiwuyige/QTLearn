#include "widget.h"
#include "ui_widget.h"
#include <QCursor>
#include <QApplication>
#include <QPixmap>
#include <QWheelEvent>
void Widget::mousePressEvent(QMouseEvent *event)
{
    //判断左键还是右键
    if(event->button() == Qt::LeftButton){
        QCursor cursor;
        cursor.setShape(Qt::ClosedHandCursor);
        QApplication::setOverrideCursor(cursor);
        offset = event->globalPosition().toPoint() - pos();
    }
    else if(event->button() == Qt::RightButton){
        // 加载图片并缩放到鼠标大小（推荐 32x32 或 64x64）
        QPixmap cursorPixmap(":/img/taojpeg.jpeg");
        QSize cursorSize(32, 32); // 设置鼠标大小
        QPixmap scaledPixmap = cursorPixmap.scaled(cursorSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QCursor cursor(scaledPixmap);
        QApplication::setOverrideCursor(cursor);
    }

}

void Widget::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    //释放事件
    QApplication::restoreOverrideCursor();
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(windowState() != Qt::WindowFullScreen){
            setWindowState(Qt::WindowFullScreen);
        }else{
            setWindowState(Qt::WindowNoState);
        }
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    //移动过程中判断鼠标是左键点击并且移动，那么要用buttons，返回的是鼠标状态的集合
    if(event->buttons() & Qt::LeftButton){
        //获取窗口应当移动到的位置
        QPoint windowpos = event->globalPosition().toPoint() - offset;
        this->move(windowpos);
    }
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //设置鼠标形状
    QCursor cursor;
    cursor.setShape(Qt::OpenHandCursor);
    setCursor(cursor);
}

void Widget::wheelEvent(QWheelEvent *event)
{
    //鼠标滚动远离使用者放大textedit
    if(event->angleDelta().y() > 0){
        qDebug() << "catch wheel event delta > 0" ;
        ui->textEdit->zoomIn();
    }else {
        qDebug() << "catch wheel event delta < 0" ;
        ui->textEdit->zoomOut();
    }
}

Widget::~Widget()
{
    delete ui;
}
