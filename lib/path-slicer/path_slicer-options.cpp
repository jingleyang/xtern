
// DO NOT EDIT -- automatically generated by ./gen-path-slicer-opt.pl
// from path_slicer.default.options

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <assert.h>

#include "options.h"

using namespace std;

int __tern_path_slicer__aaol_dbg_level = 0;

int __tern_path_slicer__do_inter_thread_slicing = 1;

int __tern_path_slicer__do_intra_thread_slicing = 1;

int __tern_path_slicer__intra_thread_slicing_start_index = 0;

int __tern_path_slicer__print_debug_info = 0;

int __tern_path_slicer__slicing_mode = 0;



namespace {

struct _options: public options {

    virtual ~_options() {








    }

    virtual void init() {






    }

    virtual int load(const char *dom, const char *opt, const char *val) {
    if (!strcmp (dom, "tern_path_slicer")) {
        if (!strcmp (opt, "aaol_dbg_level"))
            { __tern_path_slicer__aaol_dbg_level = (int)strtoul(val, 0, 0); return 1; }
        if (!strcmp (opt, "do_inter_thread_slicing"))
            { __tern_path_slicer__do_inter_thread_slicing = (int)strtoul(val, 0, 0); return 1; }
        if (!strcmp (opt, "do_intra_thread_slicing"))
            { __tern_path_slicer__do_intra_thread_slicing = (int)strtoul(val, 0, 0); return 1; }
        if (!strcmp (opt, "intra_thread_slicing_start_index"))
            { __tern_path_slicer__intra_thread_slicing_start_index = (int)strtoul(val, 0, 0); return 1; }
        if (!strcmp (opt, "print_debug_info"))
            { __tern_path_slicer__print_debug_info = (int)strtoul(val, 0, 0); return 1; }
        if (!strcmp (opt, "slicing_mode"))
            { __tern_path_slicer__slicing_mode = (int)strtoul(val, 0, 0); return 1; }

    }

        return 0;
    }
    
    virtual void print(std::ostream &o) {
    o << "tern_path_slicer::aaol_dbg_level    " << __tern_path_slicer__aaol_dbg_level << endl;
    o << "tern_path_slicer::do_inter_thread_slicing    " << __tern_path_slicer__do_inter_thread_slicing << endl;
    o << "tern_path_slicer::do_intra_thread_slicing    " << __tern_path_slicer__do_intra_thread_slicing << endl;
    o << "tern_path_slicer::intra_thread_slicing_start_index    " << __tern_path_slicer__intra_thread_slicing_start_index << endl;
    o << "tern_path_slicer::print_debug_info    " << __tern_path_slicer__print_debug_info << endl;
    o << "tern_path_slicer::slicing_mode    " << __tern_path_slicer__slicing_mode << endl;        
    }
    
};

struct register_options x(new _options);

}

