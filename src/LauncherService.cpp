#include "LauncherService.h"
#include <QProcess>
#include <QDebug>
#include <QFileInfo>

LauncherService::LauncherService(QObject *parent)
    : QObject(parent)
{
    m_vscodeCommand = findVSCodeCommand();
}

LauncherService::~LauncherService()
{
}

QString LauncherService::findVSCodeCommand() const
{
    // Try common VS Code command variations
    QStringList commands = {"code", "code-oss", "codium", "vscode"};
    
    for (const QString& cmd : commands) {
        QProcess process;
        process.start("which", QStringList() << cmd);
        process.waitForFinished();
        
        if (process.exitCode() == 0) {
            QString result = process.readAllStandardOutput().trimmed();
            if (!result.isEmpty()) {
                qDebug() << "Found VS Code command:" << cmd;
                return cmd;
            }
        }
    }
    
    qWarning() << "VS Code command not found, using 'code' as default";
    return "code";
}

void LauncherService::setVSCodeCommand(const QString &command)
{
    m_vscodeCommand = command;
    qDebug() << "VS Code command set to:" << m_vscodeCommand;
}

void LauncherService::openProject(const QString &projectPath)
{
    if (projectPath.isEmpty()) {
        emit errorOccurred("Project path is empty");
        return;
    }

    QFileInfo fileInfo(projectPath);
    if (!fileInfo.exists()) {
        emit errorOccurred("Project path does not exist: " + projectPath);
        qWarning() << "Project path does not exist:" << projectPath;
        return;
    }

    qDebug() << "Opening project:" << projectPath;

    QProcess *process = new QProcess(this);
    
    // Connect to handle completion
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process, projectPath](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
            qDebug() << "Successfully opened project:" << projectPath;
            emit projectOpened(projectPath);
        } else {
            QString error = QString("Failed to open project (exit code: %1)").arg(exitCode);
            qWarning() << error;
            emit errorOccurred(error);
        }
        process->deleteLater();
    });

    // Start VS Code with the project path
    process->start(m_vscodeCommand, QStringList() << projectPath);
    
    if (!process->waitForStarted(3000)) {
        emit errorOccurred("Failed to start VS Code: " + m_vscodeCommand);
        qWarning() << "Failed to start VS Code:" << m_vscodeCommand;
        process->deleteLater();
    }
}
