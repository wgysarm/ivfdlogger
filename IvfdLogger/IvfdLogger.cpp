#include "IvfdLogger.h"
#include <QDir>
#include <QDateTime>
#include <QTextStream>

namespace IvfdLogNameSpace{

	IvfdLogger::IvfdLogger()
	{
        QLoggerManager::getInstance();
	}

	IvfdLogger::~IvfdLogger()
	{

	}
	void QLog_Trace(const QString &module, const QString &message)
	{
		QLog_(module, TraceLevel, message);
	}

	void QLog_Debug(const QString &module, const QString &message)
	{
		QLog_(module, DebugLevel, message);
	}

	void QLog_Info(const QString &module, const QString &message)
	{
		QLog_(module, InfoLevel, message);
	}

	void QLog_Warning(const QString &module, const QString &message)
	{
		QLog_(module, WarnLevel, message);
	}

	void QLog_Error(const QString &module, const QString &message)
	{
		QLog_(module, ErrorLevel, message);
	}

	void QLog_Fatal(const QString &module, const QString &message)
	{
		QLog_(module, FatalLevel, message);
	}

	void QLog_(const QString &module, LogLevel level, const QString &message)
	{
		QLoggerManager *manager = QLoggerManager::getInstance();

		manager->addLogInstance(module,level);//这句话是原来没有的，必须在这里对文件对象进行检查，看看是否有MODULE存在，如果module返回空，就无法write

		QMutexLocker(&manager->mutex);

		QLogger *logWriter = manager->getLogInstance(module);

		if (logWriter && logWriter->getLevel() <= level)
			logWriter->write(module,level,message);
	}

	//QLoggerManager
	QLoggerManager * QLoggerManager::INSTANCE = NULL;

	QLoggerManager::QLoggerManager() : QThread(), mutex(QMutex::Recursive)
	{
		start();
	}

	QLoggerManager * QLoggerManager::getInstance()
	{
		if (!INSTANCE)
			INSTANCE = new QLoggerManager();

		return INSTANCE;
	}

	QString QLoggerManager::levelToText(const LogLevel &level)
	{
		switch (level)
		{
		case TraceLevel: return "Trace log";
		case DebugLevel: return "Debug log";
		case InfoLevel: return "Info log";
		case WarnLevel: return "Warning log";
		case ErrorLevel: return "Error log";
		case FatalLevel: return "Fatal log";
		default: return QString();
		}
	}

	bool QLoggerManager::addLogInstance(const QString &fileDest, const QString &module, LogLevel level)
	{
		QLogger *log;

		if (!moduleDest.contains(module))
		{
			log = new QLogger(fileDest,level);
			moduleDest.insert(module, log);
			return true;
		}

		return false;
	}

	bool QLoggerManager::addLogInstance(const QString &fileDest, const QStringList &modules, LogLevel level)
	{
		QLogger *log;
		foreach (QString module, modules)
		{
			if (!moduleDest.contains(module))
			{
				log = new QLogger(fileDest,level);
				moduleDest.insert(module, log);
				return true;
			}
		}
		return false;
	}

	bool QLoggerManager::addLogInstance(const QString &module,LogLevel level)
	{
		QLogger *log;
		if(!moduleDest.contains(module))
		{
			QDir dir(QDir::currentPath());
			if (!dir.exists("logs"))
				dir.mkdir("logs");

			QString fileDestination = QDir::currentPath() + "/logs/"+ module +".log";
			log = new QLogger(fileDestination,level);
			moduleDest.insert(module,log);
			return true;
		}
		return false;
	}

	void QLoggerManager::closeLogger()
	{
		exit(0);
		deleteLater();
	}

	QLogger::QLogger(const QString &fileDestination, LogLevel level,QObject *parent)
		:QObject(parent)
	{
		m_fileDestination = fileDestination;
		m_level = level;
	}

	QLogger::QLogger(LogLevel level,QObject *parent)
		:QObject(parent)
	{
		m_level = level;
		QDir dir(QDir::currentPath());
		if (!dir.exists("logs"))
			dir.mkdir("logs");

		m_fileDestination = QDir::currentPath() + "/logs/system.log";

	}

	bool QLogger::write(const QString &module,  const QString &message)
	{
		QString _fileName = m_fileDestination;

		int MAX_SIZE = 1024 * 1024;


		QFile file(_fileName);
		QString toRemove = _fileName.section('.',-1);
		QString fileNameAux = _fileName.left(_fileName.size() - toRemove.size()-1);
		bool renamed = false;
		QString newName = fileNameAux + "_%1__%2.log";

		//Renomenem l'
		if (file.size() >= MAX_SIZE)
		{
			//Cree
			QDateTime currentTime = QDateTime::currentDateTime();
			newName = newName.arg(currentTime.date().toString("dd_MM_yy")).arg(currentTime.time().toString("hh_mm_ss"));
			renamed = file.rename(_fileName, newName);

		}

		file.setFileName(_fileName);
		if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
		{
			QTextStream out(&file);
			QString dtFormat = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz");

			if (renamed)
				out << QString("%1 - Previous log %2\n").arg(dtFormat).arg(newName);

			QString logLevel = QLoggerManager::levelToText(m_level);
			QString text = QString("[%1] [%2] {%3} %4\n").arg(dtFormat).arg(logLevel).arg(module).arg(message);
			out << text;
			file.close();
			return true;
		}
		else
		{
			//Log fail
			return false;
		}
	}

	bool QLogger::write(const QString &i_module, const LogLevel i_level, const QString &i_message)
	{
		QString _fileName = m_fileDestination;

		int MAX_SIZE = 1024 * 1024;


		QFile file(_fileName);
		QString toRemove = _fileName.section('.',-1);
		QString fileNameAux = _fileName.left(_fileName.size() - toRemove.size()-1);
		bool renamed = false;
		QString newName = fileNameAux + "_%1__%2.log";

		//Renomenem l'
		if (file.size() >= MAX_SIZE)
		{
			//Creem un fixer nou
			QDateTime currentTime = QDateTime::currentDateTime();
			newName = newName.arg(currentTime.date().toString("dd_MM_yy")).arg(currentTime.time().toString("hh_mm_ss"));
			renamed = file.rename(_fileName, newName);

		}

		file.setFileName(_fileName);
		if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
		{
			QTextStream out(&file);
			QString dtFormat = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz");

			if (renamed)
				out << QString("%1 - Previous log %2\n").arg(dtFormat).arg(newName);

			QString logLevel = QLoggerManager::levelToText(i_level);
			QString text = QString("[%1] [%2] {%3} %4\n").arg(dtFormat).arg(logLevel).arg(i_module).arg(i_message);
			out << text;
			file.close();
			return true;
		}
		else
		{
			//Log fail
			return false;
		}
	}

	bool QLogger::read(QString &o_message)
	{
		o_message="";
		QString _fileName = m_fileDestination;
		QFile file(_fileName);

		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&file);
			while(!in.atEnd()){
				o_message += in.readLine() + "\n";
			}

			file.close();
			return true;
		}
		else//read fail
		{
			return false;
		}
	}

	bool QLogger::read(QStringList &o_message)
	{
		o_message.clear();
		QString _fileName = m_fileDestination;
		QFile file(_fileName);

		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&file);
			while(!in.atEnd()){
				o_message.append(in.readLine() + "\n");
			}

			file.close();
			return true;
		}
		else//read fail
		{
			return false;
		}
	}
}


