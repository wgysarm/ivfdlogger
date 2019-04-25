#ifndef IVFDLOGGER_H
#define IVFDLOGGER_H

#include "ivfdlogger_global.h"
#include <QFile>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QMap>
#include <QThread>
#include <QMutex>
#include <QDir>

namespace IvfdLogNameSpace{

    class IVFDLOGGERSHARED_EXPORT IvfdLogger
	{
	public:

		IvfdLogger();
		~IvfdLogger();

	private:

	};
/**
     * @brief The LogLevel enum define the level of the log message.
     */
    enum IVFDLOGGERSHARED_EXPORT LogLevel { TraceLevel = 0, DebugLevel, InfoLevel, WarnLevel, ErrorLevel, FatalLevel ,UnkownLevel};

    /**
     * @brief Here is done the call to write the message in the module. First of all is confirmed
     * that the log level we want to write is less or equal to the level defined when we create the
     * destination.
     *
     * @param module The module that the message references.
     * @param level The level of the message.
     * @param message The message.
     */
    void IVFDLOGGERSHARED_EXPORT QLog_(const QString &module, LogLevel level, const QString &message);
    /**
     * @brief Used to store Trace level messages.
     * @param module The module that the message references.
     * @param message The message.
     */
    void IVFDLOGGERSHARED_EXPORT QLog_Trace(const QString &module, const QString &message);
    /**
     * @brief Used to store Debug level messages.
     * @param module The module that the message references.
     * @param message The message.
     */
    void IVFDLOGGERSHARED_EXPORT QLog_Debug(const QString &module, const QString &message);
    /**
     * @brief Used to store Info level messages.
     * @param module The module that the message references.
     * @param message The message.
     */
    void IVFDLOGGERSHARED_EXPORT QLog_Info(const QString &module, const QString &message);
    /**
     * @brief Used to store Warning level messages.
     * @param module The module that the message references.
     * @param message The message.
     */
    void IVFDLOGGERSHARED_EXPORT QLog_Warning(const QString &module, const QString &message);
    /**
     * @brief Used to store Error level messages.
     * @param module The module that the message references.
     * @param message The message.
     */
    void IVFDLOGGERSHARED_EXPORT QLog_Error(const QString &module, const QString &message);
    /**
     * @brief Used to store Fatal level messages.
     * @param module The module that the message references.
     * @param message The message.
     */
    void IVFDLOGGERSHARED_EXPORT QLog_Fatal(const QString &module, const QString &message);


	/************************************************************************/
	/* 调用此函数即可创建打印日志线程自动创建文件、对象，按模块打印         */
	/************************************************************************/
    void IVFDLOGGERSHARED_EXPORT QLog_(const QString &module, LogLevel level, const QString &message);
    /**
     * @brief The QLoggerWriter class writes the message and manages the file where it is printed.
     */
    class IVFDLOGGERSHARED_EXPORT QLogger : public QObject
	{
		Q_OBJECT

		public:
            /**
             * @brief Constructor that gets the complete path and filename to create the file. It also
             * configures the level of this file to filter the logs depending on the level.
             * @param fileDestination The complete path.
             * @param level The maximum level that is allowed.
             */
            explicit QLogger(const QString &fileDestination, LogLevel level,QObject *parent=0);
            explicit QLogger(LogLevel level,QObject *parent=0);
            /**
             * @brief Gets the current maximum level.
             * @return The LogLevel.
             */
            LogLevel getLevel() const { return m_level; }
            /**
             * @brief Within this method the message is written in the log file. If it would exceed
             * from 1 MByte, another file will be created and the log message will be stored in the
             * new one. The older file will be renamed with the date and time of this message to know
             * where it is updated.
             *
             * @param module The module that corresponds to the message.
             * @param message The message log.
             */
            bool write(const QString &module, const QString &message);
            bool write(const QString &i_module,const LogLevel i_level, const QString &i_message);
            /***************************************************************************
            *Input:null
            *Output:o_message
            *Return:void
            *Description: get logged messages in module log file
            ***************************************************************************/
            bool read(QString &o_message);
            bool read(QStringList &o_message);
        private:
            /**
             * @brief Path and name of the file that will store the logs.
             */
            QString m_fileDestination;
            /**
             * @brief Maximum log level allowed for the file.
             */
            LogLevel m_level;
    };

    /**
     * @brief The QLoggerManager class manages the different destination files that we would like to have.
     */
    class IVFDLOGGERSHARED_EXPORT QLoggerManager : public QThread
	{
        public:
            /**
             * @brief Gets an instance to the QLoggerManager.
             * @return A pointer to the instance.
             */
            static QLoggerManager * getInstance();
            /**
             * @brief Converts the given level in a QString.
             * @param level The log level in LogLevel format.
             * @return The string with the name of the log level.
             */
            static QString levelToText(const LogLevel &level);
            /**
             * @brief This method creates a QLoogerWriter that stores the name of the file and the log
             * level assigned to it. Here is added to the map the different modules assigned to each
             * log file. The method returns <em>false</em> if a module is configured to be stored in
             * more than one file.
             *
             * @param fileDest The file name and path to print logs.
             * @param modules The modules that will be stored in the file.
             * @param level The maximum level allowed.
             * @return Returns true if any error have been done.
             */
            bool addLogInstance(const QString &fileDest, const QStringList &modules, LogLevel level);
            bool addLogInstance(const QString &fileDest, const QString &module, LogLevel level);
            bool addLogInstance(const QString &module,LogLevel level);
            /**
             * @brief Gets the QLoggerWriter instance corresponding to the module <em>module</em>.
             * @param module The module we look for.
             * @return Returns a pointer to the object.
             */
            QLogger * getLogInstance(const QString &module) { return moduleDest.value(module); }
            /**
             * @brief This method closes the logger and the thread it represents.
             */
            void closeLogger();
            /**
             * @brief Mutex to make the method thread-safe.
             */
            QMutex mutex;

        private:
            /**
             * @brief Instance of the class.
             */
            static QLoggerManager *INSTANCE;
            /**
             * @brief Map that stores the module and the file it is assigned.
             */
            QMap<QString,QLogger*> moduleDest;
            /**
             * @brief Default builder of the class. It starts the thread.
             */
            QLoggerManager();
    };

}






#endif // IVFDLOGGER_H
