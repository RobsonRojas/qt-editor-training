#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    textEdit = new QPlainTextEdit;
    setCentralWidget(textEdit);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    connect(textEdit->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave()){
        writeSettings();
        event->accept();
    }
    else{
        event->ignore();
    }
}

void MainWindow::open()
{
    if(maybeSave()){
        QString fileName = QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty())
            loadFile(fileName);
    }
}



void MainWindow::newFile()
{
    if(maybeSave()){
        textEdit->clear();
        setCurrentFile("");
    }
}

bool MainWindow::save()
{
    if(curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);
}

bool MainWindow::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this);
    if(fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>Application</b> example demonstrates how to "
                          "write modern GUI applications using Qt, with a menu bar,"
                          "toolbars, and a status bar"));
}

void MainWindow::documentWasModified()
{
    setWindowModified(textEdit->document()->isModified());
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()),this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save file modifications to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QIcon(":/images/save.png"), tr("&Save as"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save file modifications to disk"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(QIcon(":/images/exit.png"), tr("&Exit"), this);
    exitAct->setShortcut(QKeySequence::Close);
    exitAct->setStatusTip(tr("Exit application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("&Cut"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut selected text"));
    //connect(cutAct, SIGNAL(triggered()), this, SLOT(c)
    cutAct->setEnabled(false);
    connect(cutAct, SIGNAL(triggered()),textEdit, SLOT(cut()));
    connect(textEdit, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy selected text"));
    //connect(copyAct, SIGNAL(triggered()), this, SLOT(copy);
    copyAct->setEnabled(false);
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));
    connect(textEdit, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste selected text"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    aboutAct = new QAction(tr("About"), this);
    aboutAct->setStatusTip(tr("Show the application About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("TrollTech", "Application Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();

    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("TrollTech", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size",size());
}

bool MainWindow::maybeSave()
{
    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified. \n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return save();
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));

        return;
    }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write fiel %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);

    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty()) {
        shownName = "untitled.txt";
    }
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

MainWindow::~MainWindow()
{

}
