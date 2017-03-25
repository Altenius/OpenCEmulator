#include "MainWindow.h"
#include "ScreensWidget.h"
#include <QToolBar>
#include <QMenuBar>
#include <QDebug>
#include <QMenu>

class MainWindow::Ui {
public:
    QWidget *centralWidget;
    QMenuBar *mainMenuBar;
    
    QAction *actionAddComponent;
    QAction *actionAddInstance;
    
    QMenu *componentsMenu;
    QMenu *instancesMenu;
    
    ScreensWidget *screens;
    
    void setupUi(QMainWindow *mainWindow) {
        mainWindow->resize(700, 550);
        
        centralWidget = new QWidget(mainWindow);
        mainWindow->setCentralWidget(centralWidget);
        
        screens = new ScreensWidget(mainWindow);
        mainWindow->setCentralWidget(screens);
        
        componentsMenu = mainWindow->menuBar()->addMenu(tr("Components"));
        instancesMenu = mainWindow->menuBar()->addMenu(tr("Instances"));
        
        actionAddComponent = new QAction(tr("Add"), mainWindow);
        actionAddComponent->setObjectName(QStringLiteral("actionAddComponent"));
        actionAddComponent->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
        componentsMenu->addAction(actionAddComponent);
        
        actionAddInstance = new QAction(tr("Add"), mainWindow);
        actionAddInstance->setObjectName(QStringLiteral("actionAddInstance"));
        instancesMenu->addAction(actionAddInstance);
        
        QMetaObject::connectSlotsByName(mainWindow);
    }
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new MainWindow::Ui) {
    m_ui->setupUi(this);

    setWindowTitle("OpenCEmulator");
}

void MainWindow::on_actionAddInstance_triggered() {
    qInfo() << "Triggered";
}

MainWindow::~MainWindow() {

}
