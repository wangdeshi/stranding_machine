export MAKEROOT := $(shell pwd)

mkdir:=$(MAKEROOT)/out/

export mkdir

TARGET_DIRS := \
	../../common \
	../../drivers \
	../../hal \
	../../init \
	../../modules \

include ./env.mk

define build_obj
	for SubDir in $(TARGET_DIRS); do \
		if ! [ -d $$SubDir ]; then \
		echo "The $$SubDir is not exist !"; \
		exit 11; \
		fi; \
		echo "Building $$SubDir ..."; \
		make -C $$SubDir ; \
		if [ $$? -ne 0 ]; then \
		echo "Building $$SubDir is failed !"; \
		exit 12; \
		fi; \
	done

endef


all : 
	@mkdir -p $(MAKEROOT)/out; 
	@$(call build_obj)
	@$(CC) $(mkdir)main.rel `ls $(mkdir)*.rel|grep -v main.rel` $(LD_FLAGS)
	@-mv *.ihx $(mkdir)
	@-mv *.lk $(mkdir)
	@-mv *.map $(mkdir)
	@-mv *.mem $(mkdir)
	@#makebin $(mkdir)main.ihx strand.bin
	@packihx $(mkdir)main.ihx > strand.hex
	@#@objcopy -I ihex -O binary $(mkdir)strand.hex $(mkdir)strand.bin
clean :
	@rm -rf out
	@rm -rf *.lnk
	@rm -rf strand.bin
	@rm -rf strand.hex
