#include "kodparser.h"

#include "ast/compound.h"
#include "ast/binOp.h"
#include "ast/unOp.h"
#include "ast/const.h"
#include "ast/name.h"
#include "ast/function.h"
#include "ast/returnStatement.h"
#include "ast/call.h"
#include "ast/whileStatement.h"
#include "ast/ifStatement.h"
#include "ast/elifStatement.h"
#include "ast/elseStatement.h"

#include <stdio.h>
#include <stdlib.h>

#define SKIPNL { while(self->token->type == TOKEN_NL) Eat(self, TOKEN_NL); }

static ast_t* Parse(parser_t* self);

static token_t* Eat(parser_t* self, tokenType_t type);

static ast_t* ParseCompound(parser_t* self);
static ast_t* ParseBlock(parser_t* self);
static ast_t* ParseStatement(parser_t* self, statementType_t stype);

static ast_t* ParseExprAssignment(parser_t* self);  // = -> += -= etc
static ast_t* ParseLogicalOr(parser_t* self);       // ||
static ast_t* ParseLogicalAnd(parser_t* self);      // &&
static ast_t* ParseBitwiseOr(parser_t* self);       // |
static ast_t* ParseBitwiseXor(parser_t* self);      // ^
static ast_t* ParseBitwiseAnd(parser_t* self);      // &
static ast_t* ParseBoolEquals(parser_t* self);      // == !=
static ast_t* ParseBoolGTLT(parser_t* self);        // < > <= >=
static ast_t* ParseBitwiseSHLR(parser_t* self);     // << >>
static ast_t* ParseAddSub(parser_t* self);          // + -
static ast_t* ParseMDR(parser_t* self);             // * / %
static ast_t* ParsePow(parser_t* self);             // **
static ast_t* ParseSecond(parser_t* self);          // + - ! ~ @ # sizeof   UNARY
static ast_t* ParseFirst(parser_t* self);           // from the right () [] . 
static ast_t* ParseFactor(parser_t* self);          // values themselves

void Parser(parser_t* self, lexer_t* lexer) {
    self->lexer = lexer;
    self->token = lexer->NextToken(lexer);

    self->Parse = Parse;
}

