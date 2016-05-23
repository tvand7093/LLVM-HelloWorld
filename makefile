
LLVM = `llvm-config --cxxflags --ldflags --libs --system-libs core`

EXE = hello-llvm

all: $(EXE)

$(EXE): $(EXE).o
	$(CC) $^ -o $@

$(EXE).o: $(EXE).ir
	llc -filetype=obj $^ -o $@

$(EXE).ir: llvmtest
	./$^ &> $@

llvmtest: main.cpp
	$(CXX) -g main.cpp $(LLVM) -o $@

clean:
	@rm -rf *.dSYM *~ \#*\# llvmtest *.o *.ir $(EXE)
