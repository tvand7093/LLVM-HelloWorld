#include "llvm/IR/Module.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Support/type_traits.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Bitcode/ReaderWriter.h"


using namespace llvm;

void doMoreWork(std::string outputFile) {
  LLVMContext TheContext;
  std::unique_ptr<Module> TheModule =
    llvm::make_unique<Module>("Hello LLVM!", TheContext);
  IRBuilder<> Builder(TheContext);
  
  // Make the function type:  double(double,double) etc.
  FunctionType *FT =
    FunctionType::get(Type::getDoubleTy(TheContext), false);
  
  Function *TheFunction =
    Function::Create(FT, Function::ExternalLinkage, "getThree", TheModule.get());

  // Create a new basic block to start insertion into.
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
  Builder.SetInsertPoint(BB);

  Value *RetVal = ConstantFP::get(TheContext, APFloat(3.0));

  Builder.CreateRet(RetVal);

  // Validate the generated code, checking for consistency.
  verifyFunction(*TheFunction);

  //now create the main function
  FunctionType *mainFuncType =
    FunctionType::get(Type::getInt32Ty(TheContext), false);

  Function *mainFunc =
    Function::Create(mainFuncType, Function::ExternalLinkage, "main", TheModule.get());

  BasicBlock *mainBody = BasicBlock::Create(TheContext, "main", mainFunc);
  Builder.SetInsertPoint(mainBody);

  Value *mainReturn = ConstantInt::get(Type::getInt32Ty(TheContext), 0);

  Builder.CreateRet(mainReturn);
  verifyFunction(*mainFunc);

  std::error_code EC;
  StringRef path(outputFile);
  raw_fd_ostream output(path, EC, sys::fs::F_None);

  TheModule.get()->print(output, nullptr);
}

int main(int argc, char**argv) {
  std::string outputFile("output.bc")
    ;
  if(argc >= 2){
    outputFile = std::string(argv[1]);
  }
  doMoreWork(outputFile);
  return 0;
}
