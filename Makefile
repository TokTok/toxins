build: $(foreach d,$(notdir $(abspath $(dir $(wildcard */Makefile)))),$d/$d.build)
clean: $(foreach d,$(notdir $(abspath $(dir $(wildcard */Makefile)))),$d/$d.clean)

%.build: ; $(MAKE) -C $(@D)
%.clean: ; $(MAKE) -C $(@D) clean
