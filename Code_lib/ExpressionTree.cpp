#include "ExpressionTree.h"
#include "MyStack.h"
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <functional>
#include <iostream>

// Constructors and destructors
ExpressionTree::ExpressionTree() {}
ExpressionTree::~ExpressionTree() {}

// Helper function to check if a token is a mathematical operator
bool ExpressionTree::isOperator(const std::string& token) const {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^";

}
// Validates the balance of parentheses in an infix expression
bool ExpressionTree::isValidParentheses(const std::string& expression) const {
    int balance = 0; // Tracks the balance of parentheses
    for (char ch : expression) {
        if (ch == '(') {
            balance++;
        } else if (ch == ')') {
            balance--;
            if (balance < 0) {
                // More closing parentheses than opening
                return false;
            }
        }
    }
    return balance == 0; // Balanced if zero
}

// Checks if a string represents a valid number (integer or decimal)
bool ExpressionTree::isNumber(const std::string& str) const {
    if (str.empty()) return false;

    bool hasDecimal = false;
    // Allow negative numbers by starting from index 1 if first character is '-'
    size_t start = (str[0] == '-') ? 1 : 0;

    if (start == str.length()) return false;

    for (size_t i = start; i < str.length(); i++) {
        if (str[i] == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
        } else if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

// Validates if a token is a valid variable name
bool ExpressionTree::isVariable(const std::string& token)  {
    if (token.empty()) return false;

    // First character must be a letter
    if (!isalpha(token[0])) return false;

    // Remaining characters must be letters or numbers
    for (size_t i = 1; i < token.length(); i++) {
        if (!isalnum(token[i])) return false;
    }
    return true;
}

// Determines operator precedence for correct expression evaluation
int ExpressionTree::precedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "^") return 3;
    return 0;
}

// Recursively deletes all nodes in the tree to prevent memory leaks
void ExpressionTree::deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// Tree building functions
ExpressionTree::TreeNode* ExpressionTree::buildTreeFromInfix(const std::string& infix) {
  MyVector tokens = tokenize(infix);
    if (tokens.getSize() < 3) {
        throw std::runtime_error("Incomplete expression: Not enough operands");
    }

    // New validation function to check expression structure
    validateExpressionStructure(tokens);

    MyStack<TreeNode*> nodes;
    MyStack<string> ops;

    for (int i = 0; i < tokens.getSize(); ++i) {
        const string& token = tokens[i];

        if (isNumber(token) || isVariable(token)) {
            // Handle both numbers and variables (single or multi-character)
            nodes.push(new TreeNode(token));
        }
        else if (token == "(") {
            ops.push(token);
        }
        else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                string op = ops.top();
                ops.pop();

                TreeNode* right = nodes.empty() ? nullptr : nodes.top();
                if (!nodes.empty()) nodes.pop();

                TreeNode* left = nodes.empty() ? nullptr : nodes.top();
                if (!nodes.empty()) nodes.pop();

                TreeNode* node = new TreeNode(op);
                node->left = left;
                node->right = right;

                nodes.push(node);
            }
            if (!ops.empty()) ops.pop(); // Remove "("
        }
        else if (isOperator(token)) {
            while (!ops.empty() && ops.top() != "(" &&
                   (precedence(ops.top()) > precedence(token) ||
                    (precedence(ops.top()) == precedence(token) && token != "^"))) {
                string op = ops.top();
                ops.pop();

                TreeNode* right = nodes.empty() ? nullptr : nodes.top();
                if (!nodes.empty()) nodes.pop();

                TreeNode* left = nodes.empty() ? nullptr : nodes.top();
                if (!nodes.empty()) nodes.pop();

                TreeNode* node = new TreeNode(op);
                node->left = left;
                node->right = right;

                nodes.push(node);
            }
            ops.push(token);
        }
    }

    while (!ops.empty()) {
        string op = ops.top();
        ops.pop();

        TreeNode* right = nodes.empty() ? nullptr : nodes.top();
        if (!nodes.empty()) nodes.pop();

        TreeNode* left = nodes.empty() ? nullptr : nodes.top();
        if (!nodes.empty()) nodes.pop();

        TreeNode* node = new TreeNode(op);
        node->left = left;
        node->right = right;

        nodes.push(node);
    }

    return nodes.empty() ? nullptr : nodes.top();
}
ExpressionTree::TreeNode* ExpressionTree::buildTreeFromPrefix(const MyVector& tokens) {
    validatePrefixExpressionStructure(tokens);
    MyStack<TreeNode*> nodeStack;
    for (int i = tokens.getSize() - 1; i >= 0; --i) {
        const string& token = tokens[i];

        // Handle negative numbers as single tokens
        if (isNumber(token)) {
            nodeStack.push(new TreeNode(token));
        }
        else if (isOperator(token)) {
            TreeNode* node = new TreeNode(token);
            if (!nodeStack.empty()) {
                node->left = nodeStack.top();
                nodeStack.pop();
            }
            if (!nodeStack.empty()) {
                node->right = nodeStack.top();
                nodeStack.pop();
            }
            nodeStack.push(node);
        } else {
            nodeStack.push(new TreeNode(token));
        }
    }
    return nodeStack.empty() ? nullptr : nodeStack.top();
}
ExpressionTree::TreeNode* ExpressionTree::buildTreeFromPostfix(const MyVector& tokens) {
    validatePostfixExpressionStructure(tokens);
    MyStack<TreeNode*> nodeStack;
    for (int i = 0; i < tokens.getSize(); ++i) {
        const string& token = tokens[i];

        // Handle negative numbers as single tokens
        if (isNumber(token)) {
            nodeStack.push(new TreeNode(token));
        }
        else if (isOperator(token)) {
            TreeNode* node = new TreeNode(token);
            if (!nodeStack.empty()) {
                node->right = nodeStack.top();
                nodeStack.pop();
            }
            if (!nodeStack.empty()) {
                node->left = nodeStack.top();
                nodeStack.pop();
            }
            nodeStack.push(node);
        } else {
            nodeStack.push(new TreeNode(token));
        }
    }
    return nodeStack.empty() ? nullptr : nodeStack.top();
}

