#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

	// Initialize user interface
	this->ui->setupUi(this);

	// Tiberius project support & toolchain
	this->project = new Project();
	this->toolchain = new Toolchain();

	// Listen for project info updates
	MainWindow::connect(this->project, &Project::updated, this, &MainWindow::project_updated);

	// Listen for toolchain events
	MainWindow::connect(this->toolchain, &Toolchain::available, this, &MainWindow::toolchain_available);
	MainWindow::connect(this->toolchain, &Toolchain::started, this, &MainWindow::toolchain_started);
	MainWindow::connect(this->toolchain, &Toolchain::finished, this, &MainWindow::toolchain_finished);
	MainWindow::connect(this->toolchain, &Toolchain::hasOutput, this, &MainWindow::toolchain_hasOutput);
	MainWindow::connect(this->toolchain, &Toolchain::error, this, &MainWindow::toolchain_error);

}

MainWindow::~MainWindow() {
	delete this->ui;
	delete this->project;
	delete this->toolchain;
}

void MainWindow::updateToolbar() {

	// Toolbar state is based on toolchain availability and project validity.
	bool valid = this->project->isValid();
	bool ready = this->toolchain->isReady();

	this->ui->actionBrowse->setEnabled(valid);
	this->ui->actionBuild->setEnabled(valid && ready);
	this->ui->actionServe->setEnabled(valid && ready);
	this->ui->actionCancel->setEnabled(valid && !ready);
	this->ui->actionClose->setEnabled(valid);
	this->ui->actionCreate->setEnabled(ready);
	this->ui->actionOpen->setEnabled(ready);
}

QString MainWindow::selectFolder() {

	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly,true);
	dialog.setViewMode(QFileDialog::Detail);

	if(dialog.exec()) {
		QStringList selected = dialog.selectedFiles();

		if(selected.count() > 0) {
			return selected.first();
		}
	}

	return NULL;

}

void MainWindow::on_actionAbout_triggered() {

	QString about, version;

	about = "Tiberius Gui v"+QString(TIBERIUS_GUI_VERSION)+"\nCopyright 2013 Vic Demuzere\n\n";
	about += "Licensed under the The MIT License (MIT)\nhttps://github.com/sorcix/tiberiusGui\n\n";

	version = this->toolchain->versionString();

	if(!version.isNull() && !version.isEmpty()) {
		about += "Toolchain:\n"+version+"\n\n";
	}

	about += "Icons by icons8.com.";

	QMessageBox::about(this,this->windowTitle(), about);
}

void MainWindow::on_actionQuit_triggered() {
	qApp->quit();
}

void MainWindow::on_actionOpen_triggered() {

	// Ask the user to choose the project folder.
	QString folder = this->selectFolder();

	// Nothing to do if the user didn't select a folder.
	if(folder == NULL) {
		return;
	}

	// Set the project folder.
	if(!this->project->setPath(folder)) {
		QMessageBox::warning(this,this->windowTitle(),"Please select a Tiberius project folder!");
	}

	this->ui->labelStatus->setText("Ready!");
	this->updateToolbar();

}

void MainWindow::on_actionCreate_triggered() {

	if(!this->toolchain->isReady()) {
		return;
	}

	QString folder = this->selectFolder();

	// Nothing to do if the user didn't select a folder.
	if(folder == NULL) {
		return;
	}

	// Set the project folder.
	if(!this->project->setPath(folder, true)) {
		QMessageBox::warning(this,this->windowTitle(),"Please select an empty folder!");
	}

	this->ui->labelStatus->setText("Running tiberius init..");
	this->toolchain->init(this->project);
}

void MainWindow::project_updated() {
	this->ui->textTitle->setText(this->project->title());
	this->ui->textWebmaster->setText(this->project->webmaster());
	this->ui->textAddress->setText(this->project->address());
	this->ui->textPath->setText(this->project->path());
}

void MainWindow::toolchain_available(QString version) {
	this->ui->progressBar->setMaximum(100);
	this->ui->labelStatus->setText("Found "+version);
	this->updateToolbar();
}

void MainWindow::toolchain_started() {
	this->updateToolbar();
	this->ui->progressBar->setMaximum(0);
}

void MainWindow::toolchain_hasOutput() {
	this->ui->textStatus->appendPlainText(this->toolchain->readOutput());
}

void MainWindow::toolchain_finished(bool error, Toolchain::State state) {

	this->ui->progressBar->setMaximum(100);

	if(error) {
		this->ui->labelStatus->setText("Tiberius finished with an error..");
	} else {
		this->ui->labelStatus->setText("Ready!");
	}

	if(!this->project->isValid() && state == Toolchain::StateInit) {

		// Read configuration file!
		this->project->setPath(this->project->path());
	}

	this->updateToolbar();
}

void MainWindow::toolchain_error() {
	QMessageBox::critical(this, this->windowTitle(), "I can't find tiberius. Is it installed and available in your PATH?");
	qApp->exit();
}

void MainWindow::on_actionServe_triggered() {
	if(this->project->isValid() && this->toolchain->isReady()) {
		this->ui->labelStatus->setText("Running tiberius serve..");
		this->toolchain->serve(this->project);
	}
}

void MainWindow::on_actionCancel_triggered() {
	this->toolchain->stop();
}

void MainWindow::on_actionWebsite_triggered() {
	QDesktopServices::openUrl(QUrl("http://vic.demuzere.be/projects/tiberius/manual/"));
}

void MainWindow::on_actionDetails_toggled(bool enabled) {
	this->ui->groupProject->setVisible(enabled);
}

void MainWindow::on_actionBrowse_triggered() {
	QDesktopServices::openUrl(QUrl("file:///" + this->project->path()));
}

void MainWindow::on_actionBuild_triggered() {
	if(this->project->isValid() && this->toolchain->isReady()) {
		this->ui->labelStatus->setText("Running tiberius build..");
		this->toolchain->build(this->project);
	}
}
