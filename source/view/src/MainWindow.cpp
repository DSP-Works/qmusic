#include <QAction>
#include <QCloseEvent>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include "Application.h"
#include "LogWindow.h"
#include "AudioUnitsManagerWindow.h"
#include "AudioUnitPropertiesWindow.h"
#include "SignalChainWidget.h"
#include "SignalChainScene.h"
#include "SignalChain.h"
#include "SettingsDialog.h"
#include "../include/MainWindow.h"

MainWindow::MainWindow(QWidget *pParent, Qt::WindowFlags flags)
    : QMainWindow(pParent, flags)
{
    setWindowIcon(QIcon(":/icons/qmusic.png"));

    createDockingWindows();

    createActions();
    createMenu();
    createToolBars();

    m_pSignalChainWidget = new SignalChainWidget();
    setCentralWidget(m_pSignalChainWidget);

    connect(m_pSignalChainWidget, SIGNAL(audioUnitSelected(AudioUnit*)),
            m_pAudioUnitPropertiesWindow, SLOT(handleAudioUnitSelected(AudioUnit*)));

    updateActions();

    resize(1280, 800);

    logInfo(tr("*** <b>QMusic</b> version %1 ***").arg(QMUSIC_VERSION));
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    int ret = QMessageBox::question(
                this,
                tr("Quit"),
                tr("Do you want to exit the application<br>"
                   "(all unsaved data will be lost)?"),
                QMessageBox::Yes |
                QMessageBox::No,
                QMessageBox::No);

    if (ret == QMessageBox::No) {
        pEvent->ignore();
        return;
    }

    stopSignalChain();

    QMainWindow::closeEvent(pEvent);
}

void MainWindow::newSignalChain()
{
    int ret = QMessageBox::question(
                this,
                tr("New signal chain"),
                tr("Do you want to create a new signal chain?<br>"
                   "(All unsaved data will be lost)"),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        m_pSignalChainWidget->newSignalChainScene();
        setWindowTitle("New signal chain");
        logInfo(tr("New signal chain created"));
    }
}

void MainWindow::saveSignalChain()
{
    if (m_pSignalChainWidget->sceneFile().isEmpty()) {
        saveAsSignalChain();
        return;
    }

    m_pSignalChainWidget->save(m_pSignalChainWidget->sceneFile());
    setWindowTitle(m_pSignalChainWidget->sceneFile());
}

void MainWindow::saveAsSignalChain()
{
    QString proposedPath = Application::instance()->applicationDirPath();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save signal chain as"),
                                                    proposedPath,
                                                    tr("QMusic signal chain (*.sch)"));
    if (fileName.isEmpty()) {
        return;
    }

    m_pSignalChainWidget->save(fileName);
    logInfo(tr("Signal chain saved as %1").arg(fileName));
    setWindowTitle(fileName);
}

void MainWindow::openSignalChain()
{
    QString proposedPath = Application::instance()->applicationDirPath();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open signal chain"),
                                                    proposedPath,
                                                    tr("QMusic signalchain (*.sch)"));

    if (fileName.isEmpty()) {
        return;
    }

    m_pSignalChainWidget->load(fileName);
    setWindowTitle(fileName);

    logInfo(tr("Loaded signal chain from %1").arg(fileName));
}

void MainWindow::startSignalChain()
{
    m_pSignalChainWidget->scene()->signalChain()->start();
    updateActions();
    logInfo(tr("Synthesizer started"));
}

void MainWindow::stopSignalChain()
{
    m_pSignalChainWidget->scene()->signalChain()->stop();
    updateActions();
    logInfo(tr("Synthesizer stopped"));
}

void MainWindow::editSettings()
{
    SettingsDialog settingsDialog;

    settingsDialog.loadSettings();

    if (settingsDialog.exec() == QDialog::Accepted) {
        settingsDialog.saveSettings();
    }
}

void MainWindow::createDockingWindows()
{
    m_pLogWindow = new LogWindow(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_pLogWindow);

    m_pAudioUnitsManagerWindow = new AudioUnitsManagerWindow(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_pAudioUnitsManagerWindow);

    m_pAudioUnitPropertiesWindow = new AudioUnitPropertiesWindow(this);
    addDockWidget(Qt::RightDockWidgetArea, m_pAudioUnitPropertiesWindow);
}

