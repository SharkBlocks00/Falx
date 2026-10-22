//
// Created by SharkBlocks00 on 10/17/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "Interpreter.h"


void Interpreter::execute(const std::vector<std::shared_ptr<Statement> > &program, Environment &environment) const {
    for (const auto& statement : program) {
        statement->execute(*this, environment);
    }
}

void Interpreter::interpret(const std::vector<std::shared_ptr<Statement> > &program) {
    this->execute(program, this->m_environment);
}
