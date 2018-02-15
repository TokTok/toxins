PROGRAM = irc_syncbot

$(PROGRAM): $(PROGRAM).c ../util/misc_tools.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ -static -ltoxcore -lsodium -lpthread

clean:
	rm -f $(PROGRAM)

check:
	./$(PROGRAM) --help
