#!/bin/bash 

# GCC 4.9.3
source /cvmfs/sft.cern.ch/lcg/releases/gcc/4.9.3/x86_64-slc6/setup.sh


# ##-------------------------------##
# ##--- If you want AFS source: ---##

# ## GCC 4.9.3
# source /afs/cern.ch/sw/lcg/contrib/gcc/4.9.3/x86_64-slc6/setup.sh

# # -------------------------------------------------------------------- ---
# # ---  another alternative way to switch on gcc 4.9.3 yourself   ---
# # -------------------------------------------------------------------- ---
export PATH=/cvmfs/sft.cern.ch/lcg/releases/gcc/4.9.3/x86_64-slc6/bin:${PATH}
export LD_LIBRARY_PATH=/cvmfs/sft.cern.ch/lcg/releases/gcc/4.9.3/x86_64-slc6/lib64:/cvmfs/sft.cern.ch/lcg/releases/gcc/4.9.3/x86_64-slc6/lib:${LD_LIBRARY_PATH}

export CXX=g++
export CC=gcc

#--------------------------------------------------------------------------------
#     GSL
#--------------------------------------------------------------------------------
export GSL_HOME="/cvmfs/ilc.desy.de/sw/x86_64_gcc49_sl6/gsl/2.1"
export PATH="$GSL_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$GSL_HOME/lib:$LD_LIBRARY_PATH"

## ROOT 6.08.06
source  /cvmfs/ilc.desy.de/sw/x86_64_gcc49_sl6/root/6.08.06/bin/thisroot.sh

# ## ROOT 6.08.06
# source /afs/desy.de/project/ilcsoft/sw/x86_64_gcc49_sl6/root/6.08.06/bin/thisroot.sh
