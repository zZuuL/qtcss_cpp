#ifndef __command_line_parser_H
#define __command_line_parser_H

#include <set>
#include <map>

class CommandLineParserItem {
    friend class CommandLineParser;

    std::string _name;
    std::string _description;
    bool        _requered;

    std::string _value;

public:
    CommandLineParserItem(std::string const& _name, std::string const& _description, bool requered = false);
    std::string value() const;
};

class CommandLineParser {
    typedef std::map<std::string, CommandLineParserItem&> CommandLineParserItemMap;

    int                      _argc;
    char**                   _argv;
    CommandLineParserItemMap _items;
    bool                     _is_help;
    std::set<std::string>    _requered_args;

public:
    CommandLineParser(int argc, char** argv);

    void process();
    bool is_help() const;
    bool is_missed_args() const;
    void show_help() const;

    CommandLineParser& operator<< (CommandLineParserItem& item);
};

#endif /*__command_line_parser_H*/
