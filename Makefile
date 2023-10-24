#### File and directory names ####

ROM = cv64.z64

#### Tools ####
# Compiler
CC   = tools/ido71/cc
# Linker
LD   = mips64-ld
# nOVL (ELF -> overlay conversion)
NOVL = tools/nOVL/novl
# CV64 ROM file insertion script
CV64_FILE_INSERT = tools/cv64_file_insert.py
# CV64 misc ROM patches script
CV64_PATCHES = tools/cv64_misc_patches.py
# Extracts ELF information
READELF = readelf
# Python
PYTHON = python3

#### Options ####
# Includes
INCLUDES := -Iheaders -Iheaders/include -Iheaders/include/game -Iheaders/include/ultra64 -Iheaders/include/game/modules

# Compiler flags
CFLAGS = -G0 -mips2 -non_shared -fullwarn -verbose -Xcpluscomm -Wab,-r4300_mul $(INCLUDES)
OPTFLAGS := -O2

#### Recipes ####

default: all
all:
	@mkdir -p build/
	
	@echo "======= Compiling $@ ======="
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o build/extendedStageSelect.o src/extendedStageSelect.c
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o build/creator.o src/creator.c
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o build/hook.o src/hook.c

	@echo "\n== Linking =="
	$(LD) -o build/extendedStageSelect.elf build/extendedStageSelect.o -T linker/extendedStageSelect_linker.ld
	$(LD) -o build/creator.elf build/creator.o -T linker/creator_linker.ld
	$(LD) -o build/hook.elf build/hook.o -T linker/hook_linker.ld

	@echo "\n== Converting ELF to overlay =="
	$(NOVL) -c -o build/extendedStageSelect.ovl build/extendedStageSelect.elf
	$(NOVL) -c -o build/creator.ovl build/creator.elf
	$(NOVL) -c -o build/hook.ovl build/hook.elf

	@echo "\n== Saving symbolic information... =="
	$(READELF) --all build/extendedStageSelect.elf > build/extendedStageSelect.elf.txt
	$(READELF) --all build/creator.elf > build/creator.elf.txt
	$(READELF) --all build/hook.elf > build/hook.elf.txt

	@echo "\n== Inserting overlay in ROM =="
	$(PYTHON) $(CV64_FILE_INSERT) $(ROM) build/extendedStageSelect.ovl BB2D88 71

	@echo "\n== Applying ROM patches =="
	$(PYTHON) $(CV64_PATCHES) $(ROM) build/creator.ovl build/hook.ovl

	@echo "\n"

clean:
	rm -rf build

.PHONY: all clean
