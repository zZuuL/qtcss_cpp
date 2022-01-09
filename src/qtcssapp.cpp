#include <QDir>
#include <QFile>
#include <QHash>
#include <QDebug>

#include <QSettings>
#include <QFileInfo>

#include <QWidget>
#include <QUiLoader>
#include <QMapIterator>

#include "qtcssapp.h"

/*
./bin/qtcss --ui=/home/andrey/tmp/widget.ui --css=/home/andrey/tmp/style.css
*/

qtcssapp::qtcssapp()
    : _widget(nullptr)
{}

qtcssapp::~qtcssapp()
{
    if (_widget) {
        delete _widget;
    }
}

void qtcssapp::_init_connections() {
    connect(&_watcher,
            SIGNAL(fileChanged(const QString&)),
            SLOT(file_changed_slot(const QString&)));
}

bool qtcssapp::init(qtcss::Files const& files) {

    if (_init_files(files)) {
        _ext_enum_map.insert(FilesExtensionEnum::UI, QFileInfo(files.ui).completeSuffix().toUpper());
        _ext_enum_map.insert(FilesExtensionEnum::CSS, QFileInfo(files.css).completeSuffix().toUpper());
        _watcher.addPaths(QStringList() << files.css << files.ui);
        _load_widget(files.ui);
        _load_style(files.css);
        _init_connections();
        return true;
    }

    return false;
}

void qtcssapp::_load_widget(QString const& file) {
    if (_widget != nullptr) {
        _save_settings(qHash(file), _widget);
        _widget->close();
        delete _widget;
    }

    QFile f(file);

    if (f.open(QFile::ReadOnly)) {

        QUiLoader loader;
        _widget = loader.load(&f);
        f.close();

        if (_widget != nullptr) {
            _load_settings(qHash(file), _widget);
            _widget->show();
        }
    }
}

void qtcssapp::_load_style(QString const& file) {
    if(_widget != nullptr) {
        QFile f(file);
        if (f.open(QIODevice::ReadOnly)) {
            _widget->setStyleSheet(QString(f.readAll()));
            f.close();
        }
    }
}

bool qtcssapp::_init_files(qtcss::Files const& files) {
    for (const QString &file: {files.css, files.ui}) {
        QFile f(file);
        if (!f.exists()) {
            qDebug() << file << " not exists!";
            return false;
        }
    }
    return true;
}

void qtcssapp::file_changed_slot(const QString &path) {

    QFileInfo info (path);
    QString ext = info.completeSuffix().toUpper();

    QMapIterator<FilesExtensionEnum, QString> i(_ext_enum_map);
    while (i.hasNext()) {
        i.next();
        if (i.value() == ext) {
            switch (i.key()) {
                case FilesExtensionEnum::UI: {
                    _load_widget(path);
                    break;
                }

                case FilesExtensionEnum::CSS: {
                    _load_style(path);
                    break;
                }

                default: {
                    return ;
                }
            }

            if (!_watcher.files().contains(path)) {
                _watcher.addPath(path);
            }
        }
    }
}

void qtcssapp::_load_settings(uint file_hash, QWidget* widget) {
    QSettings settings;
    QString hash_str = QString::number(file_hash);
    if (settings.contains(hash_str)) {
        widget->move(settings.value(hash_str).value<QPoint>());
    }
}

void qtcssapp::_save_settings(uint file_hash, QWidget* widget) {
    QSettings settings;
    settings.setValue(QString::number(file_hash), widget->pos());
}
