#!/usr/bin/python
import getpass
import hashlib
import re
import abc
from   Crypto import Random
from   Crypto.Cipher import Blowfish
import struct

CONST_PRODUCTO 		= "producto"
CONST_CLIENTE		= "cliente"
CONST_DISTRIBUIDOR	= "distribuidor" 
CONST_USUARIO		= "usuario"

################# CHIN!!! MAL DISENO DEL PROGRAMA :( #############
##################################################################
key = "ABCD123"
############# UTILERIAS ##########################################
def letter_range(start, stop):
		for c in range(ord(start), ord(stop)):
			yield chr(c)

def  descifraLinea(linea):
	content=""
	bs = Blowfish.block_size
	buff=struct.unpack(str(bs)+'s',linea[0:bs])[0]
	blowfish = Blowfish.new(key, Blowfish.MODE_CBC, buff)
	buff=struct.unpack(str(len(linea[bs:]))+'s',linea[bs:])[0]
	content=blowfish.decrypt(struct.unpack(str(len(buff))+'s',buff)[0])
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


########### DEFINICION DE CLASES ##########################
class ObjetoAdministracion(object):
	__metaclass__  = abc.ABCMeta
	count=0
	def __init__(self):
		ObjetoAdministracion.count+=1	
	@abc.abstractmethod	
	def setDatos(self):
		"""ABSTRACTO"""
	@abc.abstractmethod
	def modifyDatos(self,linea):
		"""ABSTRACTO"""
	@abc.abstractmethod	
	def printLine(self,num, line):
		"""ABSTRACTO"""
	@abc.abstractmethod	
	def getFileName(self):
		"""ABSTRACTO"""
	@abc.abstractmethod
	def existInFile(self):
		"""ABSTRACTO"""
	@abc.abstractmethod
	def getID(self):
		"""ABSTRACTO"""	

