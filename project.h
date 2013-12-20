#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

class Project : public QObject {

		Q_OBJECT

		Q_PROPERTY(QString title READ title)
		Q_PROPERTY(QString webmaster READ webmaster)
		Q_PROPERTY(QString address READ address)
		Q_PROPERTY(QString path READ path WRITE setPath)
		Q_PROPERTY(bool valid READ isValid)

	public:
		explicit Project(QObject *parent = 0);
		~Project();
		QString title();
		QString webmaster();
		QString address();
		QString path();
		bool setPath(QString path, bool empty = false);
		bool isValid();

	signals:
		void updated();

	private:
		QString projectTitle;
		QString projectWebmaster;
		QString projectAddress;
		QString projectPath;
		bool projectValid;
		bool reset();
		bool readConfig();

};

#endif // PROJECT_H