// Traversal functions
std::string ExpressionTree::inorder(TreeNode* root) const {
    if (!root) return "";
    if (isOperator(root->value)) {
        return "( " + inorder(root->left) + " " + root->value + " " + inorder(root->right) + " )";
    }
    return root->value;
}
std::string ExpressionTree::preorder(TreeNode* root) const {
    if (!root) return "";
    string result = root->value;
    string leftStr = preorder(root->left);
    string rightStr = preorder(root->right);

    if (!leftStr.empty()) result += " " + leftStr;
    if (!rightStr.empty()) result += " " + rightStr;
    return result;
}
std::string ExpressionTree::postorder(TreeNode* root) const {
    if (!root) return "";
    string result = "";
    string leftStr = postorder(root->left);
    string rightStr = postorder(root->right);

    if (!leftStr.empty()) result += leftStr + " ";
    if (!rightStr.empty()) result += rightStr + " ";
    result += root->value;
    return result;
}

long double ExpressionTree::evaluate(TreeNode* root, const std::unordered_map<std::string, double>& variableValues) const {
    if (!root) return 0;

    if (!isOperator(root->value)) {
        if (isNumber(root->value)) {
            return stod(root->value); // Convert number strings to double
        }
        if (isVariable(root->value) && variableValues.find(root->value) != variableValues.end()) {
            return variableValues.at(root->value); // Use the value for the variable
        }
        throw std::runtime_error("Undefined variable: " + root->value);
    }

    double leftValue = evaluate(root->left, variableValues);
    double rightValue = evaluate(root->right, variableValues);

    if (root->value == "+") return leftValue + rightValue;
    if (root->value == "-") return leftValue - rightValue;
    if (root->value == "*") return leftValue * rightValue;
    if (root->value == "/") {
        if (rightValue == 0) throw std::runtime_error("Division by zero!");
        return leftValue / rightValue;
    }
    if (root->value == "%") {
        if (rightValue == 0) throw std::runtime_error("Modulo by zero!");
        return fmod(leftValue, rightValue);
    }
    if (root->value == "^") return pow(leftValue, rightValue);

    throw std::runtime_error("Invalid operator!");
}

