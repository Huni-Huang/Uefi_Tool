Scope(\_SB){
    Device(HUNI)
    {
        Name(_HID, "PNP0C14")
        Name(_UID, "KOBE")

        Name(_WDG, Buffer(){
            //WMAA: Huni_WMI
            //35D15EA0-107C-458D-BE4A-D78E4A494D8A
            0xA0, 0x5E, 0xD1, 0x35, 0x7C, 0x10, 0x8D, 0x45, 0xBE, 0x4A, 0xD7, 0x8E, 0x4A, 0x49, 0x4D, 0x8A, //GUID
            0x41, 0x41, //ObjectId (AA)
            0x01, //InstanceCount (1)
            0x06,  //Flage 这个flag的定义可以累加，比如0x06，那就是2+4， 定义了执行method+返回string，后面method可以用WMxx ,如果flag是0x00,那么就是包含的文件,比如mof文件,使用WQxx
                  //#define WMIACPI_REGFLAG_EXPENSIVE   0x1
                  //#define WMIACPI_REGFLAG_METHOD      0x2
                  //#define WMIACPI_REGFLAG_STRING      0x04
                  //#define WMIACPI_REGFLAG_EVENT       0x08
            
            //Reference to MOF buffer,flage = 0x00,使用WQxx method
            0x21, 0x12, 0x90, 0x05, 0x66, 0xd5, 0xd1, 0x11, 0xb2, 0xf0, 0x00, 0xa0, 0xc9, 0x06, 0x29, 0x10, //GUID
            0x30, 0x31, //ObjectId (01)
            0x01, //InstanceCount (1)
            0x00  //Flags
        })

        OperationRegion(FUK1, SystemMemory, 0xFE0B0700, 0x100)
        Field(FUK1, ByteAcc, NoLock, Preserve)
        {
            Offset(0x10),
            HUN1, 8,
            HUN2, 8,
        }
        OperationRegion(FUK2, SystemIO, 0xB2, 2)
        Field(FUK2, ByteAcc, NoLock, Preserve)
        {
            HUN3, 8,
            HUN4, 8,
        }


        Method(WMAA, 3)
        {
            If (LEqual(Arg1, 1)){
                Store(0x11,HUN1)
                Store(0x11,HUN2)
                Store(0x11,HUN3)
                Store(0x11,HUN4)
                Return("Arg == 1")
            }
            If(LEqual(Arg1, 2)){
                Store(0x22,HUN1)
                Store(0x22,HUN2)
                Store(0x22,HUN3)
                Store(0x22,HUN4)
                Return("Arg == 2")
            }
            If(LEqual(Arg1, 3)){
                Store(0x33,HUN1)
                Store(0x33,HUN2)
                Store(0x33,HUN3)
                Store(0x33,HUN4)
                Return("Arg == 3")
            }
        }
        Name(WQ01, Buffer()
        {
            Include("OemPkg\HuniWmi\HuniMof.h")
        })
    }
}