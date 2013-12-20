#include "toolchain.h"

Toolchain::Toolchain(QObject *parent) : QObject(parent) {
	this->process = new QProcess(this);

	Toolchain::connect(this->process, &QProcess::started, this, &Toolchain::process_started);
	Toolchain::connect(this->process, &QProcess::readyReadStandardOutput, this, &Toolchain::process_readyReadStandardOutput);
	Toolchain::connect(this->process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(process_error(QProcess::ProcessError)));
	Toolchain::connect(this->process, SIGNAL(finished(int , QProcess::ExitStatus )), this, SLOT(process_finished(int)));

	this->state = Toolchain::StateIdle;
	this->terminated = false;

	this->version();
}

Toolchain::~Toolchain() {

	if(this->process->state() != QProcess::NotRunning) {
		this->process->kill();
		this->process->waitForFinished();
	}

	delete this->process;
}

bool Toolchain::isReady() {
	return (this->process->state() == QProcess::NotRunning && this->state == Toolchain::StateIdle);
}

void Toolchain::version() {

	if(this->state != Toolchain::StateIdle) {
		return;
	}

	this->state = Toolchain::StateVersion;
	this->run(QStringList() << "version");
}

void Toolchain::init(Project *project) {

	if(this->state != Toolchain::StateIdle) {
		return;
	}

	this->state = Toolchain::StateInit;
	this->run(QStringList() << "init" << "-source" << project->path());
}

void Toolchain::serve(Project *project) {

	if(this->state != Toolchain::StateIdle) {
		return;
	}

	this->state = Toolchain::StateServe;
	this->run(QStringList() << "serve" << "-source" << project->path());
}

void Toolchain::build(Project *project) {

	if(this->state != Toolchain::StateIdle) {
		return;
	}

	this->state = Toolchain::StateBuild;
	this->run(QStringList() << "build" << "-source" << project->path());
}

void Toolchain::stop() {
	this->terminated = true;
	this->process->kill();
}

QByteArray Toolchain::readOutput() {
	return this->process->readAllStandardOutput();
}

QString Toolchain::versionString() {
	return this->toolchainVersion;
}

void Toolchain::run(QStringList arguments) {
	if(this->process->state() == QProcess::NotRunning) {
		this->process->start("tiberius",arguments,QProcess::ReadOnly);
	}
}

void Toolchain::process_finished(int exitCode) {
	Toolchain::State oldState = this->state;

	this->state = Toolchain::StateIdle;

	if(oldState == Toolchain::StateVersion) {

		if(exitCode != 0) {
			emit this->error();
		}

		this->toolchainVersion = this->process->readAllStandardOutput();

		if(this->toolchainVersion.startsWith("Tiberius ")) {
			emit this->available(this->toolchainVersion);
			return;
		} else {
			this->toolchainVersion = QString();
			emit this->error();
			return;
		}
	}

	emit this->finished((!this->terminated && exitCode != 0), oldState);

	this->terminated = false;

}

void Toolchain::process_started() {
	if(this->state != Toolchain::StateVersion) {
		emit this->started();
	}
}

void Toolchain::process_readyReadStandardOutput() {
	if(this->state != Toolchain::StateVersion) {
		emit this->hasOutput();
	}
}

void Toolchain::process_error(QProcess::ProcessError error) {
	emit this->error();
}
