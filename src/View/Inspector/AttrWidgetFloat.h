#ifndef INSPECTORFLOATSLOTWIDGET_H
#define INSPECTORFLOATSLOTWIDGET_H

#include <QTimer>
#include <QLineEdit>
#include <QFocusEvent>
#include <QDoubleSpinBox>

#include "AttributeWidget.h"

class XMLAttribute;
class InspectorWidget;
class FloatComponentSlotSpinBox;
class AttrWidgetFloat : public AttributeWidget //Slot for a float (label + float)
{
    Q_OBJECT

private:
    FloatComponentSlotSpinBox *m_lineEdit = nullptr;
    bool _editing = false;

public:

    // isSubWidget indicates if it belongs to another parent attributeWidget,
    // such as AttrWidgetFloat's inside AttrWidgetVectorFloat
    AttrWidgetFloat(const XMLAttribute &xmlAttribute,
                    InspectorWidget *m_inspectorWidget,
                    bool isSubWidget = false);

    virtual void SetValue(float f);
    virtual float GetValue();

    virtual void Refresh(const XMLAttribute &attribute) override;

    virtual void OnLineEditFocusIn();
    virtual void OnLineEditFocusOut();
};

class FloatComponentSlotSpinBox : public QLineEdit //Slot for a float (label + float)
{
    Q_OBJECT

public:
    FloatComponentSlotSpinBox();

    virtual void focusInEvent(QFocusEvent * event) override;
    virtual void focusOutEvent(QFocusEvent * event) override;
    virtual void  keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event);

    void SetFloat(float f);
    float GetFloat() const;

public slots:
    void AdjustStep();
    void SelectAll();
};

#endif // INSPECTORFLOATSLOTWIDGET_H
