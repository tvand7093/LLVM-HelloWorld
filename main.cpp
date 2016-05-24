#include "llvm/IR/Module.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/DataLayout.h"

#include "llvm/ADT/STLExtras.h"

#include "llvm/Bitcode/ReaderWriter.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/type_traits.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/FileSystem.h"

#include <iostream>

using namespace llvm;

void printfMethod(Module *module, LLVMContext &context, IRBuilder<> builder, Value* toPrint){

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
  //set the printf arguement values
  values.push_back(formatStr);
  values.push_back(toPrint);

  //add the printf call
  builder.CreateCall(printfFunc, values);
}

Function* makeGetThreeFunc(Module *module, LLVMContext &ctx){
  IRBuilder<> builder(ctx);

  Type* intType = Type::getInt8Ty(ctx);
  
  FunctionType *funcType =
    FunctionType::get(intType, false);
  
  Function *getThree =
    Function::Create(funcType, Function::ExternalLinkage, "getThree", module);

  // Create a new basic block to start insertion into.
  BasicBlock *codeBlock = BasicBlock::Create(ctx, "entry", getThree);
  builder.SetInsertPoint(codeBlock);
  
  Value *RetVal = ConstantInt::get(intType, 3);

  builder.CreateRet(RetVal);

  // Validate the generated code, checking for consistency.
  verifyFunction(*getThree);
  return getThree;
}

void outputIR(Module *module, std::string outputFile){
  std::error_code EC;
  StringRef path(outputFile);
  raw_fd_ostream output(path, EC, sys::fs::F_None);

  module->print(output, nullptr);
}

void makeProgram(std::string outputFile) {
  LLVMContext TheContext;
  std::unique_ptr<Module> TheModule =
    llvm::make_unique<Module>("Hello LLVM!", TheContext);
  IRBuilder<> Builder(TheContext);

  //create the  getThree function
  Function* getThree = makeGetThreeFunc(TheModule.get(), TheContext);

  //now create the main function
  FunctionType *mainFuncType =
    FunctionType::get(Type::getInt32Ty(TheContext), false);

  Function *mainFunc =
    Function::Create(mainFuncType, Function::ExternalLinkage, "main", TheModule.get());

  //create the body of main
  BasicBlock *mainBody = BasicBlock::Create(TheContext, "main", mainFunc);
  Builder.SetInsertPoint(mainBody);

  //set the return value of main
  Value *mainReturn = ConstantInt::get(Type::getInt32Ty(TheContext), 0);

  //get the value of calling the getThree function.
  std::vector<Value*> argValues;
  Value* callThreeResult = Builder.CreateCall(getThree, argValues, "calltmp");

  //do a printf
  printfMethod(TheModule.get(), TheContext, Builder, callThreeResult);

  Builder.CreateRet(mainReturn);
  verifyFunction(*mainFunc);
  outputIR(TheModule.get(), outputFile);
}

int main(int argc, char**argv) {
  std::string outputFile("output.bc");
    
  if(argc >= 2){
    outputFile = std::string(argv[1]);
  }
  makeProgram(outputFile);
  return 0;
}
