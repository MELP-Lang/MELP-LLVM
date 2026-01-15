CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I.
AR = ar
ARFLAGS = rcs

# Targets
TARGET = test_runtime_sto
TARGET_BIGDEC = test_bigdecimal
TARGET_SSO = test_sso_string
LIB = libsto_runtime.a
SOURCES = runtime_sto.c sto_runtime.c bigdecimal.c sso_string.c test_runtime_sto.c test_bigdecimal.c test_sso_string.c
LIB_OBJECTS = runtime_sto.o sto_runtime.o bigdecimal.o sso_string.o
TEST_OBJECTS = runtime_sto.o bigdecimal.o test_runtime_sto.o
BIGDEC_TEST_OBJECTS = runtime_sto.o bigdecimal.o test_bigdecimal.o
SSO_TEST_OBJECTS = runtime_sto.o sso_string.o test_sso_string.o

all: $(LIB) $(TARGET) $(TARGET_BIGDEC) $(TARGET_SSO)

# Static library for linking with compiler
$(LIB): $(LIB_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "✅ STO runtime library created: $(LIB)"

$(TARGET): $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_BIGDEC): $(BIGDEC_TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_SSO): $(SSO_TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

test: $(TARGET) $(TARGET_BIGDEC) $(TARGET_SSO)
	@echo "=== Testing Overflow Detection ==="
	./$(TARGET)
	@echo ""
	@echo "=== Testing BigDecimal ==="
	./$(TARGET_BIGDEC)
	@echo ""
	@echo "=== Testing SSO String ==="
	./$(TARGET_SSO)

clean:
	rm -f $(LIB_OBJECTS) $(TEST_OBJECTS) $(BIGDEC_TEST_OBJECTS) $(SSO_TEST_OBJECTS) $(TARGET) $(TARGET_BIGDEC) $(TARGET_SSO) $(LIB)

.PHONY: all test clean
