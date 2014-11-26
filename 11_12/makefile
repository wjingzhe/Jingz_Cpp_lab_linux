#jingz's first file to test makeFile
allTarget:stdafx_target 11_12_target
	g++ stdafx.o 11_12.o -o 11_12.exe
#remove the object files
	rm -rf stdafx.o 11_12.o
#complie the cpps into objects
stdafx_target:
	g++ -std=c++11 -c stdafx.cpp -o stdafx.o
11_12_target:
	g++ -std=c++11 -c 11_12.cpp -o 11_12.o
