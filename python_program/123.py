import hashlib
md5=hashlib.md5('123'.encode('utf-8')).hexdigest()
print(md5)
#求文件md5
file='C:\QcOSD.txt'
try:
    md5file=open(file,'rb')
    md5=hashlib.md5(md5file.read()).hexdigest()
    md5file.close()
    print(md5)
except:
    print('file not exit!')
