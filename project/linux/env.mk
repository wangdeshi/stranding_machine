CC := sdcc

localdir := $(shell pwd)

INCLUDE_DIR := \
	-I${MAKEROOT}/../../common/ \
	-I${MAKEROOT}/../../drivers/ \
	-I${MAKEROOT}/../../hal/ \
	-I${MAKEROOT}/../../init/ \
	-I${MAKEROOT}/../../modules/ \

CFLAGS += $(INCLUDE_DIR) -mmcs51 -D__SDCC__

LD_FLAGS +=  


$(mkdir)%.d:%.c
	${CC} -MM $(CFLAGS) $< > $@1
	sed 's,\($*\)\.rel[:]*, $(mkdir)\1.rel $(mkdir)\1.d:,g'< $@1 > $@
	rm -rf $@1

$(mkdir)%.rel : %.c
	${CC} ${CFLAGS} -c $<
	@-mv $(localdir)/*.asm $(mkdir)
	@-mv $(localdir)/*.lst $(mkdir)
	@-mv $(localdir)/*.rel $(mkdir)
	@-mv $(localdir)/*.sym $(mkdir)


