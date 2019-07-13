#ifndef ARGS_H
#define ARGS_H

#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>

class Arguments;

class Option {
    const char shortOption_;
    const std::string longOption_;
    const std::string description_;
    const bool hasArgument_;
    bool present_ = false;
    std::string argument_;

    friend Arguments;

public:
    Option(char shortOption,
           const std::string &longOption,
           const std::string description,
           bool hasArgument)
        :shortOption_(shortOption),
         longOption_(longOption),
         description_(description),
         hasArgument_(hasArgument)
    {}

    std::string description() const {
        return description_;
    }

    int getInt() const {
        using namespace std::string_literals;
        try {
            return std::stoi(argument_, nullptr, 0);
        } catch (...) {
            throw std::logic_error(argument_ +
                                   " cannot be converted to int"s);
        }
    }

    float getFloat() const {
        using namespace std::string_literals;
        try {
            return std::stof(argument_);
        } catch (...) {
            throw std::logic_error(argument_ +
                                   " cannot be converted to float"s);
        }
    }

    double getDouble() const {
        using namespace std::string_literals;
        try {
            return std::stod(argument_);
        } catch (...) {
            throw std::logic_error(argument_ +
                                   "cannot be converted to double"s);
        }
    }

    std::string getString() const {
        return argument_;
    }

    explicit operator bool() const {
        return present_;
    }
};

class Arguments {
    std::vector<Option> options;

public:

    void add(char shortOption,
             const std::string &longOption,
             const std::string &description,
             bool hasArgument = false) {
        options.emplace_back(shortOption, longOption, description, hasArgument);
    }

    void parse(const int argc, const char *argv[]) {
        using namespace std::string_literals;
        auto i = 1;
        while (i < argc) {
            if (isShortOption(argv[i])) {
                parseShortOptions(argc, argv, i);
            } else if (isLongOption(argv[i])) {
                parseLongOption(argc, argv, i);
            } else {
                throw std::invalid_argument("Option \""s +
                                            argv[i] +
                                            "\" has invalid format. "
                                            "Valid formats are: -o [argumnent]; "
                                            "--option [argument]; "
                                            "--option[=argument]"s );
            }
        }
    }

    Option& operator()(const std::string &opt) {
        using namespace std::string_literals;
        for (auto &option : options) {
            if (option.longOption_ == opt) {
                return option;
            }
        }
        throw std::logic_error("Option "s +
                               opt +
                               " does not exist"s);
    }

    void printOptions() {
        for (const auto &option : options) {
            std::cerr << "-" << option.shortOption_
                      << "\t"
                      << "--" << option.longOption_
                      << "\t\t"
                      << option.description_
                      << "\n";
        }
    }

private:

    bool isShortOption(std::string str) {
        return str.length() > 1
            && str[0] == '-'
            && str[1] != '-';
    }

    bool isLongOption(std::string str) {
        return str[0] == '-'
            && str[1] == '-';
    }

    Option &findOption(char shortOption) {
        using namespace std::string_literals;
        for (auto &option : options) {
            if (option.shortOption_ == shortOption) {
                return option;
            }
        }
        throw std::invalid_argument("Option "s + "-"s + std::string(1,shortOption) + " does not exist"s);
    }

    Option &findOption(const std::string &longOption) {
        using namespace std::string_literals;
        for (auto &option : options) {
            if (option.longOption_ == longOption) {
                return option;
            }
        }
        throw std::invalid_argument("Option "s + "--"s + longOption + " does not exist"s);
    }

    void parseShortOptions(const int argc, const char *argv[], int &index) {
        using namespace std;
        using namespace std::string_literals;

        string shortOpts = argv[index];
        shortOpts.erase(shortOpts.begin());

        for (unsigned i = 0; i < shortOpts.length(); ++i) {
            Option &option = findOption(shortOpts[i]);
            option.present_ = true;
            if (option.hasArgument_ && i == shortOpts.length() - 1) {
                if (index + 1 >= argc) {
                    throw logic_error("Option -"s +
                                      string(1,option.shortOption_) +
                                      " requires an argument");
                } else {
                    option.argument_ = argv[++index];
                }
            }
        }
        index++;
    }

    void parseLongOption(const int argc, const char *argv[], int &index) {
        using namespace std;
        using namespace std::string_literals;

        string longOpt = argv[index];
        longOpt.erase(longOpt.begin(), longOpt.begin() + 2);
        stringstream stream(longOpt);
        string name;
        getline(stream, name, '=');
        Option &option = findOption(name);
        option.present_ = true;
        if (option.hasArgument_) {
            string arg;
            getline(stream, arg);
            if (arg.empty()) {
                if (index + 1 >= argc) {
                    throw logic_error("Option --"s + name + " requires an argument");
                } else {
                    option.argument_ = argv[++index];
                }
            } else {
                option.argument_ = arg;
            }
        }
        index++;
    }
};

#endif // ARGS_H
