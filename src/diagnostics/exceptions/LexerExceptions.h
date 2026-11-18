//
// Created by SharkBlocks00 on 11/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include "FalxException.h"

class FalxLexerException : public FalxException {
    using FalxException::FalxException;
};

class InvalidCharacterException : public FalxLexerException {
    using FalxLexerException::FalxLexerException;
};

class InvalidEscapeSequenceException : public FalxLexerException {
    using FalxLexerException::FalxLexerException;
};

class UnterminatedStringException : public FalxLexerException {
public:
    explicit UnterminatedStringException(const std::optional<SourceLocation> &location = std::nullopt)
        : FalxLexerException("Unterminated string.", location) {}
};