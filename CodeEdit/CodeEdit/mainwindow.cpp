#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setupFileMenu();
    setupHelpMenu();
    setupEditor();
    //sets the given widget to be the main window's central widget
    setCentralWidget(editor);
    setWindowTitle(tr("notepad by zyl"));
}

QSize MainWindow::sizeHint() const
{
    return QSize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Syntax Highlighter"),
                       tr("<p>The <b>Syntax Highlighter</b> example shows how " \
                          "to perform simple syntax highlighting by subclassing " \
                          "the QSyntaxHighlighter class and describing " \
                          "highlighting rules using regular expressions.</p>"));
}

void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C++ Files (*.cpp *.h)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text))
            editor->setPlainText(file.readAll());
    }
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new CodeEdit();
    editor->setFont(font);
    highlighter = new highlight(editor->document());
}

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), QKeySequence::New, this,&MainWindow::newFile);
    fileMenu->addAction(tr("&Open..."),QKeySequence::Open, this, [this](){ openFile(); });
    fileMenu->addAction(tr("E&xit"),  QKeySequence::Quit, qApp, &QApplication::quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
}
