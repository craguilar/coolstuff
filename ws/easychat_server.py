#SOCKET SERVER##########################################
#Hecho por Carlos Ruiz Aguilar	 			#
#				 			#
#Fuentes de ayuda:					#
# stack overflow 					#
# Python Non-blocking peer to 				#
# peer chat socket.error: [Errno 32] Broken pipe	#
########################################################

import socket
import select
import sys
#coding : utf-8
permanecer =True
CONNECTION_LIST = []			# Lista de conexiones
HOST='127.0.0.1'                 	# Direccion IP default 127.0.0.1
PORT=50007              		# Puerto de conexion
srvr=socket.socket(socket.AF_INET,socket.SOCK_STREAM)# socket tipo TCP 
srvr.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
srvr.bind((HOST,PORT))     		# Asociacion y asignacion a puerto y equipo
srvr.listen(5)             		# Escuchar por conexiones entrantes
CONNECTION_LIST.append(srvr)		# Agregamos a la lista de conexiones

CONNECTION_LIST.append(sys.stdin)	# File objects corresponding to the interpreters
					# standard input, output and error streams. 
					# stdin is used for all interpreter input except 
					# for scripts but including calls to input() and raw_input()
print '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'
print '           Easy CHAT             '
print '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'
print '            Server 		'
print 'HOST: ',HOST
print 'PORT: ',PORT
print 'Esperando conexiones....         ' 
# Funcion que envia mensaje de Broadcast a todos los clientes conectados
def broadcast_data(sock, message):
	for socket in CONNECTION_LIST:
		if socket != srvr and socket != sock and socket != sys.stdin:
            		try:
                		socket.send(message)
            		except:
                		socket.close()
                		CONNECTION_LIST.remove(socket)
def prompt() :
    	sys.stdout.write('Server$')
	sys.stdout.flush()
#Llamamos a la funcion promt
prompt()
while permanecer:
    # Sockets no blqueantes 
    #This is a straightforward interface to the Unix select() system call. 
    #The first three arguments are sequences of waitable objects: either 
    #integers representing file descriptors or objects with a parameterless
    # method named fileno() returning such an integer:
    
    #Otro sentido:The select() C API returns a list of all sockets that are 
    #currently readable. Assuming python's wrapper does the same,sread will 
    #contain multiple elements if multiple sockets are readable.
    #A blocking socket will only block if there is no data present, so after
    # a select() call indicated there is data, you can assume that one call
    # to recv will not bloc
    read_sockets, write_sockets, error_sockets = select.select(CONNECTION_LIST, [], []) # SOCKETS NO BLOQUENTES
    #Para cada socke listo para ser leido
    for sock in read_sockets:
	# Si se trata de un nueva conexion
        if sock == srvr: 
            con, addr = srvr.accept()#Agregamos una nueva conexion
            CONNECTION_LIST.append(con)
            print '\n Server$ Cliente [%s, %s] se ha conectado!! ' % addr
	    print ''
            broadcast_data(con, "\n Server$ El cliente [%s:%s] ha entrado a la sala! \n" % addr)
	    prompt()
        #Si lo que esta pendiente de lectura proviene de la entrada estandar
	elif sock == sys.stdin:
            msg = sys.stdin.readline()
            broadcast_data(sock, 'Server $ ' + msg)
	    if "exit" in msg:
		permancer=False
	    	sys.exit(0)
		break
            prompt()
        else:
	#Si se trata de un socket
            try:
                data = sock.recv(4096)#Leemos
                if data:
                    print "\rCliente " + str(sock.getpeername()) + 'dice :' + data
                    broadcast_data(sock,"\r Cliente" + str(sock.getpeername()) + ' dice :' + data)
		    prompt()
            except:
                broadcast_data(sock, "El Cliente [%s, %s] se ha desconectado :(" % addr)
                print "El cliente [%s, %s] se ha desconectado" % addr
                sock.close()
                CONNECTION_LIST.remove(sock)
		continue

