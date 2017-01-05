include $(MAKEROOT)/env.mk

SRCS := $(wildcard *.c) 
OBJS := $(addprefix $(mkdir),$(SRCS:.c=.rel))
DEPS := $(addprefix $(mkdir),$(SRCS:.c=.d))

.PHONY : all
all : $(OBJS) 

-include $(DEPS)
