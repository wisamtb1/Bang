#ifndef DIALOGBROWSEASSETFILE_H
#define DIALOGBROWSEASSETFILE_H

#include <QDialog>
#include <QModelIndex>

#include "Map.h"
#include "List.h"
#include "String.h"

class QTabWidget;
class QListWidget;
class QListWidgetItem;
class DialogBrowseAssetFile : public QDialog
{
    Q_OBJECT

public:
    DialogBrowseAssetFile(String *resultFile);

    void Show(QWidget *parent = nullptr,
              const List<String> &extensions = {});

private:
    String *m_resultFile = nullptr;
    Map<QListWidgetItem*, String> m_item_To_absoluteFilepath;

    const int c_iconSize = 60;
    QTabWidget *m_tabWidget = nullptr;
    QListWidget *m_projectFileList = nullptr;
    QListWidget *m_engineFileList  = nullptr;
    QPushButton *m_openButton = nullptr;

    void ShowOnList(QListWidget *listWidget,
                    bool engineFilepaths,
                    const List<String> &filepaths);

    QListWidget *CreateList() const;

private slots:
    void OnSelectionChange(int newIndex);
    void OnFileAccepted();
    void OnFileAccepted(QModelIndex modelIndex);
};

#endif // DIALOGBROWSEASSETFILE_H
