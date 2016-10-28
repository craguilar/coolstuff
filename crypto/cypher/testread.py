from Crypto.Cipher import AES
from Crypto import Random
import struct

with open("prueba","r+b") as archivo:
	iv=struct.unpack('16s',archivo.read(16))[0]
	llave="mi contrasena si"
	#print(iv)
	print(AES.new(llave.encode('utf-8'),AES.MODE_CFB,iv).decrypt(struct.unpack('7s',archivo.read(7))[0]).decode('utf-8'))
	#print(struct.unpack('7s',archivo.read(7)))
