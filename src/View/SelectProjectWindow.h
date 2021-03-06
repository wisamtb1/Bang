#ifndef SELECTPROJECTWINDOW_H
#define SELECTPROJECTWINDOW_H

#include <QMainWindow>

#include "String.h"
#include "ui_SelectProjectWindow.h"

class SelectProjectWindow :
        public QObject,
        public Ui_SelectProjectWindow
{
    Q_OBJECT

public:
    SelectProjectWindow();

    static String ExecAndGetProjectFilepath(QMainWindow *window, QApplication *app);
    static SelectProjectWindow *GetInstance();
    QMainWindow* GetMainWindow() const;
    void FillRecentProjectsList();

public slots:
    void OnRecentProjectListSelectionChanged(int row, int column);
    void OnRecentProjectDoubleClicked(int row, int column);

    void OnLoadRecentProject();
    void OnCreateNewProject();
    void OnBrowseProject();

private:
    static SelectProjectWindow *s_instance;
    String m_loadedProjectFile = "";
    QMainWindow *m_window = nullptr;
    bool m_directlyClosedByUser = false;

    String m_selectedRecentProjectFilepath = "";
};

#endif // SELECTPROJECTWINDOW_H
