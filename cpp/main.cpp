#include<iostream>
#include<string>

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

    string input;
    while(true) {
        cout << ">> ";
        cin >> input;
        if ((input.size() == 4) && (input == "quit"))
            break;
    }
    return 0;
}

