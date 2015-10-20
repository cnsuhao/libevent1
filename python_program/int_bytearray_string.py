#bytearray to int 
aa=bytearray.fromhex('00 00 04 00')
int.from_bytes(aa, byteorder='big', signed=False)

#int to bytearray
import struct
bytes=struct.pack('i',1024)
#int to string
str(1024)
#string to int
int("1024")

#string to bytearray
fortune = bytearray("1", encoding='ascii')


#bytearray to string
qs1 = str(b'\x30', encoding = "utf-8")