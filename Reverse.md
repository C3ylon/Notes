# Reverse

## PE

+ pe：32位可执行文件
+ pe+/pe32+：64位可执行文件（不叫作pe64）

| pe文件种类 | 扩展名 |
| :---: | :---: |
| 可执行 | exe、scr |
| 库 | dll |
| 驱动 | sys |
| 对象文件 | obj |

### PE header

pe header包括dos header、dos stub、NT header、section header（节区头），剩下的节区称为pe体。各节区头定义了各节区在文件/内存中的大小、位置、属性等。

文件加载到内存时节区的大小、位置会发生变化。

pe header与各节区的尾部存在NULL　padding。在文件／内存中节区的起始位置都在各文件／内存最小单位的倍数位置上。

#### 1. dos header

重要结构体成员：

+ 0x00~0x01 e_magic `4D 5A`("MZ")
+ 0x3C~0x3F e_lfanew (offset to NT header)

其余结构体成员均可置0。

#### 2. dos stub

可选项，大小不固定，其大小由dos header末尾的偏移量决定。

由代码与数据混合而成，在DOS环境下执行pe文件将从dos stub起始处开始执行代码。

#### 3. NT header

##### 1. Signature

```C
struct _IMAGE_NT_HEADERS {
    DWORD                   Signature;
    IMAGE_FILE_HEADER       FileHeader;
    IMAGE_OPTIONAL_HEADER   OptionalHeader; // pe和pe+在此区分
};
```

Signature在pe与pe+中都是4字节大小，其值为`50 45 00 00`("PE"00)。

##### 2. FileHeader

32位与64位结构相同，大小都为0x14。（算上魔数，如果一行有16字节则到此结束长一行半）

```C
struct _IMAGE_FILE_HEADER {
    WORD    Machine;                // important
    WORD    NumberOfSections;       // important
    DWORD   TimeDateStamp;
    DWORD   PointerToSymbolTable;
    DWORD   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;   // important
    WORD    Characteristics;        // important
};
```

+ `Machine`在32位中为0x014C，代表Intel 386。在64位中为0x8664。
+ `NumberOfSections`一定要大于0，且与实际节区数量不同时会发生运行错误。
+ `SizeOfOptionalHeader`在32位中为0x00E0，在64位中为0x00F0。
  > `*(WORD*)(0x00 + *(DWROD*)0x3C + 0x14) == 0xF0`说明该pe文件为64位。
+ `Characteristics`中的重要标志位：
  + 0x0002: File is excutable
  + 0x2000: File is a DLL
  + 0x0100: 32 bit word machine
  + 0x0020: App can handle >2gb address

##### 3. OptionalHeader

32位与64位结构略有不同。32位大小为0xE0，64位大小为0xF0。（假定`IMAGE_NUMBEROF_DIRECTORY_ENTRIES`值为通用的16的情况下）

```C
struct _IMAGE_OPTIONAL_HEADER {
    WORD                 Magic;
    BYTE                 MajorLinkerVersion;
    BYTE                 MinorLinkerVersion;
    DWORD                SizeOfCode;
    DWORD                SizeOfInitializedData;
    DWORD                SizeOfUninitializedData;
    DWORD                AddressOfEntryPoint;
    DWORD                BaseOfCode;
    DWORD                BaseOfData;                  // 64位没有BaseOfData和ImageBase
    DWORD                ImageBase;                   // 代替的是QWORD ImageBase
    DWORD                SectionAlignment;
    DWORD                FileAlignment;
    WORD                 MajorOperatingSystemVersion;
    WORD                 MinorOperatingSystemVersion;
    WORD                 MajorImageVersion;
    WORD                 MinorImageVersion;
    WORD                 MajorSubsystemVersion;
    WORD                 MinorSubsystemVersion;
    DWORD                Win32VersionValue;
    DWORD                SizeOfImage;
    DWORD                SizeOfHeaders;
    DWORD                CheckSum;
    WORD                 Subsystem;
    WORD                 DllCharacteristics;
    DWORD                SizeOfStackReserve;          // 64位为QWORD
    DWORD                SizeOfStackCommit;           // 64位为QWORD
    DWORD                SizeOfHeapReserve;           // 64位为QWORD
    DWORD                SizeOfHeapCommit;            // 64位为QWORD
    DWORD                LoaderFlags;
    DWORD                NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
};
```

64位文件的可选头相较于32位文件的可选头，平替了两个双字的大小，并将四个双字扩展为四字，因此大小总体变大了0x10。

重要字段：

+ `Magic`: 32位中为0x010B，64位中为0x020B。
  > `*(WORD*)(0x00 + *(DWROD*)0x3C + 0x18) == 0x020B`说明该pe文件为64位。
+ `AddressOfEntryPoint`: EP的RVA值。
+ `ImageBase`: PE文件被加载入内存中时的优先装入位置。
+ `SectionAlignment`: 节区在内存中的起始位置必为该值的整数倍。
+ `FileAlignment`: 节区在文件中的起始位置必为该值的整数倍。
+ `SizeOfImage`: 指定PE image在虚拟内存中所占空间大小。
+ `SizeOfHeaders`: 整个PE头在内存/文件中的大小。该值也必须是FileAlignment的整数倍。同时该值也指出了第一节区在文件中的起始位置。
+ `Subsystem`:
  + 1: 驱动文件
  + 2: GUI文件
  + 3: CUI文件
+ `NumberOfRvaAndSizes`: 用于指定`IMAGE_NUMBEROF_DIRECTORY_ENTRIES`的大小，默认为16，但PE装载器仍会通过查看该值来识别数组大小。
+ `DataDirectory`:

  由`_IMAGE_DATA_DIRECTORY`结构体组成的数组，数组的每项都有被定义的值。

  ```C
  struct _IMAGE_DATA_DIRECTORY {
      DWORD VirtualAddress;
      DWORD Size;
  };
  ```
