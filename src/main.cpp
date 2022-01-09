#include <QApplication>

#include "qtcssapp.h"
#include "commandlineparser.h"

int main(int argc, char **argv) {

    CommandLineParser command_line_parser (argc, argv);

    CommandLineParserItem ui_item ("ui", "Set UI file path", true);
    CommandLineParserItem css_item ("css", "Set CSS file path", true);
    command_line_parser << ui_item << css_item;

    command_line_parser.process();

    if (command_line_parser.is_help() || command_line_parser.is_missed_args()) {
        command_line_parser.show_help();
    }
    else {
        qtcss::Files files;
        files.css = QString::fromStdString(css_item.value());
        files.ui  = QString::fromStdString(ui_item.value());

        QApplication a (argc, argv);
        qtcssapp app;
        if (app.init(files)) {
            return QApplication::exec();
        }

    }

    return EXIT_SUCCESS;
}
