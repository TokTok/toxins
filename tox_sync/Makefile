PROGRAM = tox_sync

$(PROGRAM): $(PROGRAM).c ../util/misc_tools.c
	$(LINK.c) -o $@ $^ `pkg-config --cflags --libs toxcore`

clean:
	rm -f $(PROGRAM)

check:
	./$(PROGRAM) --help