// Tokenize
MyVector ExpressionTree::tokenize(const std::string& expression) const {
    MyVector tokens;
    istringstream iss(expression);
    string token;

    while (iss >> token) {
        // If token is just a minus sign, it's an operator
        if (token == "-") {
            tokens.push_back(token);
            continue;
        }

        // Check if it's a negative number (starts with minus and has more characters)
        if (token[0] == '-' && token.length() > 1) {
            // Verify the rest is a valid number
            bool isValidNumber = true;
            bool hasDecimal = false;
            for (size_t i = 1; i < token.length(); i++) {
                if (token[i] == '.') {
                    if (hasDecimal) {
                        isValidNumber = false;
                        break;
                    }
                    hasDecimal = true;
                } else if (!isdigit(token[i])) {
                    isValidNumber = false;
                    break;
                }
            }
            if (isValidNumber) {
                tokens.push_back(token);  // Add as a single negative number token
                continue;
            }
        }

        // Handle other operators
        if (isOperator(token)) {
            tokens.push_back(token);
            continue;
        }

        // Handle parentheses
        if (token == "(" || token == ")") {
            tokens.push_back(token);
            continue;
        }

        // Handle regular numbers and variables
        if (isNumber(token) || (token.length() == 1 && isalpha(token[0]))) {
            tokens.push_back(token);
            continue;
        }

        // If we get here, it might be a multi-character variable
        bool isVariable = true;
        for (char c : token) {
            if (!isalnum(c)) {
                isVariable = false;
                break;
            }
        }
        if (isVariable) {
            tokens.push_back(token);
        }

    }
    return tokens;
}

void ExpressionTree::validateExpressionStructure(const MyVector& tokens) {
    int operandCount = 0;
    int operatorCount = 0;
    int parenthesesBalance = 0;

    for (int i = 0; i < tokens.getSize(); ++i) {
        const string& token = tokens[i];

        if (token == "(") {
            parenthesesBalance++;
            if (i > 0 && (isNumber(tokens[i-1]) || isVariable(tokens[i-1]) || tokens[i-1] == ")")) {
                throw std::runtime_error("Invalid syntax: Missing operator before opening parenthesis");
            }
        }
        else if (token == ")") {
            parenthesesBalance--;
            if (parenthesesBalance < 0) {
                throw std::runtime_error("Unbalanced parentheses: Too many closing parentheses");
            }
        }
        else if (isOperator(token)) {
            operatorCount++;
            // Check if operator is at the start or end of expression
            if (i == 0 || i == tokens.getSize() - 1) {
                throw std::runtime_error("Invalid expression: Operator cannot be at the start or end");
            }
            // Ensure operators are separated by operands
            if (i > 0 && isOperator(tokens[i-1])) {
                throw std::runtime_error("Invalid syntax: Consecutive operators");
            }
        }
        else if (isNumber(token) || isVariable(token)) {
            operandCount++;
            // Check for implicit multiplication (number/variable next to parenthesis)
            if (i > 0 && (tokens[i-1] == ")" || isNumber(tokens[i-1]) || isVariable(tokens[i-1]))) {
                throw std::runtime_error("Invalid syntax: Missing operator between operands");
            }
        }
    }

    // Final validation checks
    if (parenthesesBalance != 0) {
        throw std::runtime_error("Unbalanced parentheses");
    }

    // Ensure we have the right balance of operands and operators
    if (operandCount <= operatorCount) {
        throw std::runtime_error("Incomplete expression: Not enough operands for operators");
    }
}

void ExpressionTree::validatePostfixExpressionStructure(const MyVector& tokens) {
    // Stack to track operand availability
    MyStack<int> operandStack;

    // Iterate through tokens from left to right
    for (int i = 0; i < tokens.getSize(); ++i) {
        const string& token = tokens[i];

        if (isNumber(token) || isVariable(token)) {
            // Operand found, add to stack
            operandStack.push(0);
        }
        else if (isOperator(token)) {
            // Check if there are enough operands for this operator
            int requiredOperands = 2;  // Most operators need 2 operands

            // Check if we have enough operands in the stack
            if (operandStack.size() < requiredOperands) {
                throw std::runtime_error("Postfix validation error: Insufficient operands for operator '" + token + "'");
            }

            // Remove the required number of operand placeholders
            for (int j = 0; j < requiredOperands; ++j) {
                if (!operandStack.empty()) {
                    operandStack.pop();
                }
            }

            // The operator consumes operands and becomes a result
            operandStack.push(0);
        }
        else {
            throw std::runtime_error("Invalid token in postfix expression: " + token);
        }
    }

    // Final validation: exactly one operand (the final result) should remain
    if (operandStack.size() != 1) {
        throw std::runtime_error("Invalid postfix expression: Incorrect number of operands");
    }
}

