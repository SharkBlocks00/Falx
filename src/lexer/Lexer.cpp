//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.

//

#include "Lexer.h"

std::map<std::string, TokenKind> Lexer::_initKeywords() {
    std::map<std::string, TokenKind> keywords = {
        { "let", TokenKind::LET},
        {"const", TokenKind::CONST},
        {"if", TokenKind::IF},
        {"else", TokenKind::ELSE},
        {"elseif", TokenKind::ELSEIF},
        {"while", TokenKind::WHILE},
        {"foreach", TokenKind::FOREACH},
        {"return", TokenKind::RETURN},
        {"break", TokenKind::BREAK},
        {"continue", TokenKind::CONTINUE},
        {"define", TokenKind::DEFINE},
        {"func", TokenKind::FUNC},
        {"struct", TokenKind::STRUCT},
        {"null", TokenKind::NULL_LITERAL},
        {"throw", TokenKind::THROW},
        {"export", TokenKind::EXPORT}
    };

    return keywords;
}

Lexer::Lexer(std::string source, std::string filename)
    : m_source(std::move(source)), KEYWORDS(_initKeywords()), m_filename(std::move(filename)) {}

std::vector<Token> Lexer::lex() {
    while (!this->_isAtEnd()) {
        this->m_start = this->m_current;
        this->_scanToken();
    }

    this->m_tokens.emplace_back(TokenKind::END_OF_FILE, "", std::monostate(),
        SourceLocation(this->m_filename, this->m_line, this->m_column)
    );
    return this->m_tokens;
}

void Lexer::_scanToken() {
    this->m_startColumn = this->m_column;
    const char c = this->_advance();

    switch (c) {
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n': {
            this->m_line++;
            this->m_column = 1;
            break;
        }
        case '(': this->_addToken(TokenKind::LEFT_PAREN); break;
        case ')': this->_addToken(TokenKind::RIGHT_PAREN); break;
        case '[': this->_addToken(TokenKind::LEFT_BRACKET); break;
        case ']': this->_addToken(TokenKind::RIGHT_BRACKET); break;
        case '{': this->_addToken(TokenKind::LEFT_BRACE); break;
        case '}': this->_addToken(TokenKind::RIGHT_BRACE); break;
        case ',': this->_addToken(TokenKind::COMMA); break;
        case '.': this->_addToken(TokenKind::DOT); break;
        case ':': this->_addToken(TokenKind::COLON); break;
        case ';': this->_addToken(TokenKind::SEMICOLON); break;
        case '%': {
            if (this->_match('=')) {
                this->_addToken(TokenKind::PERCENT_EQUAL);
            } else {
                this->_addToken(TokenKind::PERCENT);
            }
            break;
        }
        case '!': {
            if (this->_match('=')) {
                this->_addToken(TokenKind::BANG_EQUAL);
            } else {
                this->_addToken(TokenKind::BANG);
            }
            break;
        }
        case '+': {
            if (this->_match('=')) {
                this->_addToken(TokenKind::PLUS_EQUAL);
            } else if (this->_match('+')) {
                this->_addToken(TokenKind::PLUS_PLUS);
            } else {
                this->_addToken(TokenKind::PLUS);
            }
            break;
        }
        case '-': {
            if (this->_match('=')) {
                this->_addToken(TokenKind::MINUS_EQUAL);
            } else if (this->_match('-')) {
                this->_addToken(TokenKind::MINUS_MINUS);
            } else {
                this->_addToken(TokenKind::MINUS);
            }
            break;
        }
        case '*': {
            if (this->_match('=')) {
                this->_addToken(TokenKind::STAR_EQUAL);
            } else if (this->_match('*')) {
                this->_addToken(TokenKind::STAR_STAR);
            } else {
                this->_addToken(TokenKind::STAR);
            }
            break;
        }
        case '/': {
            if (this->_match('/')) {
                // comments
                while (this->_peek() != '\n' && !this->_isAtEnd()) {
                    static_cast<void>(this->_advance());
                };
            } else if (this->_match('=')) {
                this->_addToken(TokenKind::SLASH_EQUAL);
            } else {
                this->_addToken(TokenKind::SLASH);
            }
            break;
        }
        case '=': this->_addToken(this->_match('=')? TokenKind::EQUAL_EQUAL : TokenKind::EQUAL); break;
        case '"': this->_string(); break;
        case '>': this->_addToken(this->_match('=')? TokenKind::GREATER_EQUAL : TokenKind::GREATER); break;
        case '<': this->_addToken(this->_match('=')? TokenKind::LESS_EQUAL : TokenKind::LESS); break;
        case '|': this->_addToken(this->_match('|')? TokenKind::OR : TokenKind::PIPE); break;
        case '&': this->_addToken(this->_match('&')? TokenKind::OR : throw std::exception()); break;
        case '?': this->_addToken(TokenKind::QUESTION);
        default: {
            if (std::isdigit(c) && isascii(c)) {
                this->_number();
            }
            else if (std::isalpha(c)) {
                this->_identifier();
            } else {
                throw std::exception();
            }
        }
    }
}

