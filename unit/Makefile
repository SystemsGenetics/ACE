.RECIPEPREFIX := +
MAKEFLAGS := $(MAKEFLAGS) --no-print-directory
CXX := g++
GDB := gdb
NAME := unit
FILES := ACE_unit.files



build := ../build/unit/
run := ../run/
lib := ../lib/
libf := $(lib)libace.a

acxxflags := $(CXXFLAGS) -g -std=c++11
aldflags := $(LDFLAGS)
aldlibs := $(LDLIBS) -L../lib -lace -lOpenCL

unitf := $(run)$(NAME)

src := $(filter %.cpp,$(shell cat $(FILES)))
dpds := $(addprefix $(build),$(src:%.cpp=%.d))
objs := $(addprefix $(build),$(src:%.cpp=%.o))

.PHONY: clean depclean all unit check debug base



all: unit
unit: $(unitf)

ifndef DIGNORE
include $(dpds)
endif

$(unitf): $(objs) $(dpds) $(libf)
+@echo "Building unit test."
+@$(CXX) $(objs) $(aldflags) $(aldlibs) -o $@

depend: $(dpds)
+@echo Done.

$(build)%.o : %.cpp
+@echo "Building object $(*F)."
+@$(CXX) $(acxxflags) -c $< -o $@

$(build)%.d: %.cpp
+@echo "Building depend $(*F)."
+@echo -n "$@ $(build)" > $@
+@$(CXX) $(acxxflags) -MM $< >> $@

check: unit
+@cd $(run)/tmp && ../unit

debug: unit
+@cd $(run)/tmp && $(GDB) ../unit

base:
+@cd ../src && $(MAKE)

clean:
+@echo "Cleaning all."
+@rm -f $(build)*.o $(unitf)

depclean:
+@echo "Cleaning all dependency files."
+@rm -f $(build)*.d
