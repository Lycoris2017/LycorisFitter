# LycorisFitter
A RooFit based packges used for Gaus and LanGaus fit on charge response distributions.

## Use Conditions

* Please be aware this code need manually superivising to run, in terms of: what is the target histogram to fit, what to fit, etc.

* Please be aware of that: one needs ROOT with full RooFit compiled distribution to run the code. One remote machine choice to run the code is on NAF, bash script to set up envrionment on NAF is READY `setup_naf.sh`.


## Code Explanation

* `convlxgFit_ranged.cxx` : this is the code to run, one can compile it with the compiled.sh

* `mylangauss.cxx` : 2nd version fitting code protoype;

* `langauss.cxx` : 1st version fitting code protoype;

* `data/` : example data to run the fitting;

* `RooEg/` : langaus fit example code from Root example storage.

* `setup_naf.sh` : bash script to set up all necessary environment on NAF

   
