#include "event-funcs.h"
#include "util.h"
#include "event-mgr.h"
#include "tern/syncfuncs.h"
using namespace tern;

#include "common/util.h"
#include "common/callgraph-fp.h"
#include "llvm/Support/CommandLine.h"
using namespace llvm;

#include "klee/BasicCheckers.h"
using namespace klee;

using namespace std;

char tern::EventMgr::ID = 0;
extern cl::opt<std::string> UseOneChecker;

namespace {
  static RegisterPass<tern::EventMgr> X(
    "event-manager",
    "Get functions with event operations",
    false,
    true); // is analysis
}

EventMgr::EventMgr(): ModulePass(&ID) {
  fprintf(stderr, "EventMgr::EventMgr UseOneChecker (%s)\n", UseOneChecker.c_str());
}


EventMgr::~EventMgr() {
  if (DBG)
    fprintf(stderr, "EventMgr::~EventMgr\n");
}

void EventMgr::clean() {
  //fprintf(stderr, "EventMgr::clean\n");
  CallGraphFP &CG = getAnalysis<CallGraphFP>();
  CG.destroy();
  /* We have to destroy the CallGraph here, since when uclibc is linking in, 
  LLVM would remove some functions in original modules and link in ones in 
  uclibc, and the removal would cause crash if we do not free the CallGraph 
  before hand. But there is no problem because callgraph-fp maintains callsites
  independently. */
}

void EventMgr::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
  AU.addRequired<CallGraphFP>();
  ModulePass::getAnalysisUsage(AU);
}

void EventMgr::setupEvents(Module &M) {
  Checker *checker = NULL;
  if (UseOneChecker == "") {
    // NOP.
  } else {
    if (UseOneChecker == "Assert") {
      checker = new AssertChecker(NULL);
      errs() << "EventMgr::setupEvents::AssertChecker\n";
    } else if (UseOneChecker == "OpenClose") {
      checker = new OpenCloseChecker(NULL);
      errs() << "EventMgr::setupEvents::OpenCloseChecker\n";
    } else if (UseOneChecker == "Lock") {
      checker = new LockChecker(NULL);
      errs() << "EventMgr::setupEvents::LockChecker\n";
    } else if (UseOneChecker == "File") {
      checker = new FileChecker(NULL);
      errs() << "EventMgr::setupEvents::FileChecker\n";
    } else if (UseOneChecker == "DataLoss") {
      checker = new DataLossChecker(NULL);
      errs() << "EventMgr::setupEvents::DataLossChecker\n";
    } else
      assert(false && "UseOneChecker must be Assert, OpenClose, Lock, File, or DataLoss.");
  }
  
  for (Module::iterator f = M.begin(); f != M.end(); ++f) {
    if (f->hasName()) {
      // Get function list from event-funcs.h.
      if (tern::EventFuncs::isEventFunc(f->getNameStr().c_str())) {
        if (!checker) {
          eventFuncs.push_back(f);
        } else if (checker->isImportant(f->getNameStr())) { // Select events based on the checker.
          fprintf(stderr, "EventMgr::initEvents event %s\n", f->getNameStr().c_str());
          eventFuncs.push_back(f);
        }
      }
    }
  }

  if (checker)
    delete checker;
}

// TODO: search in vector may be slow
bool EventMgr::is_sync_function(Function *f) {
  for (size_t i = 0; i < eventFuncs.size(); ++i) {
    if (eventFuncs[i] == f)
      return true;
  }
  return false;
}

void EventMgr::DFS(Function *f) {
  visited.insert(f);
  CallGraphFP &CG = getAnalysis<CallGraphFP>();
  InstList css = CG.get_call_sites(f);
  for (size_t i = 0; i < css.size(); ++i) {
    Function *caller = css[i]->getParent()->getParent();
    if (visited.count(caller) == 0) {
      parent[caller] = f;
      DFS(caller);
    }
  }
}

