/* Author: Junfeng Yang (junfeng@cs.columbia.edu) -*- Mode: C++ -*- */
#ifndef __TERN_RECORDER_LOG_H
#define __TERN_RECORDER_LOG_H

#include <assert.h>
#include <stdarg.h>
#include <fstream>
#include <tr1/unordered_map>

#include "tern/logdefs.h"

namespace tern {

struct Logger {
  /// per-thread logging functions and data
  virtual void logInsid(unsigned insid) {}
  virtual void logLoad (unsigned insid, char* addr, uint64_t data) {}
  virtual void logStore(unsigned insid, char* addr, uint64_t data) {}
  virtual void logCall(uint8_t flags, unsigned insid,
                       short narg, void* func, va_list args) {}
  virtual void logRet(uint8_t flags, unsigned insid,
                      short narg, void* func, uint64_t data) {}
  virtual void logSync(unsigned insid, unsigned short sync,
                       unsigned turn, bool after = true, ...) {}
  virtual ~Logger() {}
  static __thread Logger* the; /// pointer to per-thread logger

#if 0
  /// obsolete
  static void markFuncCallLogged(void *func, unsigned funcid, const char* name){
    funcsCallLogged[func] = funcid;
  }
  static void markFuncEscape(void *func, unsigned funcid, const char* name) {
    funcsEscape[func] = funcid;
  }
  static bool funcCallLogged(void *func) {
    return funcsCallLogged.find(func) != funcsCallLogged.end();
  }
  static bool funcEscape(void *func) {
    return funcsEscape.find(func) != funcsEscape.end();
  }

  static func_map funcsEscape;
#endif

  /// code and data shared by all loggers
  static void progBegin();
  static void progEnd();
  static void threadBegin(int tid);
  static void threadEnd(void);

  /// map function address at runtime to a unique function ID.  We need
  /// this mapping so that we can find the llvm::Function* corresponding
  /// to a callee when analyzing a log
  static void mapFuncToID(void *func, unsigned funcid, const char* name){
    funcs[func] = funcid;
  }
  static bool funcHasID(void *func) {
    return funcs.find(func) != funcs.end();
  }
  typedef std::tr1::unordered_map<void*, unsigned> func_map;
  static func_map funcs; /// function address at runtime to ID map
};

struct TxtLogger: public Logger {
  virtual void logSync(unsigned insid, unsigned short sync,
                       unsigned turn, bool after = true, ...);
  TxtLogger(int tid);

protected:
  int tid;
  std::fstream ouf;
};

struct BinLogger: public Logger {
  virtual void logInsid(unsigned insid);
  virtual void logLoad (unsigned insid, char* addr, uint64_t data);
  virtual void logStore(unsigned insid, char* addr, uint64_t data);
  virtual void logCall(uint8_t flags, unsigned insid,
                       short narg, void* func, va_list args);
  virtual void logRet(uint8_t flags, unsigned insid,
                      short narg, void* func, uint64_t data);
  virtual void logSync(unsigned insid, unsigned short sync,
                       unsigned turn, bool after = true, ...);
  virtual ~BinLogger();
  BinLogger(int tid);

protected:

  void mapLogTrunk(void);
  void checkAndGrowLogSize(void) {
    // TODO: check log buffer size and allocate new space if necessary
    assert(off + RECORD_SIZE <= TRUNK_SIZE);
  }

  char*      buf;
  unsigned   off;
  int        fd;
  off_t      foff;
};

}

#endif
