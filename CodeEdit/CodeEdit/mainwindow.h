#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "codeedit.h"
#include <QMainWindow>
#include "highlight.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    //布局管理器会自动调用
    QSize sizeHint() const override;
    ~MainWindow();
public slots:
    void about();
    void newFile();
    void openFile(const QString &path = QString());

private:
    Ui::MainWindow *ui;
    CodeEdit* editor;
    highlight *highlighter;
    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();
};
#endif // MAINWINDOW_H
