#include "ProjectManager.h"
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QStandardPaths>
#include <QDebug>
#include <QTimer>
#include <yaml-cpp/yaml.h>

ProjectManager::ProjectManager(QObject *parent)
    : QObject(parent)
    , m_fileWatcher(new QFileSystemWatcher(this))
    , m_editorOpen(false)
{
    m_configPath = getDefaultConfigPath();
    setupConfigWatcher();
    loadConfig();
}

ProjectManager::~ProjectManager()
{
}

QString ProjectManager::getDefaultConfigPath() const
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    return configDir + "/vscode-launcher/config.yaml";
}

void ProjectManager::setupConfigWatcher()
{
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &ProjectManager::onConfigFileChanged);
    
    if (QFile::exists(m_configPath)) {
        m_fileWatcher->addPath(m_configPath);
    }
}

void ProjectManager::loadConfig()
{
    m_groups.clear();

    if (!QFile::exists(m_configPath)) {
        qWarning() << "Config file not found:" << m_configPath;
        emit groupsChanged();
        return;
    }

    try {
        YAML::Node config = YAML::LoadFile(m_configPath.toStdString());

        // Load editor command if specified
        if (config["editor_command"]) {
            m_editorCommand = QString::fromStdString(config["editor_command"].as<std::string>());
        }

        // Load groups
        if (config["groups"] && config["groups"].IsSequence()) {
            for (const auto& groupNode : config["groups"]) {
                ProjectGroup group;
                group.name = QString::fromStdString(groupNode["name"].as<std::string>());

                if (groupNode["projects"] && groupNode["projects"].IsSequence()) {
                    for (const auto& projectNode : groupNode["projects"]) {
                        Project project;
                        project.name = QString::fromStdString(projectNode["name"].as<std::string>());
                        project.path = QString::fromStdString(projectNode["path"].as<std::string>());
                        group.projects.append(project);
                    }
                }

                m_groups.append(group);
            }
        }

        qDebug() << "Loaded" << m_groups.size() << "project groups";
        emit groupsChanged();
        emit configReloaded();

    } catch (const YAML::Exception& e) {
        qWarning() << "Failed to parse config:" << e.what();
    }
}

QVariantList ProjectManager::groupsAsVariant() const
{
    QVariantList result;

    for (const auto& group : m_groups) {
        QVariantMap groupMap;
        groupMap["name"] = group.name;

        QVariantList projectsList;
        for (const auto& project : group.projects) {
            QVariantMap projectMap;
            projectMap["name"] = project.name;
            projectMap["path"] = project.path;
            projectsList.append(projectMap);
        }

        groupMap["projects"] = projectsList;
        result.append(groupMap);
    }

    return result;
}

QString ProjectManager::getProjectPath(const QString &groupName, const QString &projectName)
{
    for (const auto& group : m_groups) {
        if (group.name == groupName) {
            for (const auto& project : group.projects) {
                if (project.name == projectName) {
                    return project.path;
                }
            }
        }
    }
    return QString();
}

QString ProjectManager::getEditorCommand() const
{
    if (!m_editorCommand.isEmpty()) {
        return m_editorCommand;
    }
    
    // Default to common text editors
    QStringList editors = {"kate", "gedit", "xdg-open"};
    for (const QString& editor : editors) {
        QProcess process;
        process.start("which", QStringList() << editor);
        process.waitForFinished();
        if (process.exitCode() == 0) {
            return editor;
        }
    }
    
    return "xdg-open"; // Fallback
}

void ProjectManager::openConfigInEditor()
{
    // Ensure config directory exists
    QFileInfo fileInfo(m_configPath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // Create config if it doesn't exist
    if (!QFile::exists(m_configPath)) {
        QFile file(m_configPath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "# VS Code Launcher Configuration\n";
            out << "# editor_command: kate  # Optional: specify your preferred editor\n\n";
            out << "groups:\n";
            out << "  - name: Example Group\n";
            out << "    projects:\n";
            out << "      - name: Example Project\n";
            out << "        path: /path/to/project\n";
            file.close();
        }
    }

    QString editor = getEditorCommand();
    
    m_editorOpen = true;
    
    // Launch editor and wait for it to close
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        Q_UNUSED(exitCode)
        Q_UNUSED(exitStatus)
        m_editorOpen = false;
        
        // Reload config after a short delay to ensure file is written
        QTimer::singleShot(500, this, &ProjectManager::loadConfig);
        
        process->deleteLater();
    });
    
    process->start(editor, QStringList() << m_configPath);
    
    if (!process->waitForStarted()) {
        qWarning() << "Failed to open editor:" << editor;
        m_editorOpen = false;
    }
}

void ProjectManager::onConfigFileChanged(const QString &path)
{
    Q_UNUSED(path)
    
    // Only reload if editor is not open (to avoid reloading while editing)
    if (!m_editorOpen) {
        // Delay reload slightly to ensure file is fully written
        QTimer::singleShot(200, this, &ProjectManager::loadConfig);
    }
    
    // Re-add the file to watcher (some editors remove and recreate files)
    if (!m_fileWatcher->files().contains(m_configPath)) {
        m_fileWatcher->addPath(m_configPath);
    }
}
