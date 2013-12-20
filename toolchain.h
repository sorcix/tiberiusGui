#ifndef TOOLCHAIN_H
#define TOOLCHAIN_H

#include <QObject>
#include <QProcess>
#include <QByteArray>

#include <project.h>

class Toolchain : public QObject {

		Q_OBJECT

	public:
		explicit Toolchain(QObject *parent = 0);
		~Toolchain();
		bool isReady();

	public:
		void init(Project *project);
		void serve(Project *project);
		void build(Project *project);
		void stop();
		QByteArray readOutput();
		QString versionString();

	public:
		enum State {
			StateIdle = 0,
			StateServe = 2,
			StateInit = 1,
			StateBuild = 3,
			StateVersion = 4,
		};

	signals:
		void available(QString version);
		void started();
		void hasOutput();
		void finished(bool error, Toolchain::State state);
		void error();

	private:
		QString toolchainVersion;
		QProcess *process;
		Toolchain::State state;
		bool terminated;
		void version();
		void run(QStringList arguments);

	private slots:
		void process_finished(int exitCode);
		void process_started();
		void process_readyReadStandardOutput();
		void process_error(QProcess::ProcessError error);
};

#endif // TOOLCHAIN_H
