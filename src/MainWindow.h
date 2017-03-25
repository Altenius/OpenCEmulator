#ifndef OPENCEMULATOR_MAINWINDOW_H
#define OPENCEMULATOR_MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

    class Ui;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui *m_ui;

private slots:
    void on_actionAddInstance_triggered();
};


#endif //OPENCEMULATOR_MAINWINDOW_H
