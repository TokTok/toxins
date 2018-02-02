PROGRAM = tox_sync

$(PROGRAM): $(PROGRAM).c ../util/misc_tools.c
	$(LINK.c) -o $@ $^ -static -ltoxcore -lsodium -lpthread

clean:
	rm -f $(PROGRAM)

check:
	./$(PROGRAM) --help
