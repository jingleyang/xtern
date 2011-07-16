#include "recorder/access/logaccess.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"

using namespace std;
using namespace llvm;
using namespace tern;

static cl::opt<std::string>
InputFilename(cl::Positional, cl::desc("<input log file>"),
    cl::init("-"), cl::value_desc("filename"));

int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv,
    "Print contents of a log file\n");

  Log log(InputFilename.c_str());
  Log::rec_iterator ri;

  for(ri=log.rec_begin(); ri!=log.rec_end(); ++ri) {
    PrintRecord(outs(), *ri) << "\n";
  }
}

