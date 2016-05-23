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

using namespace llvm;

void doMoreWork() {
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
  
  TheModule->dump();
}


// void doWork(){
//   // Have your "name" for the module set.

//   // Get the global LLVM context.
//   llvm::LLVMContext& ctx = llvm::getGlobalContext();

//   // Create a new module for our function.
//   llvm::Module* module = new llvm::Module("Hello World Module", ctx);

//   // Get the LLVM type for a 32-bit floating point value.
//   llvm::Type* f32
//     = llvm::Type::getPrimitiveType(ctx, llvm::Type::FloatTyID);

//   // Create a function type which returns a float, and takes
//   // no parameters.
//   llvm::FunctionType* ftype
//     = llvm::FunctionType::get(f32, false);

//   // Add a new function to the symbol table with the type that
//   // we created.
//   llvm::Constant* func
//     = module->getOrInsertFunction("two", ftype);

//   llvm::Function* realFunc = llvm::dyn_cast<llvm::Function>(func);
//   //check cast

//   // Create a single basic block for our function.
//   llvm::BasicBlock* basicBlock
//     = llvm::BasicBlock::Create(ctx, "block 1", realFunc);

//   // Hardcode the return value for our function.
//   llvm::Value* retVal
//     = llvm::ConstantFP::get(f32, 2.0);

//   // Create the return instruction within our basic block.
//   llvm::ReturnInst* retInst
//     = llvm::ReturnInst::Create(ctx, retVal, basicBlock);
  
//   //set filename up before this.
//   llvm::PassManager<llvm::DataLayout> passes;
//   std::error_code error;

//   llvm::StringRef filename("MyFile.txt");
  
//   llvm::tool_output_file *FDOut = new llvm::tool_output_file(filename, 1);

//   llvm::formatted_raw_ostream outstream(FDOut->os());

//   passes.addPass(*(new llvm::DataLayout(module)));
//   passes.addPass(llvm::createPrintModulePass(outstream));
//   passes.run(*module);

//   FDOut->keep();
// }


int main(int argc, char**argv) {
  doMoreWork();
  return 0;
}
