#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QFileSystemWatcher>

struct Project {
    QString name;
    QString path;
};

struct ProjectGroup {
    QString name;
    QList<Project> projects;
};

class ProjectManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList groups READ groupsAsVariant NOTIFY groupsChanged)
    Q_PROPERTY(QString configPath READ configPath NOTIFY configPathChanged)

public:
    explicit ProjectManager(QObject *parent = nullptr);
    ~ProjectManager();

    Q_INVOKABLE void loadConfig();
    Q_INVOKABLE void openConfigInEditor();
    Q_INVOKABLE QString getProjectPath(const QString &groupName, const QString &projectName);

    QVariantList groupsAsVariant() const;
    QString configPath() const { return m_configPath; }

signals:
    void groupsChanged();
    void configPathChanged();
    void configReloaded();

private slots:
    void onConfigFileChanged(const QString &path);

private:
    void setupConfigWatcher();
    QString getDefaultConfigPath() const;
    QString getEditorCommand() const;

    QList<ProjectGroup> m_groups;
    QString m_configPath;
    QString m_editorCommand;
    QFileSystemWatcher *m_fileWatcher;
    bool m_editorOpen;
};

#endif // PROJECTMANAGER_H
