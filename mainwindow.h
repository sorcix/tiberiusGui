#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>

#include <project.h>
#include <toolchain.h>

#define TIBERIUS_GUI_VERSION "0.1.0"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void on_actionAbout_triggered();
		void on_actionQuit_triggered();
		void on_actionOpen_triggered();
		void on_actionCreate_triggered();
		void on_actionServe_triggered();
		void on_actionCancel_triggered();
		void on_actionWebsite_triggered();
		void on_actionDetails_toggled(bool enabled);
		void on_actionBrowse_triggered();
		void project_updated();
		void toolchain_available(QString version);
		void toolchain_started();
		void toolchain_hasOutput();
		void toolchain_finished(bool error, Toolchain::State state);
		void toolchain_error();

		void on_actionBuild_triggered();

	private:
		Project * project;
		Toolchain * toolchain;
		Ui::MainWindow *ui;
		QString selectFolder();
		void updateToolbar();
};

#endif // MAINWINDOW_H
