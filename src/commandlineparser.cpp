#include <list>
#include <vector>
#include <iostream>
#include <sstream>

#include "commandlineparser.h"

namespace Internal {

class string : public std::string {
public:
    string(std::string const& str)
        : std::string(str)
    {}

    std::vector<string> split(const std::string& delimiter) const {

        std::vector<string> result;

        size_t pos = 0;
        std::string tmp = *this;
        while ((pos = tmp.find(delimiter)) != std::string::npos) {
            result.push_back (tmp.substr(0, pos));
            tmp.erase(0, pos + delimiter.length());
        }
        result.push_back(tmp);

        return result;
    }

    bool contains(std::list<std::string> const& items) const {
        for (auto const& item: items) {
            if((*this).find(item) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    const string& operator* (int count) {
        const string tmp = *this;
        for (int i = 0; i < count - 1; ++i) *this += tmp;
        return *this;
    }

    string& remove_substring(const std::string& str) {
        size_t pos = std::string::npos;
        while ((pos  = (*this).find(str))!= std::string::npos) {
            (*this).erase(pos, str.length());
        }
        return *this;
    }
};

} /*Internal*/

/*
 * CommandLineParserItem
 */

CommandLineParserItem::CommandLineParserItem(std::string const& _name,
                                             std::string const& _description,
                                             bool requered /* = false */)
    : _name(_name)
    , _description(_description)
    , _requered(requered)
    , _value("")
{}


std::string CommandLineParserItem::value() const {
    return _value;
}

/*
 * CommandLineParser
 */

CommandLineParser::CommandLineParser(int argc, char** argv)
    : _argc(argc)
    , _argv(argv)
    , _is_help(false)
{}


void CommandLineParser::process() {

    for (int i = 0; i != _argc; ++i) {
        Internal::string param = Internal::string(_argv[i]);
        if (!_is_help) {
            if(param.contains({"--help"})) { _is_help = true; }
        }

        std::vector<Internal::string> tmp = Internal::string(_argv[i]).split("=");
        if(tmp.size() == 2) {
            const Internal::string &param_name  = tmp[0].remove_substring("-").remove_substring(" ");
            const Internal::string &param_value = tmp[1];

            _requered_args.erase(param_name);

            CommandLineParserItemMap::iterator i =_items.find(param_name);
            if (i != _items.end()) {
                i->second._value = param_value;
            }
        }
    }
}


bool CommandLineParser::is_help() const {
    return _is_help;
}


bool CommandLineParser::is_missed_args() const {
    return !_requered_args.empty();
}

void CommandLineParser::show_help() const {
    std::stringstream ss;
    ss << "Name\tDescription" << std::endl << Internal::string("=") * 40 << std::endl;
    ss << "--help\tShow this help" << std::endl;
    for (auto const& item: _items) {
        ss << "--" << item.second._name << "\t" << item.second._description << (item.second._requered ? " (requered)\n" : "\n");
    }
    std::cout << ss.str() << std::endl;
}


CommandLineParser& CommandLineParser::operator<< (CommandLineParserItem& item) {
    _items.insert(std::pair<std::string, CommandLineParserItem&>(item._name, item));
    if (item._requered) {
        _requered_args.insert(item._name);
    }
    return *this;
}
