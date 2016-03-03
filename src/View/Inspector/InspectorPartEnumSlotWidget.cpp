#include "InspectorPartEnumSlotWidget.h"

InspectorPartEnumSlotWidget::InspectorPartEnumSlotWidget(const std::string &labelString,
                                                         const std::vector<std::string> &enumValues,
                                                         const int selectedValue,
                                                         InspectorPartWidget *parent) : InspectorPartSlotWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(0); layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    if(labelString != "")
    {
        QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
        textLabel->setContentsMargins(0,0,0,0);
        textLabel->show();
        layout->addWidget(textLabel);
    }

    comboBox = new QComboBox();
    for(std::string enumString : enumValues)
    {
        comboBox->addItem( QString::fromStdString(enumString) );
    }
    comboBox->setCurrentIndex(selectedValue);
    comboBox->show();
    connect(comboBox, SIGNAL(currentIndexChanged(QString)), parent, SLOT(_NotifyInspectorSlotChanged(QString)));


    layout->addWidget(comboBox);

    this->setContentsMargins(0,0,0,0);
    this->show();
}

void InspectorPartEnumSlotWidget::SetValue(int index)
{
    comboBox->setCurrentIndex(index);
}

int InspectorPartEnumSlotWidget::GetValue()
{
    return comboBox->currentIndex();
}

