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

### 1. PE header

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

NT header结构：

```C
struct _IMAGE_NT_HEADERS {
    DWORD                   Signature;
    IMAGE_FILE_HEADER       FileHeader;
    IMAGE_OPTIONAL_HEADER   OptionalHeader; // pe和pe+在此区分
};
```

##### 1. Signature

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

重要字段：

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
  > exe文件拥有自己的虚拟空间，能准确加载到ImageBase中；dll无法保证一定会被加载到ImageBase。
+ `SectionAlignment`: 节区在内存中的起始位置必为该值的整数倍。
+ `FileAlignment`: 节区在文件中的起始位置必为该值的整数倍。
+ `SizeOfImage`: 指定PE映像在虚拟内存中所占空间大小。
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

  + DataDirectory[0] = Export Directory
  + DataDirectory[1] = Import Directory
  + DataDirectory[5] = Base Relocation

#### 4. section header

节区头是由`_IMAGE_SECTION_HEADER`结构体组成的数组，每个结构体对应一个节区。

> 节区头并不一定紧邻在optional header之后。节区头的确切起始地址是由optional header的起始地址加上file header中的`SizeOfOptionalHeader`得到。这样可以在optional header和节区头之间插入解压缩代码。

```C
#define IMAGE_SIZEOF_SHORT_NAME 8

struct _IMAGE_SECTION_HEADER {
    BYTE  Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
        DWORD PhysicalAddress;
        DWORD VirtualSize;          // import
    } Misc;
    DWORD VirtualAddress;           // import
    DWORD SizeOfRawData;            // import
    DWORD PointerToRawData;         // import
    DWORD PointerToRelocations;
    DWORD PointerToLinenumbers;
    WORD  NumberOfRelocations;
    WORD  NumberOfLinenumbers;
    DWORD Characteristics;          // import
};
```

重要字段：

+ `VirtualSize`: 内存中节区大小(不算NULL padding)
+ `VirtualAddress`: 内存中节区起始地址(RVA)
+ `SizeOfRawData`: 文件中节区大小(算上NULL padding)
+ `PointerToRawData`: 文件中节区起始位置
+ `Characteristics`: 节区属性(bit OR)

> `Name`字段中可以放入任意值，不一定以NULL结束，也可以全填充NULL值。
>
> `VirtualSize`可能比`SizeOfRawData`更大。此时无法根据内存地址(RVA)推算出文件地址(RAW)。

### 2. IAT

#### 1. IID (IMAGE_IMPORT_DESCRIPTOR)

```C
struct _IMAGE_IMPORT_DESCRIPTOR {
    union {
      DWORD Characteristics;
      DWORD OriginalFirstThunk;   // import     INT
    };
    DWORD TimeDateStamp;
    DWORD ForwarderChain;
    DWORD Name;                   // import     DLL name
    DWORD FirstThunk;             // import     IAT
};
```

导入多少个库就存在多少个IID结构体，这些结构体形成了数组，由optional header中的`DataDirectory[1].VirtualAddress`指定其在内存中的RVA，由RVA转换得到RAW，即为在文件中的储存地址。该结构体数组最后以NULL结构体结束。

> 注意`DataDirectory[1].Size`中的大小不只是IID结构体数组的大小，这个大小是整个IAT的大小，里面还包括了导入DLL名称和函数名称等信息。

重要字段：

+ `OriginalFirstThunk`: Import Name Table(INT) address.(数组成员大小为`size_t`)
+ `Name`: Library name string address.
+ `FirstThunk`: Import Address Table(IAT) address.(数组成员大小为`size_t`)

> 以上地址都为RVA。

#### 2. INT

IID中`OriginalFirstThunk`数组的每个成员都是一个指向`_IMAGE_IMPORT_BY_NAME`结构体的RVA的指针。

```C
struct _IMAGE_IMPORT_BY_NAME {
    WORD Hint;      // ordinal
    BYTE Name[1];
};
```

#### 3. IAT

IID中`FirstThunk`数组的每个成员都是一个指向导入函数地址的指针。

> INT与IAT数组的大小通常相等(具有相同的成员个数，且成员大小都为`size_t`)。

