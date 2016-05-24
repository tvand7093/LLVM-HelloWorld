#include "llvm/IR/Module.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Support/type_traits.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Bitcode/ReaderWriter.h"

using namespace llvm;

void printfMethod(Module *module, LLVMContext &context, IRBuilder<> &builder, Value* toPrint){

  if(!toPrint) return;
  
  // accepts a char*, is vararg, and returns int
  std::vector<llvm::Type *> args;
  args.push_back(llvm::Type::getInt8PtrTy(context));
  
  //create the entry in the text section.
  llvm::Value *formatStr = builder.CreateGlobalStringPtr("value = %d\n");
  
  // get the method type, based on retrun value, given an arg list
  llvm::FunctionType *printfType =
    llvm::FunctionType::get(builder.getInt32Ty(), args, true);

  //get the actual printf function using the previous type
  llvm::Constant *printfFunc =
    module->getOrInsertFunction("printf", printfType);

  //set the values to pass into the printf call as parameters
  std::vector<llvm::Value *> values;
  //just an integer 3 value
  values.push_back(formatStr);
  values.push_back(toPrint);
  
  //add the printf call
  builder.CreateCall(printfFunc, values);
}

void doMoreWork(std::string outputFile) {
  LLVMContext TheContext;
  std::unique_ptr<Module> TheModule =
    llvm::make_unique<Module>("Hello LLVM!", TheContext);
  IRBuilder<> Builder(TheContext);
  
  // Make the function type:  double(double,double) etc.
  FunctionType *FT =
    FunctionType::get(Type::getInt8Ty(TheContext), false);
  
  Function *TheFunction =
    Function::Create(FT, Function::ExternalLinkage, "getThree", TheModule.get());

  // Create a new basic block to start insertion into.
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
  Builder.SetInsertPoint(BB);
  
  Value *RetVal = ConstantInt::get(Type::getInt8Ty(TheContext), 3);

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

  //get the value of calling the getThree function.
  Function *CalleeF = TheModule->getFunction("getThree");

  std::vector<Value*> argValues;
  Value* callThreeResult = Builder.CreateCall(CalleeF, argValues, "calltmp");
  
  //do a printf
  printfMethod(TheModule.get(), TheContext, Builder, callThreeResult);
  
  Builder.CreateRet(mainReturn);
  verifyFunction(*mainFunc);

  std::error_code EC;
  StringRef path(outputFile);
  raw_fd_ostream output(path, EC, sys::fs::F_None);

  TheModule.get()->print(output, nullptr);
}

int main(int argc, char**argv) {
  std::string outputFile("output.bc");
    
  if(argc >= 2){
    outputFile = std::string(argv[1]);
  }
  doMoreWork(outputFile);
  return 0;
}
