#ifndef LAUNCHERSERVICE_H
#define LAUNCHERSERVICE_H

#include <QObject>
#include <QString>

class LauncherService : public QObject
{
    Q_OBJECT

public:
    explicit LauncherService(QObject *parent = nullptr);
    ~LauncherService();

    Q_INVOKABLE void openProject(const QString &projectPath);
    Q_INVOKABLE void setVSCodeCommand(const QString &command);

signals:
    void projectOpened(const QString &projectPath);
    void errorOccurred(const QString &error);

private:
    QString m_vscodeCommand;
    QString findVSCodeCommand() const;
};

#endif // LAUNCHERSERVICE_H
