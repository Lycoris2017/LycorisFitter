#!/bin/sh


g++ mylangauss.cxx -o lgfit $(root-config --cflags) -Wall
g++ -o temp mylangauss.cxx $(root-config --cflags --libs) -L $ROOTSYS/lib -lRooFit -lHtml -lMinuit -lRooFitCore 