void MainWindow::createActions()
{
    m_pNewSignalChainAction = new QAction(QIcon(":/icons/new.png"), tr("&New"), this);
    m_pNewSignalChainAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    connect(m_pNewSignalChainAction, SIGNAL(triggered()), this, SLOT(newSignalChain()));

    m_pOpenSignalChainAction = new QAction(QIcon(":/icons/open.png"), tr("&Open..."), this);
    connect(m_pOpenSignalChainAction, SIGNAL(triggered()), this, SLOT(openSignalChain()));

    m_pSaveSignalChainAction = new QAction(QIcon(":/icons/save.png"), tr("&Save"), this);
    m_pSaveSignalChainAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(m_pSaveSignalChainAction, SIGNAL(triggered()), this, SLOT(saveSignalChain()));

    m_pSaveAsSignalChainAction = new QAction(tr("Save &as..."), this);
    connect(m_pSaveAsSignalChainAction, SIGNAL(triggered()), this, SLOT(saveAsSignalChain()));

    m_pQuitAction = new QAction(tr("&Quit"), this);
    m_pQuitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(m_pQuitAction, SIGNAL(triggered()), this, SLOT(close()));

    m_pStartSignalChainAction = new QAction(QIcon(":/icons/play.png"), tr("Start"), this);
    connect(m_pStartSignalChainAction, SIGNAL(triggered()), this, SLOT(startSignalChain()));

    m_pStopSignalChainAction = new QAction(QIcon(":/icons/stop.png"), tr("Stop"), this);
    connect(m_pStopSignalChainAction, SIGNAL(triggered()), this, SLOT(stopSignalChain()));

    m_pSettingsAction = new QAction(QIcon(":/icons/settings.png"), tr("Settings..."), this);
    connect(m_pSettingsAction, SIGNAL(triggered()), this, SLOT(editSettings()));
}

void MainWindow::createMenu()
{
    m_pFileMenu = menuBar()->addMenu(tr("&File"));
    m_pFileMenu->addAction(m_pNewSignalChainAction);
    m_pFileMenu->addAction(m_pOpenSignalChainAction);
    m_pFileMenu->addSeparator();
    m_pFileMenu->addAction(m_pSaveSignalChainAction);
    m_pFileMenu->addAction(m_pSaveAsSignalChainAction);
    m_pFileMenu->addSeparator();
    m_pFileMenu->addAction(m_pQuitAction);

    m_pSoundMenu = menuBar()->addMenu(tr("&Sound"));
    m_pSoundMenu->addAction(m_pStartSignalChainAction);
    m_pSoundMenu->addAction(m_pStopSignalChainAction);
    m_pSoundMenu->addSeparator();
    m_pSoundMenu->addAction(m_pSettingsAction);
}

void MainWindow::createToolBars()
{
    m_pFileToolBar = addToolBar(tr("File"));
    m_pFileToolBar->setObjectName("fileToolBar");
    m_pFileToolBar->addAction(m_pNewSignalChainAction);
    m_pFileToolBar->addAction(m_pOpenSignalChainAction);
    m_pFileToolBar->addAction(m_pSaveSignalChainAction);

    m_pSignalChainToolBar = addToolBar(tr("Signal Chain"));
    m_pSignalChainToolBar->setObjectName("signalChainToolBar");
    m_pSignalChainToolBar->addAction(m_pStartSignalChainAction);
    m_pSignalChainToolBar->addAction(m_pStopSignalChainAction);
}

void MainWindow::updateActions()
{
    bool isStarted = m_pSignalChainWidget->scene()->signalChain()->isStarted();

    m_pNewSignalChainAction->setEnabled(!isStarted);
    m_pOpenSignalChainAction->setEnabled(!isStarted);
    m_pSaveSignalChainAction->setEnabled(!isStarted);
    m_pSaveAsSignalChainAction->setEnabled(!isStarted);

    m_pStartSignalChainAction->setVisible(!isStarted);
    m_pStopSignalChainAction->setVisible(isStarted);
    m_pSettingsAction->setEnabled(!isStarted);
}
