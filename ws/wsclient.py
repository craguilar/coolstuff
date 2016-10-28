import urllib.request
import xml.etree.ElementTree
import sys

if len(sys.argv)==2:
	cp=sys.argv[1]
else:
	print("Adios.")
	sys.exit()

peticion=urllib.request.urlopen("http://api.geonames.org/postalCodeSearch?postalcode="+cp+"&maxRows=10&username=craguilar")

eltree=xml.etree.ElementTree.fromstring(peticion.read().decode('utf-8'))

#for rama in eltree:
#	for ramita in rama:
#		print(ramita.tag,ramita.text)

#if eltree.findall("name") != []:
#	for name in eltree.findall("name"):
#		print("hola")
#else:
#	for rama in eltree:
#		if rama.findall("name") != []:
#			print("ya encontre name en: ",rama.tag)

if eltree.findall("code") != []:
	for count,bloqueCode in enumerate(eltree.findall("code")):
			for hijo in bloqueCode:
				if hijo.tag == "countryCode" and hijo.text == "MX":
					#print(eltree[count+1][1].text,eltree[count+1][0].text,eltree[count+1][8].text,eltree[count+1][6].text,eltree[count+1][2].text)
					print("https://maps.google.com/maps?q="+eltree[count+1][3].text+","+eltree[count+1][4].text)
else:
	print("no lo encontré")
