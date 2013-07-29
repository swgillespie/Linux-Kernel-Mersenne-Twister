SUBDIRS += librand tests

.PHONY: kernel subdirs $(SUBDIRS)

subdirs: kernel $(SUBDIRS)

kernel:
	@cd kernel; make; cd ..;

$(SUBDIRS):
	@make -C $@

tests: kernel librand