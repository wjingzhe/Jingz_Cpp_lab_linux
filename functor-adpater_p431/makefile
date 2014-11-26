#jingz's simple file to test makeFile
allTarget:stdafx_target 11_12_target
	g++ stdafx.o functor-adapter_p431.o -o functor-adapter_p431.exe
#remove the object files
	rm -rf stdafx.o functor-adapter_p431.o
#complie the cpps into objects
stdafx_target:
	g++ -std=c++11 -c stdafx.cpp -o stdafx.o
11_12_target:
	g++ -std=c++11 -c functor-adapter_p431.cpp -o functor-adapter_p431.o
