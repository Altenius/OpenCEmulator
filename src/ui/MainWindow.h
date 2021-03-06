#ifndef OPENCEMULATOR_MAINWINDOW_H
#define OPENCEMULATOR_MAINWINDOW_H

#include <QMainWindow>
#include "ScreensWidget.h"
#include "ManageComponents.h"
#include "ManageInstances.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    Ui::MainWindow *m_ui;

private slots:

    void on_actionSaveSession_triggered();

    void componentsChanged(const std::vector<ComponentPtr> &components);

    void on_listScreens_itemActivated(QListWidgetItem *item);
};


#endif //OPENCEMULATOR_MAINWINDOW_H
