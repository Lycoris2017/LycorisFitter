CFLAGS=`root-config --cflags`
LDFLAGS=`root-config --ldflags --glibs` -lRooFit -lRooFitCore -lMinuit

temp : mylangauss.cxx
	g++ -o temp mylangauss.cxx $(CFLAGS) $(LDFLAGS)

# libMyRooPdf.so : libMyRooPdf.so.1.0
# 	ln -sf libMyRooPdf.so.1.0 libMyRooPdf.so

# libMyRooPdf.so.1.0 : MyRooPdf.o MyRooPdfDict.o
# 	gcc -shared -Wl,-soname,libMyRooPdf.so.1 -o libMyRooPdf.so.1.0 MyRooPdf.o MyRooPdfDict.o

# MyRooPdfDict.o : MyRooPdfDict.cxx
# 	g++ -c MyRooPdfDict.cxx -fPIC $(CFLAGS) $(LDFLAGS)

# MyRooPdfDict.cxx : MyRooPdf.h LinkDef.h
# 	rootcint -f MyRooPdfDict.cxx -c MyRooPdf.h LinkDef.h

# MyRooPdf.o : MyRooPdf.cxx MyRooPdf.h
# 	g++ -c MyRooPdf.cxx -fPIC $(CFLAGS) $(LDFLAGS)
