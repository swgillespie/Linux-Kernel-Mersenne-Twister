SUBDIRS += librand tests

.PHONY: kernel subdirs $(SUBDIRS)

subdirs: kernel $(SUBDIRS)

kernel:
	@cd kernel; make; cd ..;

$(SUBDIRS):
	@make -C $@

tests: kernel librand

install: kernel librand
	@echo "  [INSTALL] Installing"
	@insmod kernel/linux_twister.ko
	@cp librand/librand.so /usr/lib/librand.so.1.0
	@ln -s /usr/lib/librand.so.1.0 /usr/lib/librand.so