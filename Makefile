CXX = g++
CXXFLAGS = -g -std=gnu++11
INCS = -I./src
CXXFLAGS_TEST =  -lgtest  -lpthread $(CXXFLAGS)
OBJS_TEST = ./test/MessengerTest.o
OBJS_SRC = ./src/Messenger.o
CPP_TEST = ./test/MessengerTest.cpp
CPP_APP = ./src/Messenger.cpp

testAll: test/test.cpp $(OBJS_SRC) $(OBJS_TEST)
	$(CXX) $(CXXFLAGS_TEST) -o testAll test/test.cpp $(OBJS_TEST) $(OBJS_SRC) $(INCS)

$(OBJS_SRC):
	$(CXX) $(CXXFLAGS) -c $(CPP_APP) -o $(OBJS_SRC) $(INCS)

$(OBJS_TEST):
	$(CXX) $(CXXFLAGS_TEST) -c $(CPP_TEST) -o $(OBJS_TEST) $(INCS)

clean:
	rm *.o testAll test/*.o testAll.xml

