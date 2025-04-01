#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QObject>

#ifndef DYNAMIC_HODLER_WIDGET_H
#define DYNAMIC_HODLER_WIDGET_H

class ClickableGroupBox : public QGroupBox
{
    Q_OBJECT  //

signals:
    void clicked();

public:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
};

class dynamicWidget : public QWidget
{
    Q_OBJECT  // Add the Q_OBJECT macro if you want to use signals and slots

public:
    dynamicWidget(QWidget* parent = 0);

public:
    //Add widgets to the dyanamic widget in the order. First widget will be added as the main widget
    //@param widget - main widget to be added
    void addWidget(QWidget* widget);

    //Remove sub widgets from the layout and keep aside
    //@param widget - widget to be removed
    void removeWidget(QWidget* widget);

private:
    //Add sub widget to the vertical layout
    //@param widget - sub widget to be added
    //@param rowNum - row number of the sub widget
    void addSubWidget(QWidget* widget);

    //Add main widget to the vertical layout, only once widget is allowed
    //@param widget - main widget to be added
    void addMainWidget(QWidget* widget);

private slots:
    void onSubWidgetSelect();

private:
    QHBoxLayout* hBoxLayout;//Horizontal box layout containing all the widgets
    QVBoxLayout* vBoxLayoutSubOut;//Outer vertical box layout for sub widgets
    QVBoxLayout* vBoxLayoutMainOut;//Outer vertical box layout for the main widget
    QVBoxLayout* vBoxLayoutMain;//Vertical box layout for the main widget

    QSplitter* splitter;

    QWidget* subWidget;
    QWidget* mainWidget;

    ClickableGroupBox* groupBoxMain;//Main widget group box

    int widgetID = 0;

    QList<QWidget*> orphaWidgets;//Strore temporary removed widgets
};
#endif