##**------------------------------------------------------		
class Producto(ObjetoAdministracion):

	def __init__(self):
		self.nombrep=""
		self.preciop=""
		self.cantidad=""
		self.id_dist=""
		
	def  setDatos(self):
		cadena=""
		print("Por favor, ingresa los datos del producto")
		while 1:
			self.nombrep = input("Nombre: ")
			self.nombrep = re.sub(" ","",self.nombrep)
			if self.nombrep != "" and len(self.nombrep)<100:
				break
			print("EL nombre no puede ser nulo y debe ser menor a 100")
		while 1:
			self.preciop = input("Precio: ")
			try:
				if float(self.preciop)>0:
					break
				else:
					print("El precio no puede ser cero")
			except ValueError as e:
				print("El precio debe ser numerico no nulo")
		while 1:
			self.cantidad = input("Cantidad: ")
			try:
				if int(self.cantidad)>=0:
					break
				else:
					print("El cantidad menor que cero")
			except ValueError as e:
				print("El precio debe ser numerico no nulo")
		while 1:
			numd=adminDistribuidores.consulta()
			self.id_dist    = input("Indique:\n\t # de un ID_dist\n\t'X' No tiene distribuidor\nSeleccion>")
			if self.id_dist.lower() == "x":
				self.id_dist="-1"
				break
			elif len(self.id_dist) == 1 and self.id_dist.lower() not in letter_range('a','z') and int(self.id_dist) <= numd and int(self.id_dist) > 0:
		 		break
			else:
		        	print("Opcion invalida")
		print("<"+self.id_dist+">")
		cadena = ' '.join([str(self.getID()),self.nombrep,self.preciop,self.cantidad,self.id_dist])
		return cadena
	
	def printLine(self,num,line):
		(id_pro, nombre_pro, precio_pro,cantidad_pro,id_dist_pro)=line.split(" ")
		print('(#'+str(num)+')'+id_pro + ' ' + nombre_pro + ' $'+ precio_pro + ' ' + cantidad_pro + ' ' + id_dist_pro)

	def getFileName(self):
		return "productos"

	def existInFile(self):
		try:
			count=0
			with open(self.getFileName(),"r+b") as db:
				for linea in db:
					count=count+1
					linea=descifraLinea(linea[0:len(linea)-1])
					(id_pro, nombre_pro, precio_pro,cantidad_pro,id_dist_pro)=linea.split(" ")
					if nombre_pro == self.nombrep:
						print("El producto  ya existe!")
						return 1
		except IOError as e:
			print(" El archivo no existe se creara :)")
		return 0

	def getID(self):
		count=0
		try:
			with open(self.getFileName(),"r+b") as db:
				for linea in db:
					linea=descifraLinea(linea[0:len(linea)-1])
					(id_pro, nombre_pro, precio_pro,cantidad_pro,id_dist_pro)=linea.split(" ")
					count=int(id_pro)
			count+=1			
			db.close()
		except IOError as e:
			print("")
		return count
		
	def modifyDatos(self,linea):
		cadena=""
		(id_pro, nombre_pro, precio_pro,cantidad_pro,id_dist_pro)=linea.split(" ")
				
		self.nombrep=nombre_pro
		self.preciop=precio_pro
		self.cantidad=cantidad_pro
		self.id_dist=id_dist_pro
		while 1:
			print("Nombre:   {}".format(self.nombrep))
			print("Precio:   {}".format(self.preciop))
			print("Cantidad  {}".format(self.cantidad))
			print("Distribuidor {}".format(self.id_dist))
			print("¿Qué deseas modificar?")
			dato=input("(n)ombre, (p)recio, (c)antidad,(d)istribuidor (s)alir: ")
			if dato.lower()=='n':
				while 1:
					self.nombrep = input("Nombre: ")
					self.nombrep = re.sub(" ","",self.nombrep)
					if self.nombrep != "" and len(self.nombrep)<100:
						break
					print("EL nombre no puede ser nulo y debe ser menor a 100")
			elif dato.lower()=='p':
				while 1:
					self.preciop = input("Precio: ")
					try:
						if float(self.preciop)>0:
							break
						else:
							print("El precio no puede ser cero")
					except ValueError as e:
						print("El precio debe ser numerico no nulo")
			elif dato.lower()=='c':
				while 1:
					self.cantidad = input("Cantidad: ")
					try:
						if int(self.cantidad)>=0:
							break
						else:
							print("El cantidad menor que cero")
					except ValueError as e:
						print("El precio debe ser numerico no nulo")
			elif dato.lower()=='d':
				while 1:
					numd=adminDistribuidores.consulta()
					self.id_dist    = input("Indique:\n\t # de un ID_dist\n\t'X' No tiene distribuidor\nSeleccion>")
					if self.id_dist.lower() == "x":
						self.id_dist="-1\n"
						break
					elif len(self.id_dist)==1 and self.id_dist.lower() not in letter_range('a','z') and int(self.id_dist)<=numd and int(self.id_dist)> 0:
						self.id_dist+="\n"				 		
						break
					else:
						print("Opcion invalida")	
			elif dato.lower()=='s':
				break
			else:
				print("Opción no válida")

		cadena =id_pro+" "+self.nombrep + " " + self.preciop + " " + self.cantidad+" "+self.id_dist[0:len(self.id_dist)-1]
		return cadena
