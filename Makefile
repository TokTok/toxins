build: $(foreach d,$(notdir $(abspath $(dir $(wildcard */Makefile)))),$d/$d.build)
clean: $(foreach d,$(notdir $(abspath $(dir $(wildcard */Makefile)))),$d/$d.clean)
check: $(foreach d,$(notdir $(abspath $(dir $(wildcard */Makefile)))),$d/$d.check)

%.build: ; $(MAKE) -C $(@D)
%.clean: ; $(MAKE) -C $(@D) clean
%.check: ; $(MAKE) -C $(@D) check
