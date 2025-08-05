#include "TestFilter.h"
#include "Effects.h"
#include "WICTextureLoader11.h"

TestFilter::TestFilter() : CSFilter()
{

}


void TestFilter::InitEffect()
{
	effect = Effects::testFX;
}
