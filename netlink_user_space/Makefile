TARGET = send_m_prt

GCC_CMDLINE = -c -g
LD_CMDLINE  = -L/usr/lib -g
CC_INCLUDE  = -I/usr/include

C_SRC       = send_m_prt.c

OBJS        = $(patsubst %.c,%.o,$(C_SRC))

.SUFFIXES:	.o

.c.o:
	gcc $(GCC_CMDLINE) $< -o $@

$(TARGET):	$(OBJS) Makefile
	gcc $(LD_CMDLINE) $(OBJS) -o $@
        
clean:
	rm -f *.o $(TARGET)

dep:
	gcc $(CC_INCLUDE) -MM $(C_SRC) > .deps

-include .deps