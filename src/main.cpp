#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QMessageBox>
#include "ProjectManager.h"
#include "LauncherService.h"

void buildMenu(QMenu *menu, ProjectManager *projectManager, LauncherService *launcherService);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application metadata
    app.setApplicationName("VS Code Launcher");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("VSCodeLauncher");
    app.setQuitOnLastWindowClosed(false); // Keep running with system tray
    
    // Set application icon
    app.setWindowIcon(QIcon(":/icon.png"));
    
    // Check if system tray is available
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, "VS Code Launcher",
                            "System tray is not available on this system.");
        return 1;
    }
    
    // Create services
    ProjectManager projectManager;
    LauncherService launcherService;
    
    // Create system tray icon
    QSystemTrayIcon trayIcon(QIcon::fromTheme("code", QIcon(":/icon.png")));
    trayIcon.setToolTip("VS Code Launcher");
    
    // Create tray menu
    QMenu *trayMenu = new QMenu();
    buildMenu(trayMenu, &projectManager, &launcherService);
    
    trayIcon.setContextMenu(trayMenu);
    trayIcon.show();
    
    // Rebuild menu when config changes
    QObject::connect(&projectManager, &ProjectManager::groupsChanged, [trayMenu, &projectManager, &launcherService]() {
        trayMenu->clear();
        buildMenu(trayMenu, &projectManager, &launcherService);
    });
    
    // Show notifications
    QObject::connect(&projectManager, &ProjectManager::configReloaded, [&trayIcon]() {
        trayIcon.showMessage("VS Code Launcher", "Configuration reloaded", QSystemTrayIcon::Information, 3000);
    });
    
    QObject::connect(&launcherService, &LauncherService::projectOpened, [&trayIcon](const QString &projectPath) {
        QString projectName = projectPath.split('/').last();
        trayIcon.showMessage("VS Code Launcher", "Opened: " + projectName, QSystemTrayIcon::Information, 3000);
    });
    
    QObject::connect(&launcherService, &LauncherService::errorOccurred, [&trayIcon](const QString &error) {
        trayIcon.showMessage("VS Code Launcher", "Error: " + error, QSystemTrayIcon::Critical, 5000);
    });
    
    return app.exec();
}

void buildMenu(QMenu *menu, ProjectManager *projectManager, LauncherService *launcherService)
{
    // Get groups from ProjectManager
    QVariantList groups = projectManager->groupsAsVariant();
    
    // Add project groups
    for (const QVariant &groupVar : groups) {
        QVariantMap groupMap = groupVar.toMap();
        QString groupName = groupMap["name"].toString();
        QVariantList projects = groupMap["projects"].toList();
        
        if (!projects.isEmpty()) {
            QMenu *groupMenu = menu->addMenu(groupName);
            
            for (const QVariant &projectVar : projects) {
                QVariantMap projectMap = projectVar.toMap();
                QString projectName = projectMap["name"].toString();
                QString projectPath = projectMap["path"].toString();
                
                QAction *projectAction = groupMenu->addAction(projectName);
                QObject::connect(projectAction, &QAction::triggered, [projectPath, launcherService]() {
                    launcherService->openProject(projectPath);
                });
            }
        }
    }
    
    // Add separator
    if (!groups.isEmpty()) {
        menu->addSeparator();
    }
    
    // Add Settings action
    QAction *settingsAction = menu->addAction(QIcon::fromTheme("configure"), "Settings");
    QObject::connect(settingsAction, &QAction::triggered, [projectManager]() {
        projectManager->openConfigInEditor();
    });
    
    // Add Reload action
    QAction *reloadAction = menu->addAction(QIcon::fromTheme("view-refresh"), "Reload Config");
    QObject::connect(reloadAction, &QAction::triggered, [projectManager]() {
        projectManager->loadConfig();
    });
    
    // Add separator
    menu->addSeparator();
    
    // Add Quit action
    QAction *quitAction = menu->addAction(QIcon::fromTheme("application-exit"), "Quit");
    QObject::connect(quitAction, &QAction::triggered, &QApplication::quit);
}
