#SOCKET CLIENTE##########################################
#Hecho por Carlos Ruiz Aguilar                          #
#                                                       #
#Fuentes de ayuda:                                      #
# stack overflow                                        #
# Python Non-blocking peer to                           #
# peer chat socket.error: [Errno 32] Broken pipe        #
########################################################

import socket
import os
import select
import sys
#coding: utf-8

def prompt():
   sys.stdout.write('Cliente$')
   sys.stdout.flush()

HOST='127.0.0.1'                        # Direccion IP default 127.0.0.1
PORT=50007                              #puerto
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)# socket tipo TCP 
s.connect((HOST,PORT))                  # Asociacion y asignacion a puerto y equipo
print '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'
print '           Easy CHAT             '
print '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'
print '            Cliente              '
print 'HOST: ',HOST
print 'PORT: ',PORT
# LECTURA LECTURA LECTURA LECTURA LECTURA
prompt()
while 1:
    socket_list = [sys.stdin,s]
    read_sockets, write_sockets, error_sockets = select.select(socket_list, [], [])
    for sock in read_sockets:
        if sock == s:
            data = sock.recv(4096)
            if not data:
                print '\nSe ha desconectado el Servidor'
                sys.exit()
            else:
		sys.stdout.write("\n")
                sys.stdout.write(data)
                prompt()
        else:
            msg = sys.stdin.readline()
            s.send( msg)
            prompt()