IAT装载顺序：

1. 读取IID的`Name`成员，获取DLL名称字符串
2. 装载相应DLL(`LoadLibrary`)
3. 读取IDD的`OriginalFirstThunk`成员，获取INT地址
4. 按顺序读取INT数组的值，获取相应`_IMAGE_IMPORT_BY_NAME`地址(RVA)
5. 由`_IMAGE_IMPORT_BY_NAME`中的函数序列号或函数名获取相应函数的起始地址(`GetProcAddress()`)
6. 读取IID的`FirstThunk`成员，获取IAT地址
7. 按顺序将获得的函数地址装载入IAT数组
8. 重复以上步骤4-7，直到INT结束(即遇到NULL结构体时)

> 当PE装载器无法通过`OriginalFirstThunk`查找到INT时，就会尝试通过`FirstThunk`查找，此时INT与IAT是相同区域。

### 3. EAT [Optional]

PE文件中仅有一个用来说明EAT的结构体。(区别于IAT的结构体数组)

```C
struct _IMAGE_EXPORT_DIRECTORY {
    DWORD Characteristics;
    DWORD TimeDateStamp;
    WORD MajorVersion;
    WORD MinorVersion;
    DWORD Name;
    DWORD Base;
    DWORD NumberOfFunctions;        //import
    DWORD NumberOfNames;            //import
    DWORD AddressOfFunctions;       //import
    DWORD AddressOfNames;           //import
    DWORD AddressOfNameOrdinals;    //import
};
```

> 注意`DataDirectory[0].Size`中的大小不只是IED结构体数组的大小，这个大小是整个EAT的大小，里面还包括了导出DLL名称和函数名称等信息。

重要字段：

+ `NumberOfFunctions`: 实际导出函数个数。
+ `NumberOfNames`: 导出函数中具名函数的个数。
+ `AddressOfFunctions`: 导出函数地址数组(即EAT)，成员为导出函数地址的RVA，个数为`NumberOfFunctions`。
+ `AddressOfNames`: 函数名称地址数组，成员为函数名称字符串地址的RVA，个数为`NumberOfNames`。
+ `AddressOfNameOrdinals`: 序数数组，成员为`WORD`类型的函数序数，个数为`NumberOfNames`。

`GetProcAddress`获取函数地址原理：

1. 通过`AddressOfNames`找到函数名称字符串地址。
2. 依次比较每个字符串，查找指定名称，记此时的数组索引为name_index
3. 通过`AddressOfNameOrdinals`找到序数数组。
4. 在序数数组中通过name_index查找指定函数的序数。
5. 通过`AddressOfFunctions`找到EAT。
6. 在EAT中用函数序数作数组索引，获得指定函数的起始地址。

### 4. Relocation Table [Optional]

使程序中硬编码的内存地址随当前ImageBase变化而改变的处理过程就是PE重定位。

Relocation Table中有若干个IBR结构体。

```C
struct _IMAGE_BASE_RELOCATION {
    DWORD VirtualAddress;       // 基准RVA
    DWORD SizeOfBlock;
    // WORD TypeOffset[N];
};
```

每个IBR结构体后面紧跟一个`WORD`数组，数组的结尾不一定是0x0000，数组的大小由`SizeOfBlock`决定，即`N = (SizeOfBlock - 2*sizeof(DWORD)) / sizeof(WORD)`。IBR结构体的个数由`DataDirectory[5].Size`中的大小决定，不像IAT中的IID结构体数组那样以NULL结构体结束。

`TypeOffset`的高4位是Type，低12位是Offset。实际在装载PE文件到内存映像时，需要修改的RVA地址由`TypeOffset`数组前面紧邻的IBR结构体中的`VirtualAddress`(基准RVA)加上Offset得到。

### *5. 删除某个节区需要做的操作

1. 将对应节区头、节区体写0
2. 修改`FileHeader`中的`NumberOfSections`数量
3. 修改`OptionalHeader`中的`SizeOfImage`大小，其值为原值减去`SectionHeader`中的`VirtualSize`大小补齐到`OptionalHeader`中的`SectionAlignment`最小整数倍的大小。
