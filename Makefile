SUBDIRS += kernel librand tests

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	make -C $@

tests: kernel librand