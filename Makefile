AS = nspire-as
GCC = nspire-gcc
LD = nspire-ld
GCCFLAGS = -Wall
OBJCOPY := "$(shell which arm-elf-objcopy 2>/dev/null)"
ifeq (${OBJCOPY},"")
	OBJCOPY := arm-none-eabi-objcopy
endif
OBJS = screenclose.o

all: screenclose.tns

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $<

%.o: %.S
	$(GCC) $(GCCFLAGS) -c $<

screenclose.tns: $(OBJS)
	$(LD) $^ -o $(@:.tns=.elf)
	$(OBJCOPY) -O binary $(@:.tns=.elf) $@

.PHONY: clean

clean:
	rm -f *.o *.elf *.tns
