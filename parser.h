#ifndef _script_h_
#define _script_h_ 1

class Token;
class Node;
class KernelBuilder;

class Parser {
public:
  Parser(KernelBuilder &b);
  ~Parser();

  Node *parse(Token *head);


  Token *whenz(Token *, Node *n);
  Token *keyword(const char *word, Token *t, Node *current);
  Token *conditions(Token *t, Node *current);
  Token *actions(Token *t, Node *current);
  Token *identifier(Token *t, Node *current);
  Token *globalReference(Token *t, Node *current);
  Token *nullToken(Node *current);
  Token *consumeWhitespace(Token *t);
  Token *consumeWhitespaceAndNewline(Token *t);
  Token *unknownToken(Token *t);
  Token *unexpectedToken(const char* reason, Token* t);

private:
  KernelBuilder &_b;
};

#endif
