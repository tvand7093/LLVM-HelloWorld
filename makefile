
EXE = hello-llvm
FRONT = $(EXE)-frontend
LOC = ''
BITCODE_TEST = bitcode-test

# Determine how we are getting the location for LLVM
ifeq (,$(shell which llvm-config))
	export LLVM_LOC=$(shell brew --prefix llvm)/bin;
else
	export LLVM_LOC=$(shell llvm-config --prefix)/bin
endif

# Now we have the install location, so configure the settings 
LLVM_CONFIG=`$(LLVM_LOC)/llvm-config --cxxflags --ldflags --libs --system-libs core`

# Build out exe file.
all: $(EXE)

test: $(BITCODE_TEST).bc

# Exe build step. Compile bitcode object using gcc
$(EXE): $(EXE).o
	$(CXX) $^ -o $@

# Create object file from bitcode file
$(EXE).o: $(EXE).bc
	$(LLVM_LOC)/llc -filetype=obj $^ -o $@

# Write bitcode from frontend application
$(EXE).bc: $(FRONT)
	./$^ $@

# Build frontend application for emitting bitcode
$(FRONT):  main.cpp
	@echo $(LLVM_CONFIG)
	$(CXX) -g $^ $(LLVM_CONFIG) -o $@


$(BITCODE_TEST).bc: $(BITCODE_TEST).cpp
	$(CXX) -S -emit-llvm $^ -o $@

clean:
	@rm -rf *.dSYM *~ \#*\# *.bc *.o *.ir $(EXE)*
