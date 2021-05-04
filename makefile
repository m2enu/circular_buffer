CC          := g++
LD          := $(CC)
MKDIR       := mkdir -p
RM          := rm -rf

SRC_DIR     := src
OBJ_DIR     := obj
TST_DIR     := test
UNITY_DIR   := tool-unity

TARGET      := unittest.elf

SRCS        := \
	$(SRC_DIR)/circular_buffer.c \

TSTS        := \
	$(UNITY_DIR)/unity.c \
	$(TST_DIR)/test_main.c \

INCS        := \
	$(SRC_DIR) \
	$(TST_DIR) \
	$(UNITY_DIR) \

OBJS        := \
	$(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o)) \
	$(addprefix $(OBJ_DIR)/,$(TSTS:.c=.o)) \

DEPS        := $(OBJS:.o=.d)
INCLUDES    := $(addprefix -I,$(INCS))

CFLAGS      := \
	-c \
	-MMD \

LDFLAGS     := \

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(info Linking $@)
	@$(LD) $(LDLFAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@if [ ! -d $(dir $@) ]; then $(MKDIR) $(dir $@); fi
	$(info Compiling $<)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

$(OBJ_DIR)/%.o: %.cpp
	@if [ ! -d $(dir $@) ]; then $(MKDIR) $(dir $@); fi
	$(info Compiling $<)
	@$(CC) $(INCLUDES) -c -o $@ $<

IS_DRYRUN := $(findstring n,$(filter-out --%,$(MAKEFLAGS)))
ONLY_CLEAN := $(findstring _clean_,_$(MAKEFLAGS)_)
ifeq ($(or $(IS_DRYRUN),$(ONLY_CLEAN)),)
	-include $(DEPS)
endif
