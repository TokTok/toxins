PROGRAM = tox_shell

$(PROGRAM): $(PROGRAM).c ../util/misc_tools.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ -static -ltoxcore -lsodium -lpthread -lutil

clean:
	rm -f $(PROGRAM)

check:
	./$(PROGRAM) --help