##**------------------------------------------------------		
class Cliente(ObjetoAdministracion):

	def __init__(self):
		self.nombrec=""
		self.rfc    =""
		
		
	def  setDatos(self):
		cadena=""
		print("Por favor, ingresa los datos del Cliente")
		while 1:
			self.nombrec = input("Nombre: ")
			self.nombrec = re.sub(" ","",self.nombrec)
			if self.nombrec != "" and len(self.nombrec)<100:
				break
			print("EL nombre no puede ser nulo y debe ser menor a 100")
		while 1:
			self.rfc = input("RFC:: ")
			if self.rfc != "" and len(self.rfc)<13:
				break
			print("El RFC no puede ser nulo y debe ser menor que 13")
		cadena=	' '.join([str(self.getID()),self.nombrec,self.rfc])	
		return  cadena
		

	def getFileName(self):
		return "clientes"

	def printLine(self,num, line):
		(id_cliente, nombre, rfcc)=line.split(" ")
		print('(#'+str(num)+')'+id_cliente + ' ' + nombre + ' '+ rfcc)
		
	def existInFile(self):
		try:
			count=0
			with open(self.getFileName(),"r+b")  as db:
				for linea in db:
					count=count+1
					linea=descifraLinea(linea[0:len(linea)-1])
					(id_file,cliente_file, rfc_fil)=linea.split(" ")
					if cliente_file == self.nombrec:
						print("El cliente ya existe!")
						return 1
		except IOError as e:
			print(" El archivo no existe se creara :)")
		return 0

	def getID(self):
		count=0
		try:
			with open(self.getFileName(),"r+b")  as db:
				for linea in db:
					linea=descifraLinea(linea[0:len(linea)-1])
					(id_file,cliente_file, rfc_fil)=linea.split(" ")
					count=int(id_file)
			count+=1
			db.close()
		except IOError as e:
			print("")
		return count
		
	def modifyDatos(self,linea):
		cadena=""
		(id_cliente, nombre, rfcc)=linea.split(" ")
		self.nombrec=nombre
		self.rfc    =rfcc
		while 1:
			print("Nombre:   {}".format(self.nombrec))
			print("RFC:      {}".format(self.rfc))	
			print("¿Qué deseas modificar?")
			dato=input("(n)ombre, (r)fc,(s)alir: \n?>")
			if dato.lower()=='n':
				while 1:
					self.nombrec = input("Nombre: ")
					self.nombrec = re.sub(" ","",self.nombrec)
					if self.nombrec != "" and len(self.nombrec)<100:
							break
					print("EL nombre no puede ser nulo y debe ser menor a 100")
			elif dato.lower()=='r':
				while 1:
					self.rfc = input("RFC:: ")
					if self.rfc != "" and len(self.rfc)<13:
						self.rfc+="\n"
						break
					print("El RFC no puede ser nulo y debe ser menor que 13")
			elif dato.lower()=='s':
				break
			else:
				print("Opción no válida")
		cadena=id_cliente+" "+self.nombrec+" "+self.rfc[0:len(self.rfc)-1]
		return cadena
##**------------------------------------------------------
class Distribuidor(ObjetoAdministracion):
	def __init__(self):
		self.nombred=""
		self.rfc    =""
		
	def  setDatos(self):
		cadena=""
		print("Por favor, ingresa los datos del Distribuidor")
		while 1:
			self.nombred = input("Nombre: ")
			self.nombred = re.sub(" ","",self.nombred)
			if self.nombred != "" and len(self.nombred)<100:
				break
			print("EL nombre no puede ser nulo y debe ser menor a 100")
		while 1:
			self.rfc = input("RFC:: ")
			if self.rfc != "" and len(self.rfc)<13:
				break
			print("El RFC no puede ser nulo y debe ser menor que 13")
		
		cadena=	' '.join([str(self.getID()),self.nombred,self.rfc])
		return  cadena

	def getFileName(self):
		return "distribuidores"

	def printLine(self,num, line):
		(id_distribuidor, nombre,rfct)=line.split(" ")
		print('(#'+str(num)+')'+id_distribuidor + ' ' + nombre + ' '+ rfct)
		
	def existInFile(self):
		try:
			count=0
			with open(self.getFileName(),"r+b")  as db:
				for linea in db:
					count=count+1
					linea=descifraLinea(linea[0:len(linea)-1])
					(id_file,distribuidor_file, rfc_fil)=linea.split(" ")
					if distribuidor_file == self.nombred:
						print("El distribuidor ya existe!")
						return 1
		except IOError as e:
			print(" El archivo no existe se creara :)")
		
		return 0

	def getID(self):
		count=0
		try:
			with open(self.getFileName(),"r+b")   as db:
				for linea in db:
					linea=descifraLinea(linea[0:len(linea)-1])
					(id_file,distribuidor_file, rfc_fil)=linea.split(" ")
					count=int(id_file)
			count+=1
			db.close
		except IOError as e:
			print("")
		return count

	def modifyDatos(self,linea):
		cadena=""
		(id_dist, nombre, rfcc)=linea.split(" ")
		self.nombred=nombre
		self.rfc    =rfcc
		while 1:
			print("Nombre:   {}".format(self.nombred))
			print("RFC:      {}".format(self.rfc))	
			print("¿Qué deseas modificar?")
			dato=input("(n)ombre, (r)fc,(s)alir: \n?>")
			if dato.lower()=='n':
				while 1:
					self.nombred = input("Nombre: ")
					self.nombred = re.sub(" ","",self.nombred)
					if self.nombred != "" and len(self.nombred)<100:
							break
					print("EL nombre no puede ser nulo y debe ser menor a 100")
			elif dato.lower()=='r':
				while 1:
					self.rfc = input("RFC:: ")
					if self.rfc != "" and len(self.rfc)<13:
						self.rfc+="\n"
						break
					print("El RFC no puede ser nulo y debe ser menor que 13")
			elif dato.lower()=='s':
				break
			else:
				print("Opción no válida")
		cadena=id_dist+" "+self.nombred+" "+self.rfc[0:len(self.rfc)-1] 
		return cadena
