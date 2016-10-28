from Crypto.Cipher import AES
from Crypto import Random
import struct

with open("prueba","r+b") as archivo:
	archivo.seek(0,2)
	iv=Random.new().read(AES.block_size)
	print(iv)
	llave="mi contrasena si"
	mensaje="secreto"
	archivo.write(struct.pack(str(len(iv))+'s',iv))
	archivo.write(struct.pack(str(len(mensaje))+'s',AES.new(llave.encode('utf-8'),AES.MODE_CFB,iv).encrypt(mensaje.encode('utf-8'))))
