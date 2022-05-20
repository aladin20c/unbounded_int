//> Scanning on Demand scanner-c
#ifndef scanner_c
#define scanner_c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//______________________________________________________________________________________

typedef enum {
  TOKEN_MINUS, TOKEN_PLUS,TOKEN_SLASH, TOKEN_PERC ,TOKEN_STAR,TOKEN_EQUAL,
  TOKEN_IDENTIFIER,TOKEN_NUMBER,
  TOKEN_PRINT,
  TOKEN_ERROR, TOKEN_EOF
} TokenType;


typedef struct {
  TokenType type;
  const char* start;
  int length;
  int line;
} Token;


void initScanner(const char* source);
void print_token(Token t);
Token scanToken();

//______________________________________________________________________________________


/*initialising scanner*/
typedef struct {
  const char* start;
  const char* current;
  int line;
} Scanner;


Scanner scanner;


void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}


void print_token(Token t){
  printf("Type: ");
  switch (t.type) {
    case 0:printf("%s\n","minus" );break;
    case 1:printf("%s\n","plus" );break;
    case 2:printf("%s\n", "slash");break;
    case 3:printf("%s\n", "perc");break;
    case 4:printf("%s\n", "star");break;
    case 5:printf("%s\n", "equal");break;
    case 6:printf("%s\n","ident" );break;
    case 7:printf("%s\n","number" );break;
    case 8:printf("%s\n", "print");break;
    case 9:printf("%s\n", "Error");break;
    case 10:printf("%s\n", "EOF");break;
  }
  printf("Length: %d\n", t.length);
  printf("Line: %d\n", t.line);
  for(int i=0;i<t.length;i++){
    printf("%c", t.start[i]);
  }
  printf("\n-----------------------------------------------------------------------\n");
}


/*aux functions*/



static int isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static int isDigit(char c) {
  return c >= '0' && c <= '9';
}

static int isAtEnd() {
  return *scanner.current == '\0';
}

static char advance() {
  scanner.current++;
  return scanner.current[-1];
}


static char peek() {
  return *scanner.current;
}


static char peekNext() {
  if (isAtEnd()) return '\0';
  return scanner.current[1];
}


static int match(char expected) {
  if (isAtEnd()) return 0;
  if (*scanner.current != expected) return 0;
  scanner.current++;
  return 1;
}


static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
}


static Token errorToken() {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = scanner.current;
  token.length = 1;
  token.line = scanner.line;
  return token;
}


static void skipWhitespace() {
  for (;;) {
    char c = peek();
    switch (c) {
      case ' ':advance();break;
      case '\r':advance();break;
      case '\t':advance();break;
      case '\n':scanner.line++;advance();break;
      default: return;
    }
  }
}


static TokenType checkKeyword(int start, int length,const char* rest, TokenType type) {
  if (scanner.current - scanner.start == start + length &&
      memcmp(scanner.start + start, rest, length) == 0) {
    return type;
  }
  return TOKEN_IDENTIFIER;
}

static Token identifier() {
  while (isAlpha(peek()) || isDigit(peek())) advance();
  TokenType type=TOKEN_IDENTIFIER;
  if(scanner.start[0]=='p'){
    type=checkKeyword(1, 4, "rint", TOKEN_PRINT);
  }
  return makeToken(type);
}



static Token number() {
  while (isDigit(peek())) advance();
  return makeToken(TOKEN_NUMBER);
}


Token scanToken() {
  skipWhitespace();
  scanner.start = scanner.current;
  if (isAtEnd()) return makeToken(TOKEN_EOF);
  char c = advance();
  if (isAlpha(c)) return identifier();
  if (isDigit(c)) return number();
  switch (c) {
    case '=':return makeToken(TOKEN_EQUAL);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);
  }
  return errorToken();
}

#endif
