#include <OpenCEmulator.h>
#include <components/ComponentGPU.h>
#include <components/ComponentRealFilesystem.h>
#include <components/ComponentEEPROM.h>
#include <components/ComponentScreen.h>
#include "CreateComponent.h"
#include "ui_createcomponent.h"
#include "ComponentModifier.h"
#include "KeyboardModifier.h"

CreateComponent::CreateComponent(QWidget *parent) : QWidget(parent), m_ui(new Ui::CreateComponent), m_modifier(nullptr) {
    m_ui->setupUi(this);
    
    m_keyboardModifier = new KeyboardModifier(m_ui->frameExtra);
    
    m_ui->comboTypes->addItem("keyboard");
    m_ui->comboTypes->addItem("screen");
    m_ui->comboTypes->addItem("gpu");
    m_ui->comboTypes->addItem("filesystem");
    m_ui->comboTypes->addItem("eeprom");
}

void CreateComponent::on_comboTypes_currentIndexChanged(const QString &text) {
    if (m_modifier != nullptr) {
        m_modifier->hide();
        
        m_modifier = nullptr;
    }
    
    if (text == "keyboard") {
        m_modifier = m_keyboardModifier;
        m_modifier->show();
    }
}

void CreateComponent::on_buttonCreate_clicked() {
    std::string label = m_ui->lineLabel->text().toStdString();
    
    if (m_modifier != nullptr) {
        OpenCEmulator::get().addComponent(m_modifier->create(label));
    } else {
        const QString type = m_ui->comboTypes->currentText();
        
        if (type == "gpu") {
            OpenCEmulator::get().addComponent(ComponentPtr(new ComponentGPU(std::string(), label)));
        } else if (type == "screen") {
            OpenCEmulator::get().addComponent(ComponentPtr(new ComponentScreen(std::string(), label)));
        } else if (type == "filesystem") {
            OpenCEmulator::get().addComponent(ComponentPtr(new ComponentRealFilesystem(std::string(), label)));
        } else if (type == "eeprom") {
            OpenCEmulator::get().addComponent(ComponentPtr(new ComponentEEPROM(std::string(), label)));
        }
    }
    
    hide();
}
