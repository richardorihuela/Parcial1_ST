





def evaluarminusculas(letra):
    print ("minuscula");
    if letra < 123:
        return letra;
    else:
        evaluarminusculas(letra - 26);

def evaluarMayusculas(letra):
    print ("MAYUSCULA");
    if letra < 91:
        return letra;
    else:
        evaluarMayusculas(letra - 26);
    
#Función para encriptar Cifrado Cesar en Python
def encriptar(frase, numero):
    
    #MAYUSCULAS: 65 al 90
    #minusculas: 97 al 122
    cifrado = "";
    for letra in frase:
        valor = ord(letra);
        if letra!=' ':
            if valor>96:
                valor = valor + int(numero);
                nuevoValor = evaluarminusculas(valor);
            else:
                valor = valor + int(numero);
                nuevoValor = evaluarMayusculas(valor);
            cifrado = cifrado + chr(nuevoValor);
        else:
            cifrado = cifrado + " ";
    print("La frase encriptado es:", cifrado);

#Procedimiento principal.
print("        CIFRADO: METODO CESAR\n");
print("-----------------------------\n");

cadena = input("Introduce tu nombre: ");

x = input("Introduce un numero: ");
#Llamamos a la función encriptar
encriptar(cadena, x);
