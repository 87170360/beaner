CXX = g++
OBJECTS = main.o test.o
DESTDIR_TARGET = main


all: $(DESTDIR_TARGET)

$(DESTDIR_TARGET): $(OBJECTS)
	$(CXX) -o $(DESTDIR_TARGET) $(OBJECTS)

.cc.o:
	 $(CXX) -c -o $@ $<

clean:
	 rm -rf $(DESTDIR_TARGET) *.o
