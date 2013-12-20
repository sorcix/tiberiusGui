#include "project.h"

Project::Project(QObject *parent) : QObject(parent) {
	this->reset();
}

Project::~Project() {

}

bool Project::reset() {
	this->projectTitle = QString();
	this->projectAddress = QString();
	this->projectWebmaster = QString();
	this->projectPath = QString();
	this->projectValid = false;

	emit this->updated();

	return false;
}

bool Project::readConfig() {

	QFile file;
	file.setFileName(this->projectPath+"/tiberius.cfg");
	file.open(QIODevice::ReadOnly | QIODevice::Text);

	QString data = file.readAll();

	QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());

	QJsonObject documentObject = document.object();

	// We're reading a Tiberius 0.2 style configuration file!
	QJsonValue site = documentObject.value("Site");

	if(site.isObject()) {
		QJsonObject object = site.toObject();

		QJsonValue name = object.value("Name");
		QJsonValue address = object.value("Address");
		QJsonValue webmaster = object.value("Webmaster");

		if(name.isString() && address.isString() && webmaster.isString()) {
			this->projectTitle = name.toString();
			this->projectAddress = address.toString();
			this->projectWebmaster = webmaster.toString();
			this->projectValid = true;

			emit this->updated();

			return true;
		}

	}

	this->projectValid = false;

	return false;
}

QString Project::title() {
	return this->projectTitle;
}

QString Project::webmaster() {
	return this->projectWebmaster;
}

QString Project::address() {
	return this->projectAddress;
}

QString Project::path() {
	return this->projectPath;
}

bool Project::setPath(QString path, bool empty) {

	// We're not doing anything if the path is empty.
	if(path.isNull() || path.isEmpty()) {
		return this->reset();
	}

	QDir directory = QDir(path);

	// Check if the given path is an existing directory.
	if(!directory.exists() || !directory.isReadable()) {
		return this->reset();
	}

	if (!empty) {

		// Attempt to convert to an absolute path to prevent issues when passing it to the tiberius binary.
		directory.makeAbsolute();

		// Fast check if the path is a possible tiberius directory.
		if(directory.exists("tiberius.cfg") && directory.exists("content") && directory.exists("templates") && directory.exists("public")) {
			this->projectPath = path;
			return this->readConfig();
		}

	} else {

		if(directory.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0) {
			this->projectPath = path;
			return true;
		}
	}

	return false;
}

bool Project::isValid() {
	return this->projectValid;
}
