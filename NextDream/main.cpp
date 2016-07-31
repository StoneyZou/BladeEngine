
#include <TMap.h>
#include <BString.h>
#include <TArray.h>

struct BStringComapareFunc
{
public:
    static BladeEngine::int32 Compare(const BladeEngine::BString& lh, const BladeEngine::BString& rh)
    {
        return lh.Compare(rh);
    }
};

void main()
{
    BladeEngine::BString str("ddfsf");
    str = "dsscsdff";

    BladeEngine::TArray<void*> t;
    BladeEngine::TMap<BladeEngine::BString, BladeEngine::int32> test;

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

    BladeEngine::TMap<BladeEngine::BString, BladeEngine::int32>::Iterator ite = test.Find("R");
    if(ite != test.End())
    {
        return;
    }
}