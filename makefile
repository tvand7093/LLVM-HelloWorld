
EXE = hello-llvm
FRONT = $(EXE)-frontend
BITCODE_TEST = bitcode-test

# Now we have the install location, so configure the settings 
LLVM_CONFIG=`llvm-config --cxxflags --ldflags --libs --system-libs core`

# Build out exe file.
all: $(EXE)

test: $(BITCODE_TEST).bc

# Exe build step. Compile bitcode object using gcc
$(EXE): $(EXE).o
	$(CXX) $^ -o $@

# Create object file from bitcode file
$(EXE).o: $(EXE).bc
	llc -filetype=obj $^ -o $@

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