parser_t* newParser(lexer_t* lexer) {
    parser_t* self = (parser_t*) malloc(sizeof(parser_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    Parser(self, lexer);
    return self;
}

static ast_t* Parse(parser_t* self) {
    ast_t* root = ParseCompound(self);
    if (root)
        root->type = AST_ROOT;
    return root;
}

static token_t* Eat(parser_t* self, tokenType_t type) {
    if (self->token->type != type) {
        char* s = self->token->Str(self->token);
        if (s) {
            fprintf(stderr, "ERROR: Unexpected token %s, expected %s.\n", s, tokenTypeToStr(type));
            free(s);
        }
        return NULL;
    }

    self->token = self->lexer->NextToken(self->lexer);
    return self->token;
}

static ast_t* ParseCompound(parser_t* self) {
    astCompound_t* compound = newAstCompound();

    while (self->token->type != TOKEN_EOF) {
        ast_t* value = ParseExprAssignment(self);
        if (value->type == AST_NOOP) { free(value); continue; }
        compound->children->PushBack(compound->children, value);
    }

    return (ast_t*)compound;
}

static ast_t* ParseBlock(parser_t* self) {
    astCompound_t* compound = newAstCompound();
    Eat(self, TOKEN_LBRACE);
    SKIPNL

    while (self->token->type != TOKEN_RBRACE) {
        SKIPNL

        ast_t* value = ParseExprAssignment(self);
        if (value->type == AST_NOOP) { free(value); continue; }
        compound->children->PushBack(compound->children, value);

        SKIPNL
    }

    Eat(self, TOKEN_RBRACE);
    return (ast_t*)compound;
}

static ast_t* ParseStatement(parser_t* self, statementType_t stype) {
    switch (stype) {
        case STATEMENT_RETURN: return (ast_t*) newAstReturnStatement(ParseLogicalOr(self));

        case STATEMENT_WHILE: {
            // Parse condition
            ast_t* condition = ParseLogicalOr(self);
            // Parse body
            astCompound_t* body = (astCompound_t*)ParseBlock(self);
            return (ast_t*) newAstWhileStatement(condition, body);
        }

        case STATEMENT_IF: {
            // Parse condition
            ast_t* condition = ParseLogicalOr(self);
            // Parse body
            astCompound_t* body = (astCompound_t*)ParseBlock(self);
            return (ast_t*) newAstIfStatement(condition, body);
        }

        case STATEMENT_ELIF: {
            // Parse condition
            ast_t* condition = ParseLogicalOr(self);
            // Parse body
            astCompound_t* body = (astCompound_t*)ParseBlock(self);
            return (ast_t*) newAstElifStatement(condition, body);
        }
        
        case STATEMENT_ELSE: {
            // Parse body
            astCompound_t* body = (astCompound_t*)ParseBlock(self);
            return (ast_t*) newAstElseStatement(body);
        }

        default:
            fprintf(stderr, "ERROR: [Parser] still don't support %s\n", stype_to_str(stype));
            exit(1);
    }

    return NULL;
}

static ast_t* ParseExprAssignment(parser_t* self) {
    ast_t* left = ParseLogicalOr(self);

    // TODO: add support for ->, +=, *=, etc...
    if (self->token->type != TOKEN_EQUALS) return left;

    Eat(self, TOKEN_EQUALS);

    return (ast_t*)newAstBinOp(TOKEN_EQUALS, left, ParseLogicalOr(self));
}

static ast_t* ParseLogicalOr(parser_t* self) {
    ast_t* left = ParseLogicalAnd(self);

    if (self->token->type == TOKEN_BOOL_OR) {
        left = (ast_t*) newAstBinOp(self->token->type, left, NULL);
        Eat(self, self->token->type);

        ((astBinOp_t*)left)->second = ParseLogicalOr(self);
    }

    return left;
}

static ast_t* ParseLogicalAnd(parser_t* self) {
    ast_t* left = ParseBitwiseOr(self);

    if (self->token->type == TOKEN_BOOL_AND) {
        left = (ast_t*) newAstBinOp(self->token->type, left, NULL);
        Eat(self, self->token->type);

        ((astBinOp_t*)left)->second = ParseLogicalAnd(self);
    }

    return left;
}

static ast_t* ParseBitwiseOr(parser_t* self) {
    ast_t* left = ParseBitwiseXor(self);

    if (self->token->type == TOKEN_OR) {
        left = (ast_t*) newAstBinOp(self->token->type, left, NULL);
        Eat(self, self->token->type);

        ((astBinOp_t*)left)->second = ParseBitwiseOr(self);
    }

    return left;
}

static ast_t* ParseBitwiseXor(parser_t* self) {
    ast_t* left = ParseBitwiseAnd(self);

    if (self->token->type == TOKEN_HAT) {
        left = (ast_t*) newAstBinOp(self->token->type, left, NULL);
        Eat(self, self->token->type);

        ((astBinOp_t*)left)->second = ParseBitwiseXor(self);
    }

    return left;
}

static ast_t* ParseBitwiseAnd(parser_t* self) {
    ast_t* left = ParseBoolEquals(self);

    if (self->token->type == TOKEN_AND) {
        Eat(self, TOKEN_AND);

        left = (ast_t*) newAstBinOp(TOKEN_AND, left, ParseBitwiseAnd(self));
    }

    return left;
}   

static ast_t* ParseBoolEquals(parser_t* self) {
    ast_t* left = ParseBoolGTLT(self);

    if (
        self->token->type == TOKEN_BOOL_EQ ||
        self->token->type == TOKEN_BOOL_NOTE
    ) {
        left = (ast_t*) newAstBinOp(self->token->type, left, NULL);
        Eat(self, self->token->type);

        ((astBinOp_t*)left)->second = ParseBoolEquals(self);
    }

    return left;
}   

static ast_t* ParseBoolGTLT(parser_t* self) {
    ast_t* left = ParseBitwiseSHLR(self);

    if (
        self->token->type == TOKEN_BOOL_GT ||
        self->token->type == TOKEN_BOOL_LT ||
        self->token->type == TOKEN_BOOL_GTE ||
        self->token->type == TOKEN_BOOL_LTE
    ) {
        left = (ast_t*) newAstBinOp(self->token->type, left, NULL);
        Eat(self, self->token->type);

        ((astBinOp_t*)left)->second = ParseBoolGTLT(self);
    }

    return left;
}   

static ast_t* ParseBitwiseSHLR(parser_t* self) {
    ast_t* left = ParseAddSub(self);

    if (
        self->token->type == TOKEN_SHL ||
        self->token->type == TOKEN_SHR
    ) {
        left = (ast_t*) newAstBinOp(self->token->type, left, NULL);
        Eat(self, self->token->type);

        ((astBinOp_t*)left)->second = ParseBitwiseSHLR(self);
    }

    return left;
}   

static ast_t* ParseAddSub(parser_t* self) {
    ast_t* left = ParseMDR(self);

    if (
        self->token->type == TOKEN_ADD ||
        self->token->type == TOKEN_SUB
    ) {
        left = (ast_t*) newAstBinOp(self->token->type, left, NULL);
        Eat(self, self->token->type);

        ((astBinOp_t*)left)->second = ParseAddSub(self);
    }

    return left;
}   

static ast_t* ParseMDR(parser_t* self) {
    ast_t* left = ParsePow(self);

    if (
        self->token->type == TOKEN_MUL ||
        self->token->type == TOKEN_DIV ||
        self->token->type == TOKEN_MOD
    ) {
        left = (ast_t*) newAstBinOp(self->token->type, left, NULL);
        Eat(self, self->token->type);

        ((astBinOp_t*)left)->second = ParseMDR(self);
    }

    return left;
}

static ast_t* ParsePow(parser_t* self) {
    ast_t* left = ParseSecond(self);

    if (self->token->type == TOKEN_POW) {
        Eat(self, TOKEN_POW);

        left = (ast_t*) newAstBinOp(TOKEN_POW, left, ParsePow(self));
    }

    return left;
}

static ast_t* ParseSecond(parser_t* self) {
    // Unary stuff like sizeof, +, -, @, #, ~, !
    astUnOp_t* ast = NULL;

    if (
        self->token->type == TOKEN_SIZEOF ||
        self->token->type == TOKEN_ADD ||
        self->token->type == TOKEN_SUB ||
        self->token->type == TOKEN_NOT ||
        self->token->type == TOKEN_BOOL_NOT
    ) {
        ast = newAstUnOp(self->token->type, NULL);
        Eat(self, self->token->type);
        
        ast->first = ParseSecond(self);
    } else {
        return ParseFirst(self);
    }

    return (ast_t*) ast;
}

static ast_t* ParseFirst(parser_t* self) {
    // a[] a() a.
    ast_t* value = ParseFactor(self);
    if (value->type == AST_NOOP) return value;
    
    switch (self->token->type) {
        case TOKEN_LPAREN: {
            ast_t* ast = ParseFactor(self);
            if (ast->type == AST_FUNCTION) {
                if (value->type != AST_NAME) {
                    fputs("[Parser]: invalid syntax for declaring a function", stderr);
                    exit(1);
                }
                ((astFunction_t*)ast)->name = ((astName_t*)value)->name;
                free(value);
                return ast;
            }

            if (ast->type != AST_COMPOUND) {
                printf("[Parser]: TODO: what are u calling lol\n");
                exit(1);
            }
            
            return (ast_t*) newAstCall(value, (astCompound_t*) ast);
            puts("ERROR");
        }

        case TOKEN_LBRACKET: {
            // TODO: array subscript
            break;
        }

        case TOKEN_DOT: {
            // TODO: struct access
            break;
        }

        default: break;
    }
    
    return value;
}

static ast_t* ParseFactor(parser_t* self) {
    ast_t* ast = NULL;

    switch (self->token->type) {
        case TOKEN_INT:     ast = (ast_t*) newAstConst(AST_INT,     self->token->value); break;
        case TOKEN_FLOAT:   ast = (ast_t*) newAstConst(AST_FLOAT,   self->token->value); break;
        case TOKEN_STRING:  ast = (ast_t*) newAstConst(AST_STRING,  self->token->value); break;
        
        case TOKEN_ID: {
            // there is a chance that this is a keyword
            
            statementType_t stype = str_to_stype(self->token->value);
            if (stype != STATEMENT_UNKNOWN) {
                // this is a keyword
                Eat(self, self->token->type);
                return ParseStatement(self, stype);
            }

            ast = (ast_t*) newAstName(self->token->value);
            break;
        }

        case TOKEN_LPAREN: { // () {}, (1, 2, 3)
            Eat(self, TOKEN_LPAREN);
            // ()
            // (expression) == expression
            // (expression,) == expression,
            // (expression, expression) == expression, expression
            // () {}
            astCompound_t* compound = newAstCompound();
            
            SKIPNL

            if (self->token->type != TOKEN_RPAREN) {
                ast_t* value = ParseExprAssignment(self);
                if (value->type != AST_NOOP)
                    compound->children->PushBack(compound->children, value);
                else free(value);
            }

            SKIPNL
            while (self->token->type == TOKEN_COMMA) {
                Eat(self, TOKEN_COMMA);
                SKIPNL

                ast_t* value = ParseExprAssignment(self);
                if (value->type != AST_NOOP)
                    compound->children->PushBack(compound->children, value);
                else free(value);

                SKIPNL
            }
            
            Eat(self, TOKEN_RPAREN);

            SKIPNL
            if (self->token->type == TOKEN_LBRACE) {
                // todo: check for invalid parameters

                ast_t* body = ParseBlock(self);
                return (ast_t*) newAstFunction(NULL, compound, (astCompound_t*)body);
            }

            return (ast_t*) compound;
            break;
        }

        default: ast = newAst(AST_NOOP); break;
    }
    
    Eat(self, self->token->type);
    return ast;
}
