.RECIPEPREFIX := +
MAKEFLAGS := $(MAKEFLAGS) --no-print-directory
CXX := g++
AR := ar
NAME := ace
FILES := KINC.files



build := ../build/
run := ../run/
lib := ../lib/

acxxflags := $(CXXFLAGS) -g -std=c++11
aldflags := $(LDFLAGS)
aldlibs := $(LDLIBS) -lOpenCL

libf := $(lib)lib$(NAME).a

allsrc := $(filter %.cpp,$(shell cat $(FILES)))
utest := $(allsrc)
library := $(filter-out %.t.cpp,$(allsrc))

alldpds := $(addprefix $(build),$(allsrc:%.cpp=%.d))

dpds := $(addprefix $(build),$(library:%.cpp=%.d))
objs := $(addprefix $(build),$(library:%.cpp=%.o))

udpds := $(addprefix $(build),$(utest:%.cpp=%.d))
uobjs := $(addprefix $(build),$(utest:%.cpp=%.o))

plugins := plugins/plugins.a



.PHONY: clean depclean all library test check



all: library test
library: $(libf)
test: $(run)unit

include $(alldpds)

$(libf): $(objs) $(dpds)
+@echo "Building library."
+@$(AR) rcs $@ $(objs)

$(run)unit: $(uobjs) $(udpds)
+@echo "Building unit test."
+@$(CXX) $(uobjs) $(aldflags) $(aldlibs) -o $@

depend: $(alldpds)
+@echo Done.

$(build)%.o : %.cpp
+@echo "Building object $@"
+@$(CXX) $(acxxflags) -c $< -o $(build)$@

$(build)%.d: %.cpp
+@echo "Building depend $@"
+@echo -n "$@ $(build)" > $@
+@$(CXX) $(acxxflags) -MM $< >> $@

check: test
+@cd $(run)/tmp && ../unit

clean:
+@echo "Cleaning all."
+@rm -f $(build)*.o $(progf)

depclean:
+@echo "Cleaning all dependency files."
+@rm -f $(build)*.d
