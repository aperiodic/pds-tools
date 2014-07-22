main: pdstools.c PDSTools.h
	gcc -o libpdstools.so -lc -shared -fPIC -I/usr/lib/jvm/java-7-openjdk/include -I/usr/lib/jvm/java-7-openjdk/include/linux pdstools.c


PDSTools.class: PDSTools.java
	javac PDSTools.java

PDSTools.h: PDSTools.class
	javah PDSTools

.PHONY: clean
clean:
	rm -f *.so PDSTools.h *.class
