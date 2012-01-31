#include "util.h"
#include "path-slicer.h"
#include "oprd-summ.h"
using namespace tern;
char tern::OprdSumm::ID = 0;

using namespace llvm;

OprdSumm::OprdSumm(): CallGraphFP(&ID) {

}

OprdSumm::~OprdSumm() {

}

void OprdSumm::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
  CallGraphFP::getAnalysisUsage(AU);
}

bool OprdSumm::runOnModule(Module &M) {
  CallGraphFP::runOnModule(M);
  collectSummLocal(M);
  collectSummTopDown(M);
  return false;
}

void OprdSumm::initStat(Stat *stat) {
  this->stat = stat;
}

llvm::DenseSet<llvm::Instruction *> *OprdSumm::getLoadSummBetween(
      DynInstr *prevInstr, DynInstr *postInstr, bdd &bddResults) {
  return NULL;
}

llvm::DenseSet<llvm::Instruction *> *OprdSumm::getStoreSummBetween(
      DynInstr *prevInstr, DynInstr *postInstr, bdd &bddResults) {
  return NULL;
}

llvm::DenseSet<llvm::Instruction *> *OprdSumm::getStoreSummInFunc(
      DynCallInstr *callInstr, bdd &bddResults) {
  return NULL;
}

void OprdSumm::initAllSumm(llvm::Module &M) {
  for (Module::iterator fi = M.begin(); fi != M.end(); ++fi) {
    funcLoadSumm[fi] = funcStoreSumm[fi] = NULL;
    for (Function::iterator bi = fi->begin(); bi != fi->end(); ++bi) {
      bbLoadSumm[bi] = bbStoreSumm[bi] = NULL;
    }
  }
}

void OprdSumm::collectSummLocal(llvm::Module &M) {
  visited.clear();
  for (Module::iterator f = M.begin(); f != M.end(); ++f) {
    if (visited.count(f) == 0) {
      visited.insert(f);
      if (pathSlicer->isInternalFunction(f)) {
        funcLoadSumm[f] = new InstrDenseSet;
        funcStoreSumm[f] = new InstrDenseSet;
        collectFuncSummLocal(f);
      }
    }
  }
}

void OprdSumm::collectFuncSummLocal(const llvm::Function *f) {
  for (Function::const_iterator bi = f->begin(); bi != f->end(); ++bi) {
    bbLoadSumm[bi] = new InstrDenseSet;
    bbStoreSumm[bi] = new InstrDenseSet;
    for (BasicBlock::const_iterator ii = bi->begin(); ii != bi->end(); ++ii) {
      collectInstrSummLocal(ii);
    }
  } 
}

void OprdSumm::collectInstrSummLocal(const llvm::Instruction *instr) {
  const Function *f = Util::getFunction(instr);
  const BasicBlock *bb = Util::getBasicBlock(instr);
  if (instr->getOpcode() == Instruction::Load) {
    bbLoadSumm[bb]->insert(instr);
    funcLoadSumm[f]->insert(instr);
  } else if (instr->getOpcode() == Instruction::Store) {
    bbStoreSumm[bb]->insert(instr);
    funcStoreSumm[f]->insert(instr);
  }
}

void OprdSumm::collectSummTopDown(llvm::Module &M) {
  visited.clear();
  for (Module::iterator f = M.begin(); f != M.end(); ++f) {
    if (visited.count(f) == 0) {
      visited.insert(f);
      if (pathSlicer->isInternalFunction(f)) {
        DFSTopDown(f);
      }
    }
  }
}

void OprdSumm::DFSTopDown(const llvm::Function *f) {
  for (Function::const_iterator b = f->begin(), be = f->end(); b != be; ++b) {
    for (BasicBlock::const_iterator i = b->begin(), ie = b->end(); i != ie; ++i) {
      if (Util::isCall(i) && pathSlicer->isInternalCall(i)) {
        vector<Function *> calledFuncs = get_called_functions(i);//Inheritated from CallGraphFP
        for (size_t j = 0; j < calledFuncs.size(); ++j) {
          const Function *callee = calledFuncs[j];

          // First, do DFS to collect summary.
          if (visited.count(callee) == 0) {
            visited.insert(callee);
            if (pathSlicer->isInternalFunction(callee)) {
              DFSTopDown(callee);
            }
          }

          // Add the collected summary to caller.
          addSummToCallerTopDown(callee, f, i);
        }
      }
    }
  }

}

void OprdSumm::addSummToCallerTopDown(const llvm::Function *callee,
  const llvm::Function *caller, const Instruction *callInstr) {
  // Add function load summary from callee to caller.
  if (funcLoadSumm[callee]) {
    if (!funcLoadSumm[caller])
      funcLoadSumm[caller] = new InstrDenseSet;
    addSummTopDown(funcLoadSumm[callee], funcLoadSumm[caller]);
  }

  // Add function store summary from callee to caller.
  if (funcStoreSumm[callee]) {
    if (!funcStoreSumm[caller])
      funcStoreSumm[caller] = new InstrDenseSet;
    addSummTopDown(funcStoreSumm[callee], funcStoreSumm[caller]);
  }

  // Add function load summary from callee to the bb containing the call instruction.
  const BasicBlock *bb = Util::getBasicBlock(callInstr);
  if (funcLoadSumm[callee]) {
    if (!bbLoadSumm[bb])
      bbLoadSumm[bb] = new InstrDenseSet;
    addSummTopDown(funcLoadSumm[callee], bbLoadSumm[bb]);
  }

  // Add function store summary from callee to the bb containing the call instruction.
  if (funcStoreSumm[callee]) {
    if (!bbStoreSumm[bb])
      bbStoreSumm[bb] = new InstrDenseSet;
    addSummTopDown(funcStoreSumm[callee], bbStoreSumm[bb]);
  }
}

void OprdSumm::addSummTopDown(InstrDenseSet *calleeSet,
  InstrDenseSet *callerSet) {
  InstrDenseSet::iterator itr(calleeSet->begin());
  for (; itr != calleeSet->end(); ++itr)
    callerSet->insert(*itr);
}
