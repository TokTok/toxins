PROGRAM = minitox

$(PROGRAM): import/$(PROGRAM).c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ -static -ltoxcore -lsodium -lpthread

clean:
	rm -f $(PROGRAM)

check:
	./$(PROGRAM) --help
