#include "dyanmicWidget.h"

void ClickableGroupBox::mouseDoubleClickEvent(QMouseEvent* event)
{
    QGroupBox::mouseDoubleClickEvent(event);
    emit clicked();
}

dynamicWidget::dynamicWidget(QWidget * parent)
{
    hBoxLayout = new QHBoxLayout;
    vBoxLayoutSubOut = new QVBoxLayout;
    vBoxLayoutMainOut = new QVBoxLayout;

    splitter = new QSplitter(Qt::Horizontal, this);

    // Create containers for the sub and main layouts
    QWidget* subWidgetContainer = new QWidget;
    QWidget* mainWidgetContainer = new QWidget;

    subWidgetContainer->setLayout(vBoxLayoutSubOut);
    mainWidgetContainer->setLayout(vBoxLayoutMainOut);

    // Add the containers to the splitter
    splitter->addWidget(subWidgetContainer);
    splitter->addWidget(mainWidgetContainer);

    // Set the stretch factors to control the initial sizes
    splitter->setStretchFactor(0, 1); // Sub widget container
    splitter->setStretchFactor(1, 2); // Main widget container

    // Add the splitter to the main layout
    hBoxLayout->addWidget(splitter);
}

void dynamicWidget::onSubWidgetSelect()
{
    //Get selected group box and layout
    QGroupBox* gBoxSelected = qobject_cast<QGroupBox*>(sender());
    QLayout* layoutSelected = qobject_cast<QVBoxLayout*>(gBoxSelected->layout());

    auto getQWidget = [](const QObjectList& list)
    {
        QWidget* widget = nullptr;
        foreach(QObject * var, list)
            widget = dynamic_cast<QWidget*>(var);
        return widget;
    };

    //Get selected widget and main widget
    QWidget* widgetSelected = getQWidget(gBoxSelected->children());
    QWidget* widgetMain = getQWidget(groupBoxMain->children());

    //Remove widget selected and main widget
    layoutSelected->removeWidget(widgetSelected);
    vBoxLayoutMain->removeWidget(widgetMain);

    //Swap selected widget and main widget
    layoutSelected->addWidget(widgetMain);
    gBoxSelected->setLayout(layoutSelected);
    vBoxLayoutMain->addWidget(widgetSelected);
    groupBoxMain->setLayout(vBoxLayoutMain);
}

void dynamicWidget::addWidget(QWidget* widget)
{
    if (widgetID == 0)
        addMainWidget(widget);
    else
        addSubWidget(widget);

    widgetID++;
}

void dynamicWidget::addSubWidget(QWidget* widget)
{
    ClickableGroupBox *groupBox = new ClickableGroupBox;

    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    vBoxLayout->addWidget(widget);
    groupBox->setLayout(vBoxLayout);
    vBoxLayoutSubOut->addWidget(groupBox);

    connect(groupBox, SIGNAL(clicked()), this, SLOT(onSubWidgetSelect()));
}

void dynamicWidget::addMainWidget(QWidget* widget)
{
    groupBoxMain = new ClickableGroupBox;

    vBoxLayoutMain = new QVBoxLayout;
    widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    vBoxLayoutMain->addWidget(widget);
    groupBoxMain->setLayout(vBoxLayoutMain);
    vBoxLayoutMainOut->addWidget(groupBoxMain);

    hBoxLayout->addLayout(vBoxLayoutSubOut, 1);
    hBoxLayout->addLayout(vBoxLayoutMainOut, 2);

    this->setLayout(hBoxLayout);
}

void dynamicWidget::removeWidget(QWidget* widget)
{
    bool isMainWidgetRemoved = false;
    QLayoutItem* mainItem = vBoxLayoutMainOut->takeAt(0);
    if (mainItem)
    {
        QGroupBox* mainGroupBox = qobject_cast<QGroupBox*>(mainItem->widget());
        if (mainGroupBox)
        {
            QWidget* mainWidget = mainGroupBox->findChild<QWidget*>(); // Get the inner widget
            mainWidget->setParent(nullptr);
            if (mainWidget && mainWidget == widget)
            {
                isMainWidgetRemoved = true;
            }
            else
            {
                addMainWidget(mainWidget);
            }
            delete mainGroupBox; // Delete the group box container
        }
        delete mainItem; // Clean up layout item
    }

    if (vBoxLayoutSubOut->count() <= 0)  // if there are no sub widgets to remove
    {
        if (isMainWidgetRemoved) // if the main widget is removed, decrement the widgetID else do nothing
            widgetID--;
        return;
    }

    QList<QWidget*> widgetList;  // Store widgets before clearing
    //Collect all widgets inside vBoxLayoutSubOut
    while (vBoxLayoutSubOut->count() > 0)
    {
        QLayoutItem* item = vBoxLayoutSubOut->takeAt(0);
        if (!item) continue;

        QGroupBox* groupBox = qobject_cast<QGroupBox*>(item->widget());
        if (groupBox)
        {
            QWidget* widget = groupBox->findChild<QWidget*>(); // Get the inner widget
            if (widget)
            {
                widget->setParent(nullptr); // Detach but keep it
                widgetList.append(widget);
            }
            delete groupBox; // Delete the group box container
        }
        delete item; // Clean up layout item
    }

    if (isMainWidgetRemoved && !widgetList.isEmpty()) // if the main widget is removed, add the first sub widget to the main widget
    {
        addMainWidget(widgetList[0]);
    }

    for (int i; i < widgetList.size(); ++i)
    {
        if (widget != widgetList[i])
            addSubWidget(widgetList[i]);
    }

    if (widgetList.size() > vBoxLayoutSubOut->count()) // check if any widget is removed, if so decrement the widgetID
        widgetID--;

    //Add the widget to the orphan list
    orphaWidgets.clear();
    orphaWidgets.append(widget);
}
