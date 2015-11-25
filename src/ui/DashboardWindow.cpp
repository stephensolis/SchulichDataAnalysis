#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QDesktopWidget>
#include <QRect>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QtGlobal>

#include "FileInputDialog.h"
#include "DashboardWindow.h"
#include "PresentationDashboardWindow.h"
#include "TeachingDashboardWindow.h"
#include "GrantDashboardWindow.h"
#include "PublicationDashboardWindow.h"
#include "ui_DashboardWindow.h"

DashboardWindow::DashboardWindow() {
    ui.setupUi(this);
	
	//set size to 70% of total screen size
    QDesktopWidget dw;
    QRect mainScreen = dw.availableGeometry(dw.primaryScreen());
    this->resize(mainScreen.width()*0.7, mainScreen.height()*0.7);
}

DashboardWindow *DashboardWindow::makeDashboard() {
	FileInputDialog inputDialog;
	if (inputDialog.exec() == QDialog::Accepted) {
		if (inputDialog.getSubjectArea() == Presentation) {
			return new PresentationDashboardWindow(inputDialog.getFilename());
        }
        else if (inputDialog.getSubjectArea() == Teaching) {
            return new TeachingDashboardWindow(inputDialog.getFilename());
        }
        else if (inputDialog.getSubjectArea() == Grants) {
            return new GrantDashboardWindow(inputDialog.getFilename());
        }
        else if (inputDialog.getSubjectArea() == Publications) {
            return new PublicationDashboardWindow(inputDialog.getFilename());
        }
        else {
			throw "Unimplemented subject area";
		}
	} else {
		return nullptr;
	}
}

void DashboardWindow::on_dateFilterButton_clicked() {
	updateDateLabel();
	updateTreeWidget();
}

void DashboardWindow::on_actionOpen_triggered() {
    close();
	
    DashboardWindow *dashboard = DashboardWindow::makeDashboard();
    if (dashboard != nullptr)
        dashboard->show();

}

void DashboardWindow::updateDateLabel() {
	ui.dateRangeLabel->setText("Showing records from " + 
							   ui.startDateSelector->date().toString("MMM d yyyy") + 
							   " to " + 
							   ui.endDateSelector->date().toString("MMM d yyyy"));
}

void DashboardWindow::on_actionExit_triggered() {
	close();
}

void DashboardWindow::setColumnWidths() {
    // for now, make sure that column width is at least equal to contents
    for (int i = 0; i < ui.treeWidget->columnCount(); i++) {
        ui.treeWidget->resizeColumnToContents(i);
    }
}

void DashboardWindow::on_treeWidget_collapsed() {
    setColumnWidths();
}

void DashboardWindow::on_treeWidget_expanded() {
    setColumnWidths();
}

void DashboardWindow::on_actionPrint_triggered() {
    // set up the printer
    QPrinter printer;

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Document"));

    if (dialog.exec() != QDialog::Accepted)
        return;

    // actual printing starts here
    QTextDocument doc;
	QTextCursor cursor(&doc);
	
	QTextTable *table = cursor.insertTable(1, ui.treeWidget->columnCount());
	
	//header
	for (int colNum = 0; colNum < ui.treeWidget->columnCount(); ++colNum)
		table->cellAt(0, colNum).firstCursorPosition().insertText(ui.treeWidget->headerItem()->text(colNum));
	
	//table content
	int rowNum = 1;
    for (QTreeWidgetItemIterator it(ui.treeWidget); *it; ++it) {
		table->appendRows(1);
		for (int colNum = 0; colNum < ui.treeWidget->columnCount(); ++colNum)
			table->cellAt(rowNum, colNum).firstCursorPosition().insertText((*it)->text(colNum));
		++rowNum;
    }
	
	
	doc.print(&printer);
}
