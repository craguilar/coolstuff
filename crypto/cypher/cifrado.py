#!/usr/bin/python

from   Crypto import Random
from   Crypto.Cipher import Blowfish
import struct

key = "ABCD123"

def  descifraLinea(linea):
	content=""
	bs = Blowfish.block_size
	buff=struct.unpack(str(bs)+'s',linea[0:bs])[0]
	blowfish = Blowfish.new(key, Blowfish.MODE_CBC, buff)
	buff=struct.unpack(str(len(linea[bs:]))+'s',linea[bs:])[0]
	print(str(len(buff)))
	content=blowfish.decrypt(struct.unpack(str(len(buff))+'s',buff)[0])
	print(content)
	return content.decode('utf-8')


def  cifrarLinea(linea):
	bs = Blowfish.block_size
	iv = Random.new().read(bs)
	cipher = Blowfish.new(key, Blowfish.MODE_CBC, iv)
	plaintext = linea.encode('utf-8')
	plen = bs - divmod(len(plaintext),bs)[1]
	padding = [8]*plen
	padding   = struct.pack('b'*plen, *padding)
	msg = cipher.encrypt(plaintext+ padding)
	return iv+msg


msgcifrado=cifrarLinea("hola")
with open("prueba","a+b") as db:
	db.write(msgcifrado+"\n".encode('utf-8'))
	db.close()
with open("prueba","r+b") as db:
	for linea in db:
		print(descifraLinea(linea[0:len(linea)-1]))		
	db.close()
