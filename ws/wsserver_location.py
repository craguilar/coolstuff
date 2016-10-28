import urllib.request
import xml.etree.ElementTree
import sys
from http.server import HTTPServer,SimpleHTTPRequestHandler
import urllib.parse
import re

class httpHandler(SimpleHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        qs = {}
        path = self.path
        if '?' in path:
            path, tmp = path.split('?', 1)
            qs = urllib.parse.parse_qs(tmp)
            for key,val in qs.items():
                if key=="cp":
                        self.wfile.write( ("<h1>Location<h1><div><iframe  src='"+regresa_mapa(re.sub(r'[^0-9]',"",val[0]))+"&output=embed'></iframe></div>").encode('utf-8'))
        else:
                self.wfile.write("<img src='http://2.bp.blogspot.com/-puJ3SY-jyQc/T5pxO3IRqLI/AAAAAAAAAXM/0TQYyfvSdBg/s1600/ornitorrinco.jpg' />".encode('utf-8'))
        return

def run(server_class=HTTPServer, handler_class=httpHandler):
    server_address = ('0.0.0.0', 8000)
    httpd = server_class(server_address, handler_class)
    httpd.serve_forever()

def regresa_mapa(cp):
                peticion=urllib.request.urlopen("http://api.geonames.org/postalCodeSearch?postalcode="+cp+"&maxRows=100&username=craguilar")

                eltree=xml.etree.ElementTree.fromstring(peticion.read().decode('utf-8'))

                #for rama in eltree:
                #   for ramita in rama:
                #       print(ramita.tag,ramita.text)

                #if eltree.findall("name") != []:
                #   for name in eltree.findall("name"):
                #       print("hola")
                #else:
                #   for rama in eltree:
                #       if rama.findall("name") != []:
                #           print("ya encontre name en: ",rama.tag)

                if eltree.findall("code") != []:
                    for count,bloqueCode in enumerate(eltree.findall("code")):
                            for hijo in bloqueCode:
                                if hijo.tag == "countryCode" and hijo.text == "MX":
                                    #print(eltree[count+1][1].text,eltree[count+1][0].text,eltree[count+1][8].text,eltree[count+1][6].text,eltree[count+1][2].text)
                                    return "https://maps.google.com/maps?q="+eltree[count+1][3].text+","+eltree[count+1][4].text
                else:
                    pass
                return "nel"

run()
