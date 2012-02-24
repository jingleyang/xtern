#!/usr/bin/python

# TODO: test dynamic hook of synchronization routines

# TODO: to check the determinism of the RR scheduler, perhaps just run the
# test program a few times and check that the generated schedules are
# identical.

import re
import os
import sys
import errno
import shutil
import filecmp
import argparse
import subprocess

parser = argparse.ArgumentParser(description
                                 ='run scheduler test');
parser.add_argument('program',
                    help='.c, .cpp, or .ll program to run test on')
parser.add_argument('-gxx', dest='gxx', required=True,
                    help='g++ command line')
parser.add_argument('-llvmgcc', dest='llvmgcc', required=True,
                    help='llvmgcc command line')
parser.add_argument('-projbindir', dest='projbindir', required=True,
                    help='directory where tern tools are located')
parser.add_argument('-ternruntime', dest='ternruntime', required=True,
                    help='tern x86 runtime')
parser.add_argument('-ternbcruntime', dest='ternbcruntime', required=True,
                    help='tern bc runtime')
parser.add_argument('-gen', dest='gen', default=False, action='store_true',
                    help='generate expected outputs instead of testing them')

def gen(cmd, prog):
    m = re.search('\|\s*FileCheck.*$', cmd)
    if m == None:
        return False
    print 'generating outputs for %s on %s' % (cmd, prog)
    cmd = cmd[:m.start(0)]
    #print cmd
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)
    output = p.communicate()[0];

    fi = open(prog, 'r')
    fo = open(prog + '~', 'w')
    # print until the first matching lines
    for l in fi:
        if re.match('//\s*CHECK', l):
            break
        fo.write(l)
    fo.write('// CHECK indicates expected output checked by FileCheck; '\
             'auto-generated by appending -gen to the RUN command above.\n')
    for i, l in enumerate(output.splitlines()):
        l = re.split('\s*0x[\dA-Fa-f]+', l, maxsplit=1)[0]
        if i == 0:
            s = ':     '
        else:
            s = '-NEXT:'
        fo.write("// CHECK%s %s\n" % (s, l))
    # skip lines matching @prefix and print other lines
    for l in fi:
        if re.match('//\s*CHECK', l):
            continue
        fo.write(l)
    fo.close()
    fi.close()
    os.rename(fo.name, fi.name)
    return True

def normalize_ptr(kvmap, m):
    k = int(m.group(), 16)
    limit = 10
    if k >= -limit and k <= limit:
        return str(k)
    if k == errno.ETIMEDOUT or k == errno.EBUSY:
        return str(k)
    if k not in kvmap:
        kvmap[k] = 'ptr' + str(len(kvmap))
    return str(kvmap[k])

curry = lambda func, *args, **kw:\
    lambda *p, **n: func(*args + p, **dict(kw.items() + n.items()))

def check_schedule(schedule):
    # check that schedule is not empty
    assert os.path.getsize(schedule) > 0
    # TODO: check that no critical sections of the same lock overlap
    # TODO: check that semaphore count is always non-negative

def normalize_schedule(schedule):
    kvmap = dict()
    pat = re.compile('0x[\dA-Fa-f]+')
    fi = open(schedule, 'r')
    fo = open(schedule + '~', 'w')
    for l in fi:
        lo = pat.sub(curry(normalize_ptr, kvmap), l)
        fo.write(lo)
    fo.close()
    fi.close()
    os.rename(fo.name, fi.name)

def check_deterministic(cmd, prog):
    m = re.search('output_dir=([^\s]+)\s+', cmd)
    assert m != None
    # print 'output directory is %s' % m.group(1)
    output_dir = m.group(1)

    # generate schedule
    schedule = output_dir.replace('outdir', 'schedule');
    shutil.rmtree(output_dir)
    os.system(cmd)
    os.system('sort -k 2 -n %s/*.txt > %s' % (output_dir, schedule))
    normalize_schedule(schedule)
    check_schedule(schedule)

    # check determinism
    for i in range(10):
        new_schedule = ('%s.%d' % (schedule, i))
        shutil.rmtree(output_dir)
        os.system(cmd)
        os.system('sort -k 2 -n %s/*.txt > %s' % (output_dir, new_schedule))
        normalize_schedule(new_schedule)
        assert filecmp.cmp(schedule, new_schedule)
    return

def run(cmd, map):
    cmd = cmd.split('RUN:')
    if len(cmd) == 1:
        return
    cmd = cmd[1]
    for key, val in map.iteritems():
        # print key, val
        if isinstance(val, str):
            cmd = cmd.replace('%'+key, val)
    if args['gen'] and gen(cmd, prog):
        return
    if cmd.endswith('ScheduleCheck'):
        cmd = cmd.split('ScheduleCheck')[0]
        check_deterministic(cmd, prog)
        return
    print 'running <%s>' % cmd
    os.system(cmd);

args = vars(parser.parse_args())
prog = args['program']
del args['program']

args['s'] = prog
args['t'] = os.path.basename(args['s']) + '.tmp'

# these commands follow the same style as LLVM dejagnu test commands
cmds = '''
// RUN: %llvmgcc %s -c -o %t1.ll -S
// RUN: %projbindir/tern-instr < %t1.ll -o %t2
// RUN: llvm-dis -f %t2-record.bc

// test the x86 .a libraries
// RUN: llc -o %t2.s %t2-record.bc
// RUN: %gxx -o %t2 %t2.s %ternruntime -lpthread -lrt
// test FCFS scheduler
// RUN: env TERN_OPTIONS=runtime_type=FCFS ./%t2 | FileCheck %s
// test RR scheduler
// : rm -rf %t2.outdir
// RUN: env TERN_OPTIONS=runtime_type=RR:RR_skip_zombie=0:log_type=test:output_dir=%t2.outdir ./%t2  | FileCheck %s
// RUN: env TERN_OPTIONS=runtime_type=RR:RR_skip_zombie=0:log_type=test:output_dir=%t2.outdir ./%t2  ScheduleCheck

// test the LLVM .bc modules
// XXX: llvm-ld -o %t3 %t2-record.bc %ternbcruntime
// XXX: llvm-dis -f %t3.bc
// XXX: llc -o %t3.s %t3.bc
// XXX: %gxx -o %t3 %t3.s -lpthread -lrt
// test FCFS scheduler
// XXX: env TERN_OPTIONS=runtime_type=FCFS ./%t3 | FileCheck %s
// test RR scheduler
// : rm -rf %t3.outdir
// XXX: env TERN_OPTIONS=runtime_type=RR:RR_skip_zombie=0:log_type=test:output_dir=%t3.outdir ./%t3  | FileCheck %s
// XXX: env TERN_OPTIONS=runtime_type=RR:RR_skip_zombie=0:log_type=test:output_dir=%t3.outdir ./%t3  ScheduleCheck
'''
for cmd in cmds.splitlines():
    run(cmd, args)