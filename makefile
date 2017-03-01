CXX = g++
OBJECTS = main.o beaner.o
DESTDIR_TARGET = beaner


all: $(DESTDIR_TARGET)
	ctags -R

$(DESTDIR_TARGET): $(OBJECTS)
	$(CXX) -o $(DESTDIR_TARGET) $(OBJECTS) 

.cc.o:
	 $(CXX) -std=c++11 -c -g -o $@ $<

clean:
	 rm -f $(DESTDIR_TARGET) *.o
	 rm -f ./data/*

r:	$(DESTDIR_TARGET)
	./$(DESTDIR_TARGET)
