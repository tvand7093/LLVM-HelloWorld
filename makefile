
LLVM = `llvm-config --cxxflags --ldflags --libs --system-libs core`

llvmtest:  main.cpp
	$(CXX) -g main.cpp $(LLVM) -o $@

clean:
	@rm -rf *.dSYM *~ \#*\# llvmtest