void Lexer::_string() {
    std::string value;
    this->m_startColumn = this->m_column;

    if (this->_isAtEnd()) {
        throw std::exception();
    }

    while (!this->_isAtEnd()) {
        const char c{this->_peek()};

        if (c == '"') {
            static_cast<void>(this->_advance());
            this->_addToken(TokenKind::STRING, value);
        }
        if (c == '\n') {
            this->m_line++;
            this->m_column = 1;
            value += this->_advance();
            continue;
        }
        if (c == '\\') {
            static_cast<void>(this->_advance());

            switch (this->_advance()) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case 'f': value += '\f'; break;
                case 'b': value += '\b'; break;
                case '"': value += '"'; break;
                case '\\': value += '\\'; break;
                default: throw std::exception();
            }
        } else {
            value += this->_advance();
        }
        if (this->_isAtEnd()) {
            throw std::exception();
        }
    }
}

void Lexer::_number() {
    const std::size_t start{this->m_start};

    while (!this->_isAtEnd() && std::isdigit(this->_peek()) && isascii(this->_peek())) {
        static_cast<void>(this->_advance());
    }

    if (this->_peek() == '.' && std::isdigit(this->_peekNext()) && isascii(this->_peekNext())) {
        static_cast<void>(this->_advance());
        while (!this->_isAtEnd() && std::isdigit(this->_peek()) && isascii(this->_peek())) {
            static_cast<void>(this->_advance());
        }
    }

    const std::string text{this->m_source.substr(start, this->m_current-start)};
    this->_addToken(TokenKind::NUMBER, std::stod(text));
}

void Lexer::_identifier() {
    const size_t start{this->m_start};

    while (!this->_isAtEnd() && (std::isalnum(this->_peek()) || this->_peek() == '_')) {
        static_cast<void>(this->_advance());
    }

    const std::string value{this->m_source.substr(start, this->m_current - start)};

    if (value == "true" || value == "false") {
        this->_addToken(TokenKind::BOOLEAN, value== "true");
        return;
    }

    const auto it{this->KEYWORDS.find(value)};

    const TokenKind kind = (it != KEYWORDS.end()) ? it->second : TokenKind::IDENTIFIER;
    this->_addToken(kind, value);
}

[[nodiscard]] bool Lexer::_match(const char c) {
    if (this->_isAtEnd()) {
        return false;
    }
    if (this->_peek() != c) {
        return false;
    }

    static_cast<void>(this->_advance());
    return true;
}

void Lexer::_addToken(TokenKind kind, const Literal& literal) {
    std::string lexeme{this->m_source.substr(this->m_start, this->m_current - this->m_start)};
    this->m_tokens.emplace_back(kind, lexeme, literal,
        SourceLocation(this->m_filename, this->m_line, this->m_startColumn)

    );
}

[[nodiscard]] char Lexer::_peek() const {
    if (this->_isAtEnd()) {
        return '\0';
    }
    return this->m_source[this->m_current];
}

[[nodiscard]] char Lexer::_peekNext() const {
    if (this->_isAtEnd()) {
        return '\0';
    }

    if (this->m_current + 1 >= this->m_source.length()) {
        return '\0';
    }
    return this->m_source[this->m_current];
}

[[nodiscard]] char Lexer::_advance() {
    if (this->_isAtEnd()) {
        return '\0';
    }

    const char c{this->m_source.at(this->m_current)};
    this->m_current++;
    this->m_column++;
    return c;
}

[[nodiscard]] bool Lexer::_isAtEnd() const {
    return this->m_current >= this->m_source.length();
}
