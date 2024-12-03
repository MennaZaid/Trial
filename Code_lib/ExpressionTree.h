//implemented by Fatima Nageh
#ifndef EXPRESSION_TREE_H
#define EXPRESSION_TREE_H

#include <string>
#include "MyVector.h"
#include <unordered_map>

class ExpressionTree {
public:
    struct TreeNode {
        std::string value;
        TreeNode* left;
        TreeNode* right;
        TreeNode(const std::string& val) : value(val), left(nullptr), right(nullptr) {}
    };

    TreeNode* root;

    // Helper functions
    bool isOperator(const std::string& token) const;
    bool isValidParentheses(const std::string& expression) const;
    bool isNumber(const std::string& str) const;
    static bool isVariable(const std::string& token);
    static int precedence(const std::string& op);
    void deleteTree(TreeNode* node);

    // Constructors and destructors
    ExpressionTree();
    ~ExpressionTree();

    // Tree building functions
    TreeNode* buildTreeFromInfix(const std::string& infix);
    TreeNode* buildTreeFromPrefix(const MyVector& tokens);
    TreeNode* buildTreeFromPostfix(const MyVector& tokens);

    // Traversal functions
    std::string inorder(TreeNode* root) const;
    std::string preorder(TreeNode* root) const;
    std::string postorder(TreeNode* root) const;

    // Evaluation and tokenization functions
    long double evaluate(TreeNode* root, const std::unordered_map<std::string, double>& variableValues) const;
    MyVector tokenize(const std::string& expression) const;

    //Expression Validation Functions
    void validateExpressionType(const MyVector& tokens, int expectedType);
    int  determineExpressionType(const MyVector& tokens);
    void validateExpressionStructure(const MyVector& tokens);
    void validatePostfixExpressionStructure(const MyVector& tokens);
    void validatePrefixExpressionStructure(const MyVector& tokens);
    unordered_map<std::string, double> getVariableValues(ExpressionTree::TreeNode* root);

};

#endif // EXPRESSION_TREE_H