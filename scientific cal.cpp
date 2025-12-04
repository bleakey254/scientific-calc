#include <iostream>
#include <cstring>
#include <cmath>
#include <cctype>
#include <iomanip>
#include <stdlib.h>
using namespace std;

// Global constants
const double PI = 3.14159265358979323846;
const double E  = 2.71828182845904523536;

// Forward declaration
double evaluate(const char* expr, int& pos);

double parseNumber(const char* expr, int& pos) {
    double num = 0;
    while (pos < strlen(expr) && isdigit(expr[pos])) {
        num = num * 10 + (expr[pos] - '0');
        pos++;
    }
    if (pos < strlen(expr) && expr[pos] == '.') {
        pos++;
        double frac = 0.1;
        while (pos < strlen(expr) && isdigit(expr[pos])) {
            num += (expr[pos] - '0') * frac;
            frac /= 10;
            pos++;
        }
    }
    return num;
}

double parseFactor(const char* expr, int& pos) {
    double val = 0;
    char ch = expr[pos];

    if (ch == '(') {
        pos++; // eat (
        val = evaluate(expr, pos);
        pos++; // eat )
    }
    else if (isdigit(ch) || ch == '.') {
        val = parseNumber(expr, pos);
    }
    else if (ch == 'p' && strncmp(expr + pos, "pi", 2) == 0) {
        pos += 2;
        val = PI;
    }
    else if (ch == 'e' && (pos == 0 || !isalpha(expr[pos-1]))) {
        pos++;
        val = E;
    }
    else if (strncmp(expr + pos, "sqrt(", 5) == 0) {
        pos += 5;
        val = sqrt(evaluate(expr, pos));
        pos++; // eat )
    }
    else if (strncmp(expr + pos, "sin(", 4) == 0) {
        pos += 4;
        val = sin(evaluate(expr, pos));
        pos++;
    }
    else if (strncmp(expr + pos, "cos(", 4) == 0) {
        pos += 4;
        val = cos(evaluate(expr, pos));
        pos++;
    }
    else if (strncmp(expr + pos, "tan(", 4) == 0) {
        pos += 4;
        val = tan(evaluate(expr, pos));
        pos++;
    }
    else if (strncmp(expr + pos, "log(", 4) == 0) {
        pos += 4;
        val = log10(evaluate(expr, pos));
        pos++;
    }
    else if (strncmp(expr + pos, "ln(", 3) == 0) {
        pos += 3;
        val = log(evaluate(expr, pos));
        pos++;
    }
    else {
        cout << "Error at: " << expr + pos << endl;
        pos = strlen(expr);
    }

    // Power ^ (right-associative)
    while (pos < strlen(expr) && expr[pos] == '^') {
        pos++;
        val = pow(val, parseFactor(expr, pos));
    }
    return val;
}

double parseTerm(const char* expr, int& pos) {
    double val = parseFactor(expr, pos);
    while (pos < strlen(expr)) {
        if (expr[pos] == '*') {
            pos++;
            val *= parseFactor(expr, pos);
        }
        else if (expr[pos] == '/') {
            pos++;
            double temp = parseFactor(expr, pos);
            if (temp != 0) val /= temp;
            else { cout << "Division by zero!\n"; return 0; }
        }
        else break;
    }
    return val;
}

double evaluate(const char* expr, int& pos) {
    double val = parseTerm(expr, pos);
    while (pos < strlen(expr)) {
        if (expr[pos] == '+') {
            pos++;
            val += parseTerm(expr, pos);
        }
        else if (expr[pos] == '-') {
            pos++;
            val -= parseTerm(expr, pos);
        }
        else break;
    }
    return val;
}

double calculate(const char* input) {
    char expr[1000] = "";
    int j = 0;
    for (int i = 0; input[i]; i++) {
        if (!isspace(input[i])) expr[j++] = tolower(input[i]);
    }
    int pos = 0;
    double result = evaluate(expr, pos);
    if (pos < strlen(expr)) {
        cout << "Syntax error!\n";
        return 0;
    }
    return result;
}

int main() {
    system("cls");
    cout << "==================================================\n";
    cout << "   EXPRESSION-BASED SCIENTIFIC CALCULATOR\n";
    cout << "          (Works 100% in Falcon C++)\n";
    cout << "==================================================\n\n";
    cout << "Examples you can type:\n";
    cout << "   2 + 3*4^2\n";
    cout << "   sin(pi/4)*2 + cos(0)\n";
    cout << "   log(100) + ln(e^3)\n";
    cout << "   sqrt(2)^2 + 5*3\n";
    cout << "   2^10\n\n";

    char input[1000];
    while (true) {
        cout << "\nEnter expression (or 'quit' to exit): ";
        cin.getline(input, 1000);

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0 || strcmp(input, "q") == 0)
            break;

        if (strlen(input) == 0) continue;

        double ans = calculate(input);
        cout << fixed << setprecision(10);
        cout << "Result = " << ans << endl;
    }

    cout << "\nThanks for using! Goodbye :)\n";
    return 0;
}