##**------------------------------------------------------

class Usuario(ObjetoAdministracion):
	def __init__(self):
		self.nombre=""
		self.contrasena=""
		self.perm=""
		
	def  setDatos(self):
		cadena=""
		print("Por favor, ingresa los datos del usuario")
		while 1:
			self.nombre = input("Nombre: ")
			self.nombre = re.sub(" ","",self.nombre)
			if self.nombre != "":
				break
		while 1:
			self.contrasena = getpass.getpass("Pass: ")
			if self.contrasena != "":
				break
		self.perm = input("Permisos ('admin' o 'mortal'):")

		elhash=hashlib.sha1()
		elhash.update(self.contrasena.encode('utf-8'))
		cadena = ' '.join([self.nombre,elhash.hexdigest(),self.perm])+'\n'
		return cadena

	def getFileName(self):
		return "contrasenas"

	def printLine(self,num, line):
		(usuario_file, contrasena_file, perm)=line.split(" ")
		print('(#'+str(num)+')'+usuario_file + ' ' + perm)
		
	def existInFile(self):
		try:
			with open(self.getFileName(),"r")  as db:
				for linea in db:
					(usuario_file, contrasena_file, perm)=linea.split(" ")
					if usuario_file == self.nombre:
						print("El usuario ya existe!")
						return 1
		except IOError as e:
			return 0
		return 0

	def modifyDatos(self, linea):
		cadena=""
		(usuario_file, contrasena_file, perm_file)=linea.split(" ")
		self.nombre=usuario_file
		self.contrasena=contrasena_file
		self.perm=perm_file
		while 1:
			print("Usuario: {}".format(self.nombre))
			print("Permisos: {}".format(self.perm))
			print("¿Qué deseas modificar?")
			dato=input("(u)suario, (p)ermisos, (c)ontraseña, (s)alir: ")
			if dato.lower()=='u':
				while 1:
					self.nombre=re.sub(" ","",input("Dame el nuevo usuario: "))
					if self.nombre!="":
						break
					print("Por favor introduce algo más que espacios en blanco")
			elif dato.lower()=='p':
				while 1:
					self.perm=input("Permisos ('admin' o 'mortal'):").lower()
					if self.perm in ('admin','mortal'):
						self.perm += "\n"
						break
					print("Por favor elige una opción válida")
			elif dato.lower()=='c':
				self.contrasena=hashlib.sha1(getpass.getpass("Por favor introduce tu contraseña").encode('utf-8')).hexdigest()
			elif dato.lower()=='s':
				break
			else:
				print("Opción no válida")

		cadena =self.nombre + " " + self.contrasena + " " + self.perm
		return cadena

	def getID(self):
		count=0
		return count

