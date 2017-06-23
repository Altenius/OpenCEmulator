#include "MainWindow.h"
#include "ScreensWidget.h"
#include <QToolBar>
#include <QMenuBar>
#include <QDebug>
#include <QMenu>
#include <iostream>
#include <OpenCEmulator.h>
#include "ui_mainwindow.h"
#include "components/ComponentScreen.h"



/*
class MainWindow::Ui {
public:
    QWidget *centralWidget;
    QMenuBar *mainMenuBar;
    
    QAction *actionManageComponents;
    QAction *actionManageInstances;
    QAction *actionSaveSession;
    
    QMenu *editMenu;
    QMenu *fileMenu;
    
    ScreensWidget *screens;
    
    void setupUi(QMainWindow *mainWindow) {
        mainWindow->resize(700, 550);
        
        centralWidget = new QWidget(mainWindow);
        mainWindow->setCentralWidget(centralWidget);
        
        screens = new ScreensWidget(mainWindow);
        mainWindow->setCentralWidget(screens);

        fileMenu = mainWindow->menuBar()->addMenu(tr("File"));
        editMenu = mainWindow->menuBar()->addMenu(tr("Edit"));
        
        actionManageComponents = new QAction(tr("Manage Components"), mainWindow);
        actionManageComponents->setObjectName(QStringLiteral("actionManageComponents"));
        // actionManageComponents->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
        editMenu->addAction(actionManageComponents);
        
        actionSaveSession = new QAction(tr("Save Session"), mainWindow);
        actionSaveSession->setObjectName("actionSaveSession");
        fileMenu->addAction(actionSaveSession);
        
        actionManageInstances = new QAction(tr("Manage Instances"), mainWindow);
        actionManageInstances->setObjectName(QStringLiteral("actionManageInstances"));
        editMenu->addAction(actionManageInstances);
        
        QMetaObject::connectSlotsByName(mainWindow);
    }
};*/



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    // setWindowTitle("OpenCEmulator");

    connect(&OpenCEmulator::get(), &OpenCEmulator::componentsChanged, this, &MainWindow::componentsChanged);
}



void MainWindow::on_actionManageComponents_triggered()
{
    m_manageComponents.show();
}



void MainWindow::on_actionManageInstances_triggered()
{
    m_manageInstances.show();
}



MainWindow::~MainWindow()
{

}



Q_DECLARE_METATYPE(ComponentWeakPtr);



void MainWindow::componentsChanged(const std::vector<ComponentPtr> &components)
{
    m_ui->listScreens->clear();

    for (const ComponentPtr &component : components) {
        if (component->type() == "screen") {
            QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(component->label()), m_ui->listScreens);
            item->setData(Qt::UserRole, QVariant::fromValue(ComponentWeakPtr(component)));
        }
    }
}



void MainWindow::on_listScreens_itemActivated(QListWidgetItem *item)
{
    if (auto component = item->data(Qt::UserRole).value<ComponentWeakPtr>().lock()) {
        std::shared_ptr<ComponentScreen> screen = std::static_pointer_cast<ComponentScreen>(component);

        if (!screen->widget()) {
            ScreensWidgetPtr widget = std::make_shared<ScreensWidget>();
            screen->setWidget(widget);
            widget->setScreen(screen);

            auto &keyboards = screen->keyboards();
            if (!keyboards.empty()) {
                if (auto keyboard = keyboards[0].lock()) {
                    widget->setKeyboard(keyboard);
                }
            }

            widget->show();
        } else {
            screen->widget()->show();
        }
    }
}



void MainWindow::on_actionSaveSession_triggered()
{
    OpenCEmulator::get().save();
}
