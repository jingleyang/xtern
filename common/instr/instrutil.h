/* Author: Junfeng Yang (junfeng@cs.columbia.edu) -*- Mode: C++ -*- */

#ifndef __TERN_COMMON_INSTRUTIL_H
#define __TERN_COMMON_INSTRUTIL_H

#include "llvm/Instruction.h"
#include "common/id-manager/IDManager.h"

namespace tern {

llvm::Value* getIntMetadata(const llvm::Instruction *I, const char* key);
llvm::Value* getInsID(const llvm::Instruction *I);
bool funcEscapes(llvm::Function* F);
llvm::IDManager *getIDManager(void);
void setIDManager(llvm::IDManager * IDM);

}

#endif