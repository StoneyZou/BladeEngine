#include <RHIEnum.h>
#include <TArray.h>
#include <TMap.h>
#include <BString.h>
#include <Utility.h>
#include <RHIDevice.h>
#include <RHIShaderBase.h>

using namespace BladeEngine;

struct BStringComapareFunc
{
public:
    static int32 Compare(const BString& lh, const BString& rh)
    {
        return lh.Compare(rh);
    }
};

void main()
{
    RHI::RHIShaderCreateInfo info;

    BString str("ddfsf");
    str = "dsscsdff";

    TArray<void*> t;
    TMap<BString, int32, BStringComapareFunc> test;
    test.Insert("A", 100);
    test.Insert("B", 200);
    test.Insert("C", 300);
    test.Insert("D", 400);
    test.Insert("R", 500);
    test.Insert("F", 600);
    test.Insert("G", 700);
    test.Insert("H", 800);
    test.Insert("I", 900);

    test.Erase("A");
    test.Erase("B");
    test.Erase("C");
    test.Erase("D");
    test.Erase("R");
    test.Erase("F");
    test.Erase("G");
    test.Erase("H");
    test.Erase("I");
}