// #include "llvm/IR/Module.h"
// #include "llvm/IR/Function.h"
// #include "llvm/IR/PassManager.h"
// #include "llvm/IR/CallingConv.h"
// #include "llvm/IR/Verifier.h"
// #include "llvm/IR/IRPrintingPasses.h"
// #include "llvm/IR/IRBuilder.h"
// #include "llvm/Support/raw_ostream.h"

// using namespace llvm;

// Module* makeLLVMModule();

// int main(int argc, char**argv) {
//   Module* Mod = makeLLVMModule();

//   verifyModule(*Mod, PrintMessageAction);

//   PassManager<Module> PM;
//   PM.addPass(createPrintModulePass(outs()));
//   PM.run(*Mod);

//   delete Mod;
//   return 0;
// }


#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

int main()
{
  llvm::LLVMContext& context = llvm::getGlobalContext();
  llvm::Module* module = new llvm::Module("top", context);
  llvm::IRBuilder<> builder(context);

  module->dump( );
}
