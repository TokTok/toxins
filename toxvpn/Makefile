PROGRAM = toxvpn

SOURCES :=				\
	import/src/control.cpp		\
	import/src/interface.cpp	\
	import/src/interface_linux.cpp	\
	import/src/listener.cpp		\
	import/src/main.cpp		\
	import/src/route_linux.cpp

build: $(PROGRAM) toxvpn-remote

$(PROGRAM): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -I. -o $@ $^ -static -std=c++11 -ltoxcore -lsodium -lpthread -lcap -DBOOTSTRAP_FILE='"$(CURDIR)/import/res/bootstrap.json"'

toxvpn-remote: import/src/toxvpn-remote.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -I. -o $@ $^ -static -std=c++11           -lsodium -lpthread

clean:
	rm -f $(PROGRAM) toxvpn-remote

check:
	./$(PROGRAM) -h
