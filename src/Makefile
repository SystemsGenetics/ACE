.RECIPEPREFIX := +
MAKEFLAGS := $(MAKEFLAGS) --no-print-directory
CXX := g++
AR := ar
NAME := ace
FILES := ACE.files



build := ../build/
run := ../run/
lib := ../lib/

acxxflags := $(CXXFLAGS) -g -std=c++11
aldflags := $(LDFLAGS)
aldlibs := $(LDLIBS) -lOpenCL

libf := $(lib)lib$(NAME).a

src := $(filter %.cpp,$(shell cat $(FILES)))
dpds := $(addprefix $(build),$(src:%.cpp=%.d))
objs := $(addprefix $(build),$(src:%.cpp=%.o))

.PHONY: clean depclean all library



all: library
library: $(libf)

include $(dpds)

$(libf): $(objs) $(dpds)
+@echo "Building library."
+@$(AR) rcs $@ $(objs)

depend: $(dpds)
+@echo Done.

$(build)%.o : %.cpp
+@echo "Building object $@"
+@$(CXX) $(acxxflags) -c $< -o $@

$(build)%.d: %.cpp
+@echo "Building depend $@"
+@echo -n "$@ $(build)" > $@
+@$(CXX) $(acxxflags) -MM $< >> $@

clean:
+@echo "Cleaning all."
+@rm -f $(build)*.o $(libf)

depclean:
+@echo "Cleaning all dependency files."
+@rm -f $(build)*.d
