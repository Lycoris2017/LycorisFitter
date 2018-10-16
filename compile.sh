#!/bin/sh


#g++ -o temp mylangauss.cxx $(root-config --cflags --libs) -L $ROOTSYS/lib -lRooFit -lHtml -lMinuit -lRooFitCore 

g++ -o temp convlxgFit_ranged.cxx $(root-config --cflags --libs) -L $ROOTSYS/lib -lRooFit -lHtml -lMinuit -lRooFitCore 

## how to run
# ./temp hist_s632.root
