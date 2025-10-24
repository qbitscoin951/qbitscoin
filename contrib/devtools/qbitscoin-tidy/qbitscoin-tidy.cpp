// Copyright (c) 2023 Qbitscoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "logprintf.h"

#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>

class QbitscoinModule final : public clang::tidy::ClangTidyModule
{
public:
    void addCheckFactories(clang::tidy::ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<qbitscoin::LogPrintfCheck>("qbitscoin-unterminated-logprintf");
    }
};

static clang::tidy::ClangTidyModuleRegistry::Add<QbitscoinModule>
    X("qbitscoin-module", "Adds qbitscoin checks.");

volatile int QbitscoinModuleAnchorSource = 0;
