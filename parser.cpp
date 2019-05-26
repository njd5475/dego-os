#include "new.h"
#include "string_functions.h"
#include "parser.h"
#include "terminal.h"
#include "kernel_builder.h"
#include "token.h"
#include "parse_node.h"

/* Start of Parser */
Parser::Parser(KernelBuilder &b) : _b(b) {}
Parser::~Parser() {}

Node *Parser::parse(Token *head) {
  Node *n = new Node();
  while(head != NULL) {
    head = whenz(head, n);
  }
  return n;
}

Token *Parser::keyword(const char *word, Token *t, Node *n) {
  t = consumeWhitespace(t);
  if(strcmp(word, t->tok())) {
    //_b.putNumber((unsigned char)(t->tok()[1]+5), 15, (unsigned short)5);
    n->buildChild(word);
    return t->next();
  }else{
    _b.put("Expected '");
    _b.put(word);
    _b.put("' got ");
    t->print(_b);
    return NULL;
  }
}

Token *Parser::identifier(Token *t, Node *n) {
  if(t->ttype() == LETTER || t->ttype() == NUMBER) {
    Node *ident = n->buildChild("Identifier");
    ident->buildChild("Part", t);
    t = t->next();
    while(t->ttype() == LETTER || t->ttype() == NUMBER) {
      t = t->next();
      ident->buildChild("Part", t);
    }

    return t;
  }
  
  return NULL;
}

Token *Parser::globalReference(Token *t, Node *n) {
  t = consumeWhitespace(t);
  if(t->ttype() == AT_SYMBOL) {
    t = t->next(); //consume at symbol

    n = n->buildChild("GlobalReference");
    
    t = identifier(t, n); // consume identifier
    if(t) {
      Token *goodSave = t;
      while(t->ttype() == DOT || t->ttype() == LETTER || t->ttype() == NUMBER) {
        if(t->ttype() == DOT) {
          t = t->next();
        }

        if(t->ttype() == LETTER || t->ttype() == NUMBER) {
          t = identifier(t, n);

          if(t) {
            goodSave = t;
          }
        }else{
          t = goodSave;
          break;
        }
      }
      return t;
    }else{
      return unexpectedToken("Expected identifier but got ", t);
    }
  }
  return NULL;
}

Token *Parser::consumeWhitespace(Token *t) {
  while(t->ttype() == WHITESPACE) {
    t = t->next();
  }
  return t;
}

Token *Parser::consumeWhitespaceAndNewline(Token *t) {
  while(t->ttype() == WHITESPACE || t->ttype() == NEWLINE) {
    t = t->next();
  }
  return t;
}

Token *Parser::unknownToken(Token *t) {
  _b.put("Unknown Token:");
  t->print(_b);
  return NULL;
}

Token *Parser::unexpectedToken(const char *reason, Token *t) {
  _b.put(reason);
  t->print(_b);
  return NULL;
}

Token *Parser::nullToken(Node *current) {
  _b.put("Unexpected NULL token!");
  return NULL;
}

Token *Parser::conditions(Token *t, Node *n) {
  Node *node = new Node("conditions");
  Token *back = t;
  t = consumeWhitespace(t);
  t = identifier(t, node);
  if(t) {
    n->addChild(node);
    return t;
  }
  
  t = back; //rewind to try a different node
  t = globalReference(t, node);
  if(t) {
    n->addChild(node);
    return t;
  }

  return NULL;
}

Token *Parser::actions(Token *t, Node *n) {
  t = consumeWhitespace(t);
  n = n->buildChild("action");
  t = keyword("print", t, n);
  if(t) {
    Node *child = n->buildChild("printCommand");
    t = consumeWhitespace(t);
    while(t->ttype() != NEWLINE) {
      child->buildChild("PrintPart", t);
      t = t->next();
    }
  }else{
    _b.put("Expected keyword got ");
    t->print(_b);
  }
  return t;
}

Token *Parser::whenz(Token *tok, Node *current) {
  tok = consumeWhitespaceAndNewline(tok);
  tok = keyword("when", tok, current);
  if(!tok) return nullToken(current);
  tok = conditions(tok, current);
  if(!tok) return NULL;
  tok = consumeWhitespace(tok);
  Token *beforeDo = tok;
  tok = keyword("do", tok, current);
  if(!tok) {
    return unexpectedToken("Expected do but got ", beforeDo);
  }
  tok = consumeWhitespace(tok);
  if(tok->ttype() == NEWLINE) tok = tok->next();
  tok = actions(tok, current);
  if(!tok) return NULL;
  return tok;
}
