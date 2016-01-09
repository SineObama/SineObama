#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <cassert>
using namespace std;
struct Token {
    string text;
    Token(string text)
            : text(text) {
    }
    int intValue() {
        stringstream ss(text);
        int value;
        ss >> value;
        return value;
    }
    static list<Token> tokenize(istream & in) {
        list<Token> tokens;
        while (!in.eof()) {
            string token;
            in >> token;
            if (token.length() > 0)
                tokens.push_back(Token(token));
        }
        return tokens;
    }
};

class Parser {
    struct Tree {
        Token token;
        Tree * left;  // left child
        Tree * right;  // right child
        Tree(Token token)
                : token(token) {
            left = 0;
            right = 0;
        }
        ~Tree() {
            if (left != 0)
                delete left;
            if (right != 0)
                delete right;
        }
    };
    Tree * expression;
    int result;
    static int execute(Tree * expression) {
        if (expression->left == 0) {
            assert(expression->right == 0);
            return expression->token.intValue();
        } else {
            int left = execute(expression->left);
            int right = execute(expression->right);
            switch (expression->token.text[0]) {
                case '+':
                    return left + right;
                case '-':
                    return left - right;
                case '*':
                    return left * right;
                case '/':
                    return left / right;
                    // default: throw runtime_error("Unknown operator.");
            }
        }
    }

 public:

    Parser() {
        expression = 0;
        result = 0;
    }
    ~Parser() {
        if (expression != 0)
            delete expression;
    }
    int execute() {
        if (expression != 0) {
            result = execute(expression);
            delete expression;
            expression = 0;
        }
        return result;
    }

    void parse(list<Token> & l) {
        delete expression;
        expression = 0;
        if (!l.empty())
            dobracket(l, expression);
    }

    int getPrior(char c) {
        switch (c) {
            case '+':
            case '-':
                return 1;
            case '*':
            case '/':
                return 2;
            default:
                return 0;
        }
    }

    void dobracket(list<Token> & l, Tree *&root) {
        if (get(l, root))
            return;

        Tree *left = root;
        root = new Tree(l.front());  // must be operator
        l.pop_front();
        root->left = left;

        if (get(l, root->right))
            return;

        if (getPrior(root->token.text[0]) == 1
                && getPrior(l.front().text[0]) == 2)
            dobracket(l, root->right);
        else
            dobracket(l, root);
    }

    int get(list<Token> & l, Tree *&root) {
        if (!root) {
            if (l.front().text[0] == '(') {
                l.pop_front();
                dobracket(l, root);
            } else {
                root = new Tree(l.front());
                l.pop_front();
            }
        }
        if (l.empty())
            return 1;
        if (l.front().text[0] == ')') {
            l.pop_front();
            return 1;
        }
        return 0;
    }

};

int main() {
    stringstream ss;
//    ss << "1 + ( ( 4 - 1 ) + 1 ) * 2 + 1 - 1";
    ss << "( ( ( 1 + 2 ) ) ) * ( ( ( 3 - 1 ) ) )";
    list<Token> l = Token::tokenize(ss);
    Parser parser;
    parser.parse(l);
    cout << parser.execute();

}

