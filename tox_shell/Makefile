PROGRAM = tox_shell

$(PROGRAM): $(PROGRAM).c ../util/misc_tools.c
	$(LINK.c) -o $@ $^ `pkg-config --cflags --libs toxcore` -lutil

clean:
	rm -f $(PROGRAM)

check:
	./$(PROGRAM) --help
