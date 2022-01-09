#ifndef __qtcss_H
#define __qtcss_H

#include <QMap>
#include <QObject>
#include <QFileSystemWatcher>

#include "applicationdata.h"

class QWidget;

class qtcssapp : public QObject {

    Q_OBJECT
    enum class FilesExtensionEnum { UI, CSS };

public:
    qtcssapp();
    ~qtcssapp();
    bool init(qtcss::Files const& files);

private:
    void _init_connections();
    bool _init_files(qtcss::Files const& files);

    void _load_widget(QString const& file);
    void _load_style(QString const& file);

    void _load_settings(uint file_hash, QWidget* widget);
    void _save_settings(uint file_hash, QWidget* widget);


    Q_SLOT void file_changed_slot(const QString &path);

private:
    QMap<FilesExtensionEnum, QString> _ext_enum_map;
    QFileSystemWatcher                _watcher;
    QWidget*                          _widget;
};

#endif /*__qtcss_H*/
