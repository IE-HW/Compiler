#include <C:\Program Files\LLVM\include\llvm-c/core.h>
#include <llvm-c/BitWriter.h>

// تابع برای تولید کد IR از AST
void generateIRFromAST() {
    // ایجاد یک ماژول جدید
    LLVMModuleRef module = LLVMModuleCreateWithName("my_module");

    // تعریف نوع داده integer
    LLVMTypeRef intType = LLVMInt32Type();

    // تعریف متغیرها
    LLVMValueRef varA = LLVMAddGlobal(module, intType, "a");
    LLVMValueRef varB = LLVMAddGlobal(module, intType, "b");
    LLVMValueRef varC = LLVMAddGlobal(module, intType, "c");

    // تعریف بلوک‌ها
    LLVMBasicBlockRef entryBlock = LLVMAppendBasicBlock(module, "entry");
    LLVMBasicBlockRef calcBlock = LLVMAppendBasicBlock(module, "calc");
    LLVMBasicBlockRef endBlock = LLVMAppendBasicBlock(module, "end");

    // تعریف تابع اصلی
    LLVMTypeRef mainFunctionType = LLVMFunctionType(LLVMVoidType(), NULL, 0, 0);
    LLVMValueRef mainFunction = LLVMAddFunction(module, "main", mainFunctionType);
    LLVMSetLinkage(mainFunction, LLVMExternalLinkage);
    LLVMValueRef mainFunctionEntryBlock = LLVMAppendBasicBlock(mainFunction, "entry");

    // ساخت برنامه‌نویسی توابع
    LLVMBuilderRef builder = LLVMCreateBuilder();

    // قرار دادن دستورات در بلوک‌ها
    LLVMPositionBuilderAtEnd(builder, entryBlock);
    LLVMBuildBr(builder, calcBlock);

    LLVMPositionBuilderAtEnd(builder, calcBlock);
    LLVMValueRef cMinusBC = LLVMBuildMul(builder, varB, varC, "tmp1");
    LLVMValueRef cMinusBCMinusA = LLVMBuildSub(builder, cMinusBC, varA, "tmp2");
    LLVMValueRef result = LLVMBuildAdd(builder, varC, cMinusBCMinusA, "result");
    LLVMBuildStore(builder, result, varC);
    LLVMBuildBr(builder, endBlock);

    LLVMPositionBuilderAtEnd(builder, endBlock);
    LLVMBuildRetVoid(builder);

    // تعیین بلوک ورودی برای تابع اصلی
    LLVMPositionBuilderAtEnd(builder, mainFunctionEntryBlock);
    LLVMBuildCall(builder, mainFunction, NULL, 0, "");

    // رفع خطاهای مربوط به ماژول
    char *error = NULL;
    LLVMPrintModuleToFile(module, "output.ll", &error);

    // پاک کردن ماژول
    LLVMDisposeModule(module);

    // پاک کردن سایر منابع
    LLVMDisposeBuilder(builder);
}

int main() {
    // فراخوانی تابع برای تولید کد IR از AST
    generateIRFromAST();

    return 0;
}
