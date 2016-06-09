.RECIPEPREFIX := +
MAKEFLAGS := $(MAKEFLAGS) --no-print-directory
CXX := g++
AR := ar
NAME := unit
FILES := ACE_unit.files



build := ../build/unit/
run := ../run/
lib := ../lib/

acxxflags := $(CXXFLAGS) -g -std=c++11
aldflags := $(LDFLAGS)
aldlibs := $(LDLIBS) -lOpenCL -L../lib -lace

unitf := $(run)$(NAME)

src := $(filter %.cpp,$(shell cat $(FILES)))
dpds := $(addprefix $(build),$(src:%.cpp=%.d))
objs := $(addprefix $(build),$(src:%.cpp=%.o))

.PHONY: clean depclean all unit test check



all: unit
unit: $(unitf)

include $(dpds)

$(unitf): $(objs) $(dpds)
+@echo "Building unit test."
+@$(CXX) $(objs) $(aldflags) $(aldlibs) -o $@

depend: $(dpds)
+@echo Done.

$(build)%.o : %.cpp
+@echo "Building object $@"
+@$(CXX) $(acxxflags) -c $< -o $@

$(build)%.d: %.cpp
+@echo "Building depend $@"
+@echo -n "$@ $(build)" > $@
+@$(CXX) $(acxxflags) -MM $< >> $@

check: unit
+@cd $(run)/tmp && ../unit

clean:
+@echo "Cleaning all."
+@rm -f $(build)*.o $(unitf)

depclean:
+@echo "Cleaning all dependency files."
+@rm -f $(build)*.d