##**------------------------------------------------------
class Administracion(object):
	def __init__(self,objeto_name):
		self.objeto_name=objeto_name
		self.objeto=ObjetoAdministracion()
		if objeto_name == CONST_PRODUCTO:
			self.objeto.__class__=Producto
		elif objeto_name ==CONST_CLIENTE:
			self.objeto.__class__=Cliente
		elif objeto_name ==CONST_DISTRIBUIDOR:
			self.objeto.__class__=Distribuidor
		elif objeto_name ==CONST_USUARIO:
			self.objeto.__class__=Usuario
		self.file_name  =self.objeto.getFileName()

	def menu_administracion(self):
		while 1:
			print("\n####################################")
			print("Administracion de "+self.objeto_name+"s:::::\n\n")
			count = 1 
			try:
				if self.objeto_name ==CONST_USUARIO:
					with open(self.file_name,'r') as db:
						for linea in db:
							self.objeto.printLine(count,linea)
							count += 1
				else:
					with open(self.file_name,"r+b") as db:
						for linea in db:
							linea=descifraLinea(linea[0:len(linea)-1])
							self.objeto.printLine(count,linea)
							count += 1	
						db.close()	
			except IOError as e:
				print("ERROR: ARCHIVO DE "+self.objeto_name+"s vacio")
			opcion = input("Menu:\n\tSelecciona un "+self.objeto_name+" (#)\n\t'A' para agregar uno nuevo\n\t'X' para salir\n\t'L' Consulta\n\tSeleccion>")
			if opcion.lower() == "x":
				return 1
			elif opcion.lower() == "a":
				self.agrega()
			elif opcion.lower() == "l":
				self.consulta()
			elif len(opcion) == 1 and opcion.lower() not in letter_range('a','z') and int(opcion) < count and int(opcion) > 0:
				operacion = input("'B' para borrar, 'M' para modificar\n?>")
				if operacion.lower()=='b':
					self.borra(opcion)
				elif operacion.lower() =='m':
					self.modifica(opcion)
				else:
					print("Opcion invalida")
					self.menu_administracion()
			else:
				print("Opcion invalida")
				self.menu_administracion()

	def agrega(self):
		print("\n&&&&&&&&&&& AGREGA "+self.objeto_name+"&&&&&&&&&&&") 
		cadena    =""
		cadena    = self.objeto.setDatos()
		if self.objeto.existInFile() :
			return 0
		try:			
			if self.objeto_name ==CONST_USUARIO:
				with open(self.file_name,'a') as db:
					db.write(cadena)
			else:
				with open(self.file_name,'a+b') as db:
					cadena=cifrarLinea(cadena)
					db.write(cadena+"\n".encode('utf-8'))
					db.close()
			print("El "+self.objeto_name+"ha sido agregado exitosamente")	
		except IOError as e:
			return 0

	def consulta(self):	
		print ("\n&&&&&&&&&&& CONSULTA "+self.objeto_name+" &&&&&&&&&&&")
		count =0
		try:
			if self.objeto_name ==CONST_USUARIO:
				with  open(self.file_name,"r")  as db:
					for linea in db:
						count+=1
						self.objeto.printLine(count,linea)
			else:			
				with open(self.file_name,"r+b") as db:
						for linea in db:
							linea=descifraLinea(linea[0:len(linea)-1])
							count += 1
							self.objeto.printLine(count,linea)
								
						db.close()
			return count
		except IOError as e:
			print(" NO EXISTEN ENTRADAS")
		return count

	def borra(self,opcion):
		print("\nBORRADO "+self.objeto_name+" .... ") 
		count = 0
		final = b''
		try:
			with  open(self.file_name,"r+b")  as db:
				for linea in db:
					count += 1
					if count == int(opcion):
						continue 
					final += linea
			db.close()
			try:
				with open(self.file_name,'wb') as db:
					db.write(final)
			except IOError as e:
				print("ERROR:Error en escritura")
				pass
		except IOError as e:
			print("ERROR:error en lectura")
			pass
	
	def modifica(self,opcion):
		print ("\n&&&&&&&&&&& MODIFICA "+self.objeto_name+" &&&&&&&&&&&")
		count = 0
		try:	
			if self.objeto_name ==CONST_USUARIO:
				antes = ""
				modificacion = ""
				final = ""
				with open(self.file_name,"r") as db:
						for linea in db:
							count += 1
							if count == int(opcion):
								modificacion=self.objeto.modifyDatos(linea)
								antes=final
								final=""
								continue
							final += linea
						db.close()
						try:
							with open(self.file_name,'w') as db:
								db.write(antes)
								db.write(modificacion)
								db.write(final)
						except IOError as e:
							pass
			else:
				antes = b''
				modificacion = b''
				final = b''
				with open(self.file_name,"rb") as db:
					for linea in db:
						count += 1
						if count == int(opcion):
							linea=descifraLinea(linea[0:len(linea)-1])
							modificacion=self.objeto.modifyDatos(linea)
							antes=final
							final=b''
							continue
						final += linea
					db.close()
					try:
						with open(self.file_name,'wb') as db:
							db.write(antes)
							modificacion=cifrarLinea(modificacion)
							db.write(modificacion+"\n".encode('utf-8'))
							db.write(final)
							db.close()
					except IOError as e:
						pass
		except IOError as e:
			pass

