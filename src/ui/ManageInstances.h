#ifndef OPENCEMULATOR_MANAGEINSTANCES_H
#define OPENCEMULATOR_MANAGEINSTANCES_H


#include <QtWidgets/QWidget>
#include <Instance.h>

namespace Ui
{
    class ManageInstances;
}

class ManageInstances : public QWidget
{
Q_OBJECT
public:
    explicit ManageInstances(QWidget *parent = 0);

private:
    Ui::ManageInstances *m_ui;

    InstanceWeakPtr m_instance;

    void refresh();

private slots:

    void instancesChanged(const std::vector<InstancePtr> &instances);

    void on_buttonRemove_clicked();

    void on_buttonDetach_clicked();

    void on_buttonAttach_clicked();

    void on_buttonPower_clicked();

    void on_buttonReboot_clicked();

    void on_buttonCreate_clicked();

    void on_listInstances_currentItemChanged(QListWidgetItem *current);

    void on_lineLabel_editingFinished();
};


#endif //OPENCEMULATOR_MANAGEINSTANCES_H
