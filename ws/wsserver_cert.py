import urllib.request
import xml.etree.ElementTree
import sys
from http.server import HTTPServer,SimpleHTTPRequestHandler
import urllib.parse
import re
from weasyprint import HTML
##########################################################
#Carlos Ruiz Aguilar					 #
# Sobre wasyprint: http://weasyprint.org/                #
#		   http://weasyprint.org/docs/install/   #
#							 #
# Archivo usuarios(debe estar en la misma ruta)		 #
# id:tkn:NombreCompleto                                  #
###########################################################
class Token(object):
	def __init__(self,tkn):
		self.name=tkn
		self.count=0
	def getCount(self):
		return self.conunt
	
class ConstanciaHandler(SimpleHTTPRequestHandler):
	
	usuarios ={}

	def do_GET(self):
		self.send_response(200)
		
		qs_temp = {}
		qs      = {}
		path    = self.path
		id_u	= ""
		tkn     = ""
		if '?' in path:
			path, tmp = path.split('?', 1)
			param     = tmp.split('?')
			if len(param)==2 :
				for value in param:
					qs_temp = urllib.parse.parse_qs(value)
					qs.update(qs_temp)

				for key,val in qs.items():
					if key=="id":
						id_u=val[0]
					elif key=="tkn":
						tkn =val[0]
				if id_u=="" and tkn =="":
					self.send_header("Content-type", "text/html")
					self.end_headers()
					self.wfile.write("Error en los parámetros".encode('utf-8'))
					return
				#Ahora sabemos que nuestra url es correcta
				self.generaCertificado(id_u,tkn)
				return 
			else:
				self.send_header("Content-type", "text/html")
				self.end_headers()
				self.wfile.write("Error en los parámetros".encode('utf-8'))
				return 
		else:
			self.send_header("Content-type", "text/html")
			self.end_headers()
			self.wfile.write("<img src='http://s1.eulisesavila.com/wp-content/uploads/2012/10/pagina-web-error.jpg' />".encode('utf-8'))
		return
	#### Generamos el certificado ################################################
	def generaCertificado(self,id_u,tkn):
		#Validacion sobre el token 
		if id_u in ConstanciaHandler.usuarios and tkn==ConstanciaHandler.usuarios[id_u].name:
			token=ConstanciaHandler.usuarios[id_u]
			token.count+=1
			if token.count>2:
				self.send_header("Content-type", "text/html")
				self.end_headers()
				self.wfile.write("Ha excedido el limite de descargas... :(".encode('utf-8'))
				return 		
		else:
			if self.autenticacion(id_u,tkn):
				token=Token(tkn)
				ConstanciaHandler.usuarios[id_u]=token
				
			else:
				self.send_header("Content-type", "text/html")
				self.end_headers()
				self.wfile.write("Error de autenticacion".encode('utf-8'))
				return 
		#########################
		# Creacion del PDF
		self.send_header("Content-type", "application/pdf")
		self.end_headers()		
		#self.wfile.write("Parametros correctos... Enviando su certificado".encode('utf-8'))
		HTML(string='<h2>CERTIFICADO EN SEGURIDAD DE LA INFORMACION</h2>'+
			    '<BR>USUARIO :'+self.getUser(id_u)+ 
			    '<BR>ID      :'+id_u).write_pdf('./CERT.pdf')
		f = open("./CERT.pdf","rb")	
		self.wfile.write(f.read())
		f.close()		
		return 

	def getUser(self,id_u):
		try:
			with open('usuarios','r') as db:
				for linea in db:
					(id_user, token, user_name)=linea.split(":")
					if id_user ==id_u:
						return user_name
		except IOError as e:
			print(" ERROR: NO encuentro el archivo usuarios")
		return "NULL"	

	def autenticacion(self,id_u,tkn):
		try:
			with open('usuarios','r') as db:
				for linea in db:
					(id_user, token, user_name)=linea.split(":")
					if id_user ==id_u and tkn == token:		
						return 1
		except IOError as e:
			print(" ERROR: NO encuentro el archivo usuarios")
		return 0

###################### DONDE CORREMOS A NUESTRO SERVIDOR #############################
def run(server_class=HTTPServer, handler_class=ConstanciaHandler):
    server_address = ('0.0.0.0', 8000)
    httpd = server_class(server_address, handler_class)
    httpd.serve_forever()

####################  CORREMOS NUESTRO WEB SERVICE ###################################
run()
