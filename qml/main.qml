import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

Item {
    id: root
    
    SystemTrayIcon {
        id: trayIcon
        visible: true
        icon.source: "qrc:/icon.png"
        tooltip: "VS Code Launcher"
        
        menu: Menu {
            id: trayMenu
            
            // Dynamically generated project groups and items
            Instantiator {
                model: projectManager.groups
                delegate: Menu {
                    id: groupMenu
                    title: modelData.name
                    
                    Instantiator {
                        model: modelData.projects
                        delegate: MenuItem {
                            text: modelData.name
                            onTriggered: {
                                var projectPath = projectManager.getProjectPath(
                                    groupMenu.title,
                                    modelData.name
                                );
                                if (projectPath) {
                                    launcherService.openProject(projectPath);
                                }
                            }
                        }
                        onObjectAdded: (index, object) => groupMenu.insertItem(index, object)
                        onObjectRemoved: (index, object) => groupMenu.removeItem(object)
                    }
                }
                onObjectAdded: (index, object) => trayMenu.insertMenu(index, object)
                onObjectRemoved: (index, object) => trayMenu.removeMenu(object)
            }
            
            MenuSeparator {}
            
            MenuItem {
                text: "Settings"
                icon.name: "configure"
                onTriggered: {
                    projectManager.openConfigInEditor();
                }
            }
            
            MenuItem {
                text: "Reload Config"
                icon.name: "view-refresh"
                onTriggered: {
                    projectManager.loadConfig();
                    notification.show("Configuration reloaded");
                }
            }
            
            MenuSeparator {}
            
            MenuItem {
                text: "Quit"
                icon.name: "application-exit"
                onTriggered: Qt.quit()
            }
        }
        
        onActivated: (reason) => {
            if (reason === SystemTrayIcon.Trigger) {
                trayMenu.open();
            }
        }
    }
    
    // Notification system
    Item {
        id: notification
        
        function show(message) {
            trayIcon.showMessage("VS Code Launcher", message, SystemTrayIcon.Information, 3000);
        }
    }
    
    // Connect to signals
    Connections {
        target: projectManager
        function onConfigReloaded() {
            notification.show("Configuration reloaded");
        }
    }
    
    Connections {
        target: launcherService
        function onProjectOpened(projectPath) {
            notification.show("Opened: " + projectPath.split('/').pop());
        }
        function onErrorOccurred(error) {
            notification.show("Error: " + error);
        }
    }
}
