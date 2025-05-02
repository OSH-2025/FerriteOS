# Programming Instance<a name="EN-US_TOPIC_0302389230"></a>

## Instance Description<a name="section1260725555919"></a>

This instance shows how to run C++ code on Huawei LiteOS.

1.  Write the C++ code.
2.  Before running the C++ code, use the  **NO\_SCATTER**  parameter in the app\_init function to call LOS\_CppSystemInit to initialize the C++ constructor.
3.  Call the written C++ code after LOS\_CppSystemInit.

## Example Code<a name="section1053156161411"></a>

Prerequisite: C++ is enabled by running the  **make menuconfig**  command.

The C++ code is as follows:

```c++
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

using namespace std;

class TestClass {
public:
    TestClass(int arg);
    ~TestClass(void);
    void PrintTest(void);
    void StringTest(void);
    void MapTest(void);
private:
    int intTest;
    string stringTest;
    map<string, int> mapTest;
};
TestClass::TestClass(int arg)
{
    cout << "TestClass is constructed here, arg = " << arg << endl;
    intTest = arg;
}
TestClass::~TestClass(void)
{
    cout << "TestClass is destructed" << endl;
}
void TestClass::PrintTest(void)
{
    cout << __FUNCTION__ << " enter" << endl;
    cout << " intTest = " << this->intTest << endl;
}
void TestClass::StringTest(void)
{
    cout << __FUNCTION__ << " enter" << endl;
    string a("Lite");
    string b("OS");
    string c("LiteOS");
    if (a != b) {
        cout << " " << a << " != " << b << endl;
    }
    a += b;
    if (a == c) {
        cout << " " << a << " == " << c << endl;
    }
}
void TestClass::MapTest(void)
{
    cout << __FUNCTION__ << " enter" << endl;
    mapTest.insert(pair<string, int>("Huawei", 1));
    mapTest.insert(pair<string, int>("LiteOS", 2));
    mapTest.insert(pair<string, int>("Open", 3));
    mapTest.insert(pair<string, int>("Source", 4));
    cout << " show map key&value" << endl;
    for (auto &it : mapTest) {
        cout << " " << it.first << " " << it.second << endl;
    }
    mapTest["LiteOS"] = 8; /* 8: new value */
    cout << " change value of \"LiteOS\" key" << endl;
    for (auto &it : mapTest) {
        cout << " " << it.first << " " << it.second << endl;
    }
}

void CppTestEntry(void)
{
    cout << "LiteOS cpp sample start" << endl;
    TestClass test(123);
    test.PrintTest();
    test.StringTest();
    test.MapTest();
    cout << "LiteOS cpp sample stop" << endl;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
```

After the C++ constructor is initialized in  **app\_init**, call the C++ code. The code is as follows:

```c
void app_init(void)
{
    ......
/* Initialize the C++ constructor. */
    LOS_CppSystemInit((UINT32)&__init_array_start__, (UINT32)&__init_array_end__, NO_SCATTER);
/* Call the C++ code. */
    CppTestEntry();
    ......
}
```

## Verification<a name="section034935085"></a>

Run the CppTestEntry\(\) function. The verification result is as follows:

```
LiteOS cpp sample start
TestClass is constructed here, arg = 123
PrintTest enter
 intTest = 123
StringTest enter
 Lite != OS
 LiteOS == LiteOS
MapTest enter
 show map key&value
 Huawei 1
 LiteOS 2
 Open 3
 Source 4
 change value of "LiteOS" key
 Huawei 1
 LiteOS 8
 Open 3
 Source 4
LiteOS cpp sample stop
TestClass is destructed
```

## Complete Code<a name="section20680141585"></a>

[TestClass.zip](resource/TestClass.zip)

