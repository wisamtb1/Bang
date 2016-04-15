#ifndef EXPLORER_H
#define EXPLORER_H

#include <string>

#include <QListView>
#include <QMouseEvent>
#include <QToolButton>
#include <QFileSystemModel>

#include "Bang.h"

#include "Logger.h"

#include "File.h"
#include "FileMesh.h"
#include "InspectorMeshFileWidget.h"
#include "InspectorTexture2DWidget.h"

class Explorer : public QListView
{
    Q_OBJECT

    friend class ExplorerDirTree;

private:
    QFileSystemModel *fileSystemModel = nullptr;
    QToolButton *buttonDirUp;
    QToolButton *buttonChangeViewMode;

    static std::string topPath;

    void setDir(const std::string &path);
    std::string getCurrentDir() const;

    std::string lastSelectedFileName = "";
    QTimer *updateTimer = nullptr;

public:
    Explorer(QWidget *parent);
    virtual ~Explorer();

    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void dropEvent(QDropEvent *e) override;

    static std::string GetTopPath();

    //Updates the Inspector with the selected file info
    void RefreshInspector();

public slots:
    void Refresh();

    void OnDirLoaded(QString dir);
    void OnButtonDirUpClicked();
    void OnButtonChangeViewModeClicked();
};

#endif // EXPLORER_H
