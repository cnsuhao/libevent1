file='D:\cyp\matlab\pic_a'
file_read=open(file,'rb')
file_write=open("xxx.dat",'wb')
print(type(file_write))
while True:
    s=file_read.read(4096)
    if len(s)<1024:
        file_read.close()
        file_write.write(s)
        file_write.close()
        break
        print("end")
    else:
        file_write.write(s)
        #fortune = bytearray(s, encoding='ascii')
        #print(s)

