#include "simpleCalculation.h"

double calculator(double num1, double num2, char operation);

int calculatorMain(int argc, char* argv[]) {
    char operation;
    double num1, num2;
    int precision;
    // Enter precision
    printf("Enter precision(0-15): ");
    precision = inputNUMRange(0,15);

    // Read the operator
    printf("Enter an operator (+, -, *, /): ");
    scanf("%c", &operation);

    // Read the two numbers
    printf("Enter two operands: ");
    scanf("%lf %lf", &num1, &num2);

    double result = calculator(num1, num2, operation);

    printf("Result: %.*lf\n",precision,result);

    while (getchar() != '\n') {};

    return 0;

}

double calculator(double num1, double num2, char operation) {
    double result;

    // Perform the corresponding operation
    if (operation == '+') {

        // Addition
        result = num1 + num2;
    }
    else if (operation == '-') {

        // Subtraction
        result = num1 - num2;
    }
    else if (operation == '*') {

        // Multiplication
        result = num1 * num2;
    }
    else if (operation == '/') {

        // Division
        // Check for division by zero
        if (num2 != 0) {
            result = num1 / num2;
        }
        else {
            printf("Error! Division by zero.\n");
            return INT_MIN;
        }
    }
    else {
        printf("Error! Operator is not correct.\n");
        return INT_MIN;
    }

    return result;
}