##**------------------------------------------------------
class POS(object):

	def __init__(self):
		global adminProductos
		global adminClientes
		global adminDistribuidores
		global adminUsuarios
		print("Bienvenido al Sistema POS")

	def login(self):
		while 1:
			perm=self.checa_permisos()[:-1]
			if perm == "admin":
				self.presenta_interfaz_admin()
			elif perm == "mortal":
#				presenta_interfaz_mortal()
				pass

	def checa_permisos(self):
		print("\nSISTEMA DE ADMINISTRACION:::::")
		print("Por favor, introduce tus datos de ingreso a continuacion")
		usuario=input("Nombre de usuario: ")
		contrasena=getpass.getpass("Contrasena: ")
		elhash2=hashlib.sha1()
		elhash2.update(contrasena.encode('utf-8'))
		try:
			with open('contrasenas','r') as db:
				for linea in db:
					(usuario_file, contrasena_file, perm)=linea.split(" ")
					if usuario_file == usuario:
						if contrasena_file == elhash2.hexdigest():
							return perm
						else:
							break
					print("Datos incorrectos")
				return "error"
		except IOError as e:
			return "error"

	def presenta_interfaz_mortal(self):
	#		mientras no se vaya el vendedor:
	#			dame_cliente()
	#			mientras no me digan basta:
	#				agrega_producto()
	#			cobrar()
			pass

	def presenta_interfaz_admin(self):
		
		global adminProductos
		adminProductos= Administracion(CONST_PRODUCTO)
		global adminClientes
		adminClientes= Administracion(CONST_CLIENTE)
		global adminDistribuidores
		adminDistribuidores= Administracion(CONST_DISTRIBUIDOR)
		global adminUsuarios
		adminUsuarios= Administracion(CONST_USUARIO)
		while 1:
			print("\n::::::::::::::::::::::::")
			print("Bienvenido administrador")
			print("Por favor, selecciona una de las siguientes opciones:")
			print("    1) Productos")
			print("    2) Clientes")
			print("    3) Distribuidores")
			print("    4) Usuarios")
			print("    5) Salir")
			opcion=input('Que quieres administrar: ?>')
		
			if opcion=='1':
				adminProductos.menu_administracion()
			elif opcion=='2':
				adminClientes.menu_administracion()
			elif opcion=='3':
				adminDistribuidores.menu_administracion()
			elif opcion=='4':
				adminUsuarios.menu_administracion()
			elif opcion=='5':
				break
			else:
				print("Opcion invalida")

############## INICIAMOS PROGRAMA BASE ############################

tienda=POS()
tienda.login()



