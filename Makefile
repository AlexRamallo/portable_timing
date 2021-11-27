CXX=g++

OUT/Test: test.cpp OUT/Time.o
	$(CXX) test.cpp OUT/Time.o -o OUT/Test

OUT:
	mkdir -p OUT

OUT/Time.o: Time.h Time.cpp OUT
	$(CXX) -c Time.cpp -o OUT/Time.o

clean:
	rm -rf OUT