#include<iostream>
#include<string>

#include "parser.h"

using namespace std;

int main(int argc, char *argv[]) {
    for (int idx = 1; idx < argc; ++idx) {
        string opt(argv[idx]);
        if (opt == "-v") {
            cout << "Expression Evaluator 0.0\n";
            exit(0);
        } else if (opt == "-h") {
            cout << "Expression Evaluator\n";
            cout << ">> Type one-line expressions to evaluate.\n";
            cout << ">> Type quit to exit.\n";
            exit(0);
        } else {
            cout << "Usage: " << argv[0] << "[-v|-h]" << endl;
            cout << "  -h: help message" << endl;
            cout << "  -v: version info" << endl;
            exit(1);
        }
    }

    cout << "Expression Evaluator\n";
    cout << ">> Type quit to exit.\n";

    Parser parser;

    string input;
    while(true) {
        cout << ">> ";
        std::getline(cin, input);
        if (input.empty()) continue;

        if ((input.size() == 4) && (input == "quit"))
            break;
        IExpression *expr = parser.parse(input);
        if (!expr) {
            cout << "Error in parsing the input.\n";
        }
        if (!expr->evaluate()) {
            cout << "Error in evaluating expression!\n";
        } else {
            cout << ">> = " << expr->getValue() << endl;
        }
    }
    return 0;
}

