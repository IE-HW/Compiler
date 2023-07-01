#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// تعریف ساختارهای AST
typedef struct {
    int value;
} NumberNode;

typedef struct {
    char name[20];
} VariableNode;

typedef struct {
    char op;
    struct ExprNode* left;
    struct ExprNode* right;
} BinaryOpNode;

typedef struct ExprNode {
    enum { NUMBER, VARIABLE, BINARY_OP } type;
    union {
        NumberNode number;
        VariableNode variable;
        BinaryOpNode binaryOp;
    };
} ExprNode;

// تعریف توکن‌ها
typedef enum {
    TOKEN_NUMBER,
    TOKEN_VARIABLE,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    union {
        int number;
        char variable[20];
        char op;
    };
} Token;

// تعریف متغیرهای گلوبال برای پارسر
Token currentToken;
char input[100];
int position;

// تابع خطا
void error(const char* message) {
    printf("Error: %s\n", message);
    exit(1);
}

// تابع بررسی نوع توکن فعلی
bool match(TokenType expectedType) {
    return currentToken.type == expectedType;
}

// تابع گرفتن توکن بعدی
void getNextToken() {
    // صرف نظر از توکن‌های فاصله و علامت سمی‌کالن
    while (input[position] == ' ' || input[position] == ';') {
        position++;
    }

    // بررسی پایان ورودی
    if (input[position] == '\0') {
        currentToken.type = TOKEN_EOF;
        return;
    }

    // تشخیص نوع توکن و استخراج اطلاعات
    if (isdigit(input[position])) {
        currentToken.type = TOKEN_NUMBER;
        currentToken.number = atoi(&input[position]);
        while (isdigit(input[position])) {
            position++;
        }
    } else if (isalpha(input[position])) {
        currentToken.type = TOKEN_VARIABLE;
        int i = 0;
        while (isalnum(input[position])) {
            currentToken.variable[i++] = input[position++];
        }
        currentToken.variable[i] = '\0';
    } else {
        currentToken.type = TOKEN_PLUS;
        currentToken.op = input[position++];
    }
}

// تابع ساخت یک نود عددی
ExprNode* makeNumberNode(int value) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    node->type = NUMBER;
    node->number.value = value;
    return node;
}

// تابع ساخت یک نود متغیر
ExprNode* makeVariableNode(const char* name) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    node->type = VARIABLE;
    strcpy(node->variable.name, name);
    return node;
}

// تابع ساخت یک نود عملگر دومینه
ExprNode* makeBinaryOpNode(char op, ExprNode* left, ExprNode* right) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    node->type = BINARY_OP;
    node->binaryOp.op = op;
    node->binaryOp.left = left;
    node->binaryOp.right = right;
    return node;
}

// تابع پارس کردن عبارت اولیه
ExprNode* parseExpression();

// تابع پارس کردن عبارت داخل پرانتز
ExprNode* parseParenthesesExpression() {
    getNextToken();
    ExprNode* expr = parseExpression();
    if (!match(TOKEN_SEMICOLON)) {
        error("Expected ';'");
    }
    return expr;
}

// تابع پارس کردن عبارت عددی یا متغیر
ExprNode* parseAtomicExpression() {
    if (match(TOKEN_NUMBER)) {
        ExprNode* node = makeNumberNode(currentToken.number);
        getNextToken();
        return node;
    } else if (match(TOKEN_VARIABLE)) {
        ExprNode* node = makeVariableNode(currentToken.variable);
        getNextToken();
        return node;
    } else if (match(TOKEN_SEMICOLON)) {
        getNextToken();
        return NULL;
    } else if (match(TOKEN_EOF)) {
        return NULL;
    } else {
        error("Invalid expression");
        return NULL;
    }
}

// تابع پارس کردن عبارت
ExprNode* parseExpression() {
    ExprNode* left = parseAtomicExpression();

    while (match(TOKEN_PLUS) || match(TOKEN_MINUS) || match(TOKEN_MULTIPLY) || match(TOKEN_DIVIDE) || match(TOKEN_ASSIGN)) {
        char op = currentToken.op;
        getNextToken();
        ExprNode* right = parseAtomicExpression();
        left = makeBinaryOpNode(op, left, right);
    }

    return left;
}

// تابع پارس کردن برنامه
void parseProgram() {
    getNextToken();
    while (!match(TOKEN_EOF)) {
        ExprNode* expression = parseParenthesesExpression();

        // انجام عملیات معنایی
        if (expression != NULL) {
            printf("Expression: ");

            switch (expression->type) {
                case NUMBER:
                    printf("%d\n", expression->number.value);
                    break;
                case VARIABLE:
                    printf("%s\n", expression->variable.name);
                    break;
                case BINARY_OP:
                    printf("( ");
                    switch (expression->binaryOp.op) {
                        case '+':
                            printf("+");
                            break;
                        case '-':
                            printf("-");
                            break;
                        case '*':
                            printf("*");
                            break;
                        case '/':
                            printf("/");
                            break;
                        case '=':
                            printf("=");
                            break;
                    }
                    printf(" )\n");
                    break;
            }
        }
    }
}

int main() {
    strcpy(input, "a = 5;");
    position = 0;

    parseProgram();

    return 0;
}
