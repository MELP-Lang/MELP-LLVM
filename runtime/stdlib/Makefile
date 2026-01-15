# MLP Standard Library Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC -D_GNU_SOURCE
AR = ar
ARFLAGS = rcs

# Libraries
LIB_STDLIB = libmlp_stdlib.a
LIB_STAGE2 = libmlp_stage2.a

# Standard library sources (STO-aware, for future use)
STDLIB_SOURCES = mlp_io.c mlp_string.c mlp_panic.c mlp_state.c mlp_math.c mlp_list.c mlp_map.c mlp_optional.c
STDLIB_OBJECTS = $(STDLIB_SOURCES:.c=.o)

# Stage 2 bootstrap sources (non-STO, simple wrappers)
STAGE2_WRAPPER_SRC = mlp_stage2_wrappers.c
STAGE2_WRAPPER_OBJ = mlp_stage2_wrappers.o
STAGE2_WRAPPER_RENAMED = mlp_stage2_wrappers_renamed.o

# All sources for easy management
ALL_SOURCES = $(STDLIB_SOURCES) $(STAGE2_WRAPPER_SRC)
ALL_OBJECTS = $(STDLIB_OBJECTS) $(STAGE2_WRAPPER_OBJ)

all: $(LIB_STDLIB) $(LIB_STAGE2)

# Standard library (STO-aware)
$(LIB_STDLIB): $(STDLIB_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "✅ MLP stdlib created: $(LIB_STDLIB)"

# Stage 2 library (simple wrappers + stdlib, but wrapper symbols renamed)
# Also rename STO-aware mlp_print_numeric to avoid conflict with wrapper
$(LIB_STAGE2): $(STDLIB_OBJECTS) $(STAGE2_WRAPPER_RENAMED)
	# Rename STO-aware mlp_print_numeric in mlp_io.o to mlp_print_numeric_sto
	objcopy --redefine-sym mlp_print_numeric=mlp_print_numeric_sto mlp_io.o mlp_io_stage2.o
	# Create library with renamed wrapper (mlp_print_numeric_s2 -> mlp_print_numeric)
	$(AR) $(ARFLAGS) $@ $(STAGE2_WRAPPER_RENAMED) mlp_io_stage2.o $(filter-out mlp_io.o,$(STDLIB_OBJECTS))
	@echo "✅ Stage 2 library created: $(LIB_STAGE2)"

# Rename wrapper symbols (mlp_print_numeric_s2 -> mlp_print_numeric)
$(STAGE2_WRAPPER_RENAMED): $(STAGE2_WRAPPER_OBJ)
	objcopy --redefine-sym mlp_print_numeric_s2=mlp_print_numeric $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(ALL_OBJECTS) $(STAGE2_WRAPPER_RENAMED) mlp_io_stage2.o $(LIB_STDLIB) $(LIB_STAGE2)

.PHONY: all clean
