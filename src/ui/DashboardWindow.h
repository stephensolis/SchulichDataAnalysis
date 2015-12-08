#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QPrinter>

#include "ui_DashboardWindow.h"

class DashboardWindow : public QMainWindow {
    Q_OBJECT

 public:
	static DashboardWindow *makeDashboard();

 private slots:
	void on_actionOpen_triggered();
	void on_actionExport_triggered();
	void on_actionPrint_triggered();
	void on_actionExit_triggered();
	void on_dateFilterButton_clicked();
    void on_treeWidget_collapsed();
    void on_treeWidget_expanded();

protected slots:
    virtual void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item) = 0;

 protected:
    DashboardWindow();
	void updateDateLabel();
	void setColumnWidths();
	void printTreeWidget(QPrinter *printer);
	
	virtual void updateTreeWidget() = 0;
	
    Ui::DashboardWindow ui;
};

#endif
