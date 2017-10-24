#include "testopenclprogram.h"
#include "opencldevice.h"
#include "openclprogram.h"
#include "openclkernel.h"






void TestOpenCLProgram::testFromSource()
{
   auto program {EOpenCLDevice::getInstance().makeProgram()};
   program->addSource("__kernel void dummy() {}");
   Q_ASSERT( program->compile() );
   Q_ASSERT( !program->hasBuildError() );
   auto kernel {program->makeKernel("dummy")};
   Q_ASSERT( (bool)*kernel );
   auto badKernel {program->makeKernel("not_exist")};
   Q_ASSERT( !(bool)*badKernel );
}






void TestOpenCLProgram::testFromFile()
{
   auto program {EOpenCLDevice::getInstance().makeProgram()};
   program->addFile(":/opencl/testkernel.cl");
   Q_ASSERT( program->compile() );
   Q_ASSERT( !program->hasBuildError() );
   auto kernel {program->makeKernel("hello")};
   Q_ASSERT( (bool)*kernel );
   auto badKernel {program->makeKernel("not_exist")};
   Q_ASSERT( !(bool)*badKernel );
}






void TestOpenCLProgram::testBadCompile()
{
   auto program {EOpenCLDevice::getInstance().makeProgram()};
   program->addSource("invalid kernel source >:))");
   Q_ASSERT( !program->compile() );
   Q_ASSERT( program->hasBuildError() );
   Q_ASSERT( !program->getBuildError().isEmpty() );
}