void ExpressionTree::validatePrefixExpressionStructure(const MyVector& tokens) {
    // Stack to track operand requirements
    MyStack<int> operandStack;

    // Iterate through tokens from right to left
    for (int i = tokens.getSize() - 1; i >= 0; --i) {
        const string& token = tokens[i];

        if (isNumber(token) || isVariable(token)) {
            // Operand found, add to stack with no further requirements
            operandStack.push(0);
        }
        else if (isOperator(token)) {
            // Check if there are enough operands for this operator
            int requiredOperands = 2;  // Most operators need 2 operands

            // Check if we have enough operands in the stack
            if (operandStack.size() < requiredOperands) {
                throw std::runtime_error("Prefix validation error: Insufficient operands for operator '" + token + "'");
            }

            // Remove the required number of operand placeholders
            for (int j = 0; j < requiredOperands; ++j) {
                if (!operandStack.empty()) {
                    operandStack.pop();
                }
            }

            // The operator itself becomes an operand
            operandStack.push(0);
        }
        else {
            throw std::runtime_error("Invalid token in prefix expression: " + token);
        }
    }

    // Final validation: exactly one operand (the final result) should remain
    if (operandStack.size() != 1) {
        throw std::runtime_error("Invalid prefix expression: Incorrect number of operands");
    }
}


// Validate if the expression entered matches the expected type (the one chosen by the user)
void ExpressionTree::validateExpressionType(const MyVector& tokens, int expectedType) {
    // expectedType: 1 for Infix, 2 for Prefix, 3 for Postfix

    // Check for empty expression, if the user enters nothing
    if (tokens.getSize() < 1) {
        throw std::runtime_error("Empty expression. Enter an expression and leave spaces between tokens, operators, and parentheses");
    }

    // Determine the actual type of expression
    int actualType = determineExpressionType(tokens);

    // Compare actual type with expected type
    if (actualType != expectedType) {
        std::string typeNames[] = {"", "Infix", "Prefix", "Postfix"};
        throw std::runtime_error("Incorrect expression type. Expected " +
                                 typeNames[expectedType] + ", but got " +
                                 typeNames[actualType] + " expression.");
    }
}


// Detect expression type based on token arrangement
int ExpressionTree::determineExpressionType(const MyVector& tokens) {

    // Prefix: Operator comes first
    if (isOperator(tokens[0])) {
        return 2; // Prefix
    }

    // Postfix: Operator comes last
    if (isOperator(tokens[tokens.getSize() - 1])) {
        return 3; // Postfix
    }

    // Infix: Operators are between operands
    for (int i = 1; i < tokens.getSize() - 1; ++i) {
        if (isOperator(tokens[i])) {
            return 1; // Infix
        }
    }

    // If no clear type is detected
    throw std::runtime_error("Unable to determine expression type");
}

// Function to prompt the user for variable values if he enters an expressions containing variables
unordered_map<std::string, double> ExpressionTree::getVariableValues(ExpressionTree::TreeNode* root) {
    unordered_map<std::string, double> variableValues;

    function<void(ExpressionTree::TreeNode*)> collectVariables = [&](ExpressionTree::TreeNode* node) {
        if (!node) return;
        if (ExpressionTree::isVariable(node->value)) {
            if (variableValues.find(node->value) == variableValues.end()) {
                cout << "Enter the value for variable '" << node->value << "': " << std::endl;
                double value;
                // cin >> value;
                while (!(cin >> value)) {
                    cin.clear(); // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Invalid input! Please enter a numeric value for '" << node->value << "': " << endl;
                }
                variableValues[node->value] = value;
            }
        }
        collectVariables(node->left);
        collectVariables(node->right);
    };

    collectVariables(root);
    return variableValues;
}