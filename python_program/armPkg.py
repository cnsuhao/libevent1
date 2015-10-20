import hashlib
import os
import struct
##packet_start=bytearray.fromhex('dd cc bb aa')
#print(type(packet_end))

def arm_package_update_file_data(datain):
    start='start'
    packettype='3'
    bytes=struct.pack('i',1024)
    data= str(datain, encoding = "utf-8")
    end='end'
    return start+'|'+packettype+'|'+data_lenth+'|'+data+'|'+end
def file_read_send(file):
    file_read=open(file,'rb')
def arm_package_update_assign(file_addr,update_mod,version_in):
    start='start'
    packettype='1'
    md5_sum=file_digest(file_addr)
    update_mod=update_mod
    file_size=str(os.path.getsize(file_addr))
    vesion=version_in
    end='end'
    return start+'|'+packettype+'|'+md5_sum+'|'+update_mod+'|'+\
           file_size+'|'+vesion+'|'+end
def file_digest(filename):
    try:
        md5file=open(file,'rb')
        md5=hashlib.md5(md5file.read()).hexdigest()
        md5file.close()
        return md5
    except:
        return None
def string_digest(strname):
    md5=hashlib.md5(strname.encode('utf-8')).hexdigest()
    return md5
def get_filesize(filePath):
    for r,ds,fs in os.walk(filePath):
        for f in fs:
            size=os.path.getsize(os.path.join(r,f))
if __name__ == "__main__":
    file='D:\cyp\matlab\pic_a'
    print(arm_package_update_file_data(file))
    print(arm_package_update_assign(file,'fpga','v11'))    
    input()

##packet_end=bytearray.fromhex('aa bb cc dd')
##print(packet_start+packet_end)
##packet_end.append(int('0xaa',16))
##packet_end.append(int('0xbb',16))
##packet_end.append(int('0xcc',16))
##packet_end.append(int('0xdd',16))
