
#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

class Lexer {
public:
    enum Token { NUMBER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, END };

    Lexer(const std::string& input) : text(input), pos(0) {
        currentChar = text[pos];
    }

    void advance() {
        pos++;
        if (pos < text.size())
            currentChar = text[pos];
        else
            currentChar = ' ';
    }

    void skipWhitespace() {
        while (currentChar != ' ' && isspace(currentChar))
            advance();
    }

    int integer() {
        int result = 0;
        while (currentChar != ' ' && isdigit(currentChar)) {
            result = result * 10 + (currentChar - '0');
            advance();
        }
        return result;
    }

    Token getNextToken() {
        while (currentChar != ' ') {
            if (isspace(currentChar)) {
                skipWhitespace();
                continue;
            }
            if (isdigit(currentChar)) {
                currentNumber = integer();
                return NUMBER;
            }
            if (currentChar == '+') {
                advance();
                return PLUS;
            }
            if (currentChar == '-') {
                advance();
                return MINUS;
            }
            if (currentChar == '*') {
                advance();
                return MUL;
            }
            if (currentChar == '/') {
                advance();
                return DIV;
            }
            if (currentChar == '(') {
                advance();
                return LPAREN;
            }
            if (currentChar == ')') {
                advance();
                return RPAREN;
            }
            throw std::runtime_error("Invalid character");
        }
        return END;
    }

    int currentNumber;

private:
    std::string text;
    size_t pos;
    char currentChar;
};

class Parser {
public:
    Parser(Lexer& lexer) : lexer(lexer) {
        currentToken = lexer.getNextToken();
    }

    int parse() {
        return expr();
    }

private:
    Lexer& lexer;
    Lexer::Token currentToken;

    void eat(Lexer::Token token) {
        if (currentToken == token)
            currentToken = lexer.getNextToken();
        else
            throw std::runtime_error("Syntax error");
    }

    int factor() {
        if (currentToken == Lexer::NUMBER) {
            int value = lexer.currentNumber;
            eat(Lexer::NUMBER);
            return value;
        } else if (currentToken == Lexer::LPAREN) {
            eat(Lexer::LPAREN);
            int result = expr();
            eat(Lexer::RPAREN);
            return result;
        }
        throw std::runtime_error("Syntax error in factor");
    }

    int term() {
        int result = factor();
        while (currentToken == Lexer::MUL || currentToken == Lexer::DIV) {
            if (currentToken == Lexer::MUL) {
                eat(Lexer::MUL);
                result *= factor();
            } else if (currentToken == Lexer::DIV) {
                eat(Lexer::DIV);
                int divisor = factor();
                if (divisor == 0)
                    throw std::runtime_error("Division by zero");
                result /= divisor;
            }
        }
        return result;
    }

    int expr() {
        int result = term();
        while (currentToken == Lexer::PLUS || currentToken == Lexer::MINUS) {
            if (currentToken == Lexer::PLUS) {
                eat(Lexer::PLUS);
                result += term();
            } else if (currentToken == Lexer::MINUS) {
                eat(Lexer::MINUS);
                result -= term();
            }
        }
        return result;
    }
};

int main() {
    std::string input;
    std::cout << "Enter an arithmetic expression: ";
    std::getline(std::cin, input);

    try {
        Lexer lexer(input);
        Parser parser(lexer);
        int result = parser.parse();
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
