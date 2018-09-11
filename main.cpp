#include <iostream>
#include "Args.h"
#include <exception>

using namespace std;

int main(int argc, const char *argv[])
{
    try {
        Arguments arguments;
        arguments.add('h', "help", "Short description of usage of program");
        arguments.add('p', "prefix", "path where to install something", true);
        arguments.parse(argc, argv);
        if (arguments("help")) {
            cout << "Help is set!" << endl;
        }
        if (arguments("prefix")) {
            cout << "path is " << arguments("prefix").getFloat() << endl;
        }
    } catch (std::exception &e) {
        cout << "\nException: " << e.what() << endl;
    }

    cout << "Hello World!" << endl;
    return 0;
}
