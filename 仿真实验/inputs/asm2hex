#!/usr/bin/python

import os, argparse, subprocess

# parse arguments
parser = argparse.ArgumentParser()
parser.add_argument("fasm", metavar="input.s", help="the MIPS assembly file (ASCII)")
args = parser.parse_args()

fasm = args.fasm
fhex = os.path.splitext(args.fasm)[0] + ".x"

# run SPIM (the actual MIPS assembler)
SPIM = "/afs/ece/class/ece447/bin/spim447"
cmd = [SPIM, "-notrap", "-vasm", fasm, fhex]
subprocess.call(cmd)

# SPIM outputs many files; but we are interested in only one
cmd = ["mv", fhex + ".text.dat", fhex]
subprocess.call(cmd)

# remove unnecessary two lines from the file
lines = open(fhex).readlines()
lines = map(lambda x: x.lstrip(), lines)
data = str.join('', lines[2:])
data = str.join('\n', data.split())
open(fhex, 'w').write(data)

# remove all other files
cmd = ["rm", fhex + ".*.dat"]
cmd = str.join(' ', cmd)
subprocess.call(cmd, shell=True)  # we need a shell to expand the wildcard