void EventMgr::output(const Module &M) const {
  vector<string> func_names;
  FILE *fout;

  func_names.clear();
  for (Module::const_iterator fi = M.begin(); fi != M.end(); ++fi)
    func_names.push_back(fi->getNameStr());
  sort(func_names.begin(), func_names.end());
  fout = fopen("/tmp/all-func.txt", "w");
  for (size_t i = 0; i < func_names.size(); ++i) {
    fprintf(fout, "%s\n", func_names[i].c_str());
  }

  func_names.clear();
  for (DenseSet<Function *>::const_iterator it = visited.begin();
      it != visited.end(); ++it) {
    func_names.push_back((*it)->getNameStr());
  }
  sort(func_names.begin(), func_names.end());
  fout = fopen("/tmp/event-func.txt", "w");
  for (size_t i = 0; i < func_names.size(); ++i) {
    fprintf(fout, "%s\n", func_names[i].c_str());
  }
}

bool EventMgr::mayCallEvent(Function *f) {
  return visited.count(f);
}

bool EventMgr::eventBetween(BranchInst *prevInstr, Instruction *postInstr) {
  Function *func = Util::getFunction(prevInstr);
  BasicBlock *postDomBB = NULL;
  if (postInstr)
    postDomBB = Util::getBasicBlock(postInstr);
  
  /* Flood fill from <bb> until reaching <post_dominator_bb> */
  bbVisited.clear();
  CallGraphFP &CG = getAnalysis<CallGraphFP>();
  for (Function::iterator bi = func->begin(); bi != func->end(); ++bi)
    bbVisited[bi] = false;
  for (unsigned i = 0; i < prevInstr->getNumSuccessors(); i++)
    DFS(prevInstr->getSuccessor(i), postDomBB);
  /* If any visited BB has a sync operation, the branch cannot be omitted. */
  for (Function::iterator bi = func->begin(); bi != func->end(); ++bi) {
    if (!bbVisited[bi])
      continue;
    // cerr << "Visited BB: " << bi->getNameStr() << endl;
    for (BasicBlock::iterator ii = bi->begin(); ii != bi->end(); ++ii) {
      if (is_call(ii) && !is_intrinsic_call(ii)) {
        vector<Function *> called_funcs = CG.get_called_functions(ii);
        for (size_t i = 0; i < called_funcs.size(); ++i) {
          if (mayCallEvent(called_funcs[i]))
            return true;
        }
      }
    }
  }
  return false;
}

void EventMgr::DFS(BasicBlock *x, BasicBlock *sink) {
  // cerr << "DFS: " << x->getNameStr() << endl;
  if (bbVisited[x])
    return;
  // Stop at the sink -- the post dominator of the branch
  if (x == sink)
    return;
  bbVisited[x] = true;
  for (succ_iterator it = succ_begin(x); it != succ_end(x); ++it) {
    BasicBlock *y = *it;
    DFS(y, sink);
  }
}

void EventMgr::traverse_call_graph(Module &M) {
  visited.clear();
  parent.clear();
  for (size_t i = 0; i < eventFuncs.size(); ++i) {
    DFS(eventFuncs[i]);
  }
}

void EventMgr::print_call_chain(Function *f) {
  /*if (!parent.count(f)) {
    std::cerr << endl;
    return;
  }
  std::cerr << f->getNameStr() << " => ";
  print_call_chain(parent[f]);*/
}

void EventMgr::print_calling_functions(Function *f) {
  /*if (!call_sites.count(f))
    return;
  cerr << f->getNameStr() << ": ";
  vector<Instruction *> &callers = call_sites[f];
  for (size_t i = 0; i < callers.size(); ++i) {
    cerr << callers[i]->getParent()->getParent()->getNameStr() << ' ';
  }
  cerr << endl;*/
}

void EventMgr::stats(const Module &M) const {

}

bool EventMgr::runOnModule(Module &M) {
  //CallGraphFP::runOnModule(M);
  setupEvents(M);
  traverse_call_graph(M);

  // Clean callgraph-fp.
  clean();
  
  return false;
}

void EventMgr::print(llvm::raw_ostream &O, const Module *M) const {
  output(*M);
  stats(*M);
}
