/*
	Registrar 
	Loggear
	Ingresar DInero
	Retirarlo
	Agregar Contactos
	Ver Historial de transferencias
	Hacer Transferencias
*/
/*-------------------------------------------------------------------------------------------------------------------------*/
//LIBRERIAS
#include<stdio.h>
#include<locale.h>
#include<stdbool.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
/*-------------------------------------------------------------------------------------------------------------------------*/
//ESTRUCTURAS
typedef struct{
	int iNumCuenta;
}regContacto;

typedef struct{
	int iCuentaRemitente;
	int iCuentaDestino;
	float fImporte;
}regTransaccion;

typedef struct{
	int iNumCuenta;
	char *cNombre;
	char *cPassword;
	float fDinero;
	int iNumAmigos;
	int iNumTransferencias;
	regContacto *vtrAmigos;
	regTransaccion *vtrHistorial;
}regUsuario;

/*-------------------------------------------------------------------------------------------------------------------------*/
//DECLARACION DE LAS FUNCIONES Y VARIABLES GLOBALES
void vMenu();
void vRegistro();
void vRegistrar(char *, char *, int);
void vVerUsuarios();
void vMenuUsuario();

void vHacerTransferencia();
void vTransferir(int, int, float);
void vVerTransferencias();
void vIngresarDinero();
void vIngresar(float);
void vRetirarDinero();
void vRetirar(float);

char *cObtenerTexto(char *, int);
int iObtenerNumero(char *);
float fObtenerNumero(char *);
int iGenerarNumeroCuenta();
void vAgregarContacto();
bool bContactoExiste(int, regUsuario *);
void vVerContactos();

bool bLogin();

regUsuario *vtrBaseUsuarios;
int iNumeroUsuarios=0;
int iSessionNumUsuario; 
/*-------------------------------------------------------------------------------------------------------------------------*/
int main()
{
	setlocale(LC_ALL, "");
	
	vtrBaseUsuarios=(regUsuario *)malloc(sizeof(regUsuario));
	vRegistrar("Juan", "Contraseña", 1);
	
	vMenu();
	
	return 0;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vMenu()
{
	bool bSalir=false;
	char cOpcion[2];
	int iOpcion;
	do
	{
		system("CLS");
		printf("Elija una opción:\n");
		printf("1:Registrarse\n2:Iniciar Cesión\n3:Ver Usuarios\n4:Salir\n");
		//VALIDAR LA ENTRADA DE EL USUARIO
		scanf("%s", cOpcion);
		iOpcion=atoi(cOpcion);
		switch(iOpcion)
		{
			case 1:
					vRegistro();
				break;
			case 2:{
					if(bLogin()==true)
					vMenuUsuario();
				}
				break;
			case 3:
					vVerUsuarios();
				break;
			case 4:
					bSalir=true;
				break;
			default: printf("Parametro Invalido\n");
		}
	}while(bSalir==false);
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vMenuUsuario()
{
	bool bSalir=false;
	int iOpcion;
	char cOpcion[2];
	do
	{
		system("CLS");
		printf("Bienvenido %s, Dinero disponible: $%f\nElija una opción:\n", vtrBaseUsuarios[iSessionNumUsuario-1].cNombre, vtrBaseUsuarios[iSessionNumUsuario-1].fDinero);
		printf("1:Hacer Transferecia\n2:Ver Contactos\n3:Agregar Contacto\n4:Historial Transferencias\n5:Ingresar Dinero\n6:Retirar Dinero\n7:Cerrar Cesión\n");
		scanf("%s", cOpcion);
		iOpcion=atoi(cOpcion);
		
		switch(iOpcion)
		{
			case 1:
				vHacerTransferencia();
				break;
			case 2: 
				vVerContactos();
				break;
			case 3:
				vAgregarContacto();
				break;
			case 4:
				vVerTransferencias();
				break;
			case 5:
				vIngresarDinero();
				break;
			case 6:
				vRetirarDinero();
				break;
			case 7:
				bSalir=true;
				break;
			default: {
				printf("Parametros invalidos\n");
				system("PAUSE");
			}
		}
	}while(bSalir==false);

}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vAgregarContacto()
{
	int iNumCuenta=iObtenerNumero("Ingrese el número de cuenta");
	if(iNumCuenta<=iNumeroUsuarios && iNumCuenta!=iSessionNumUsuario)
	{
		regUsuario *vtrUsuario;
		vtrUsuario=&vtrBaseUsuarios[iSessionNumUsuario-1];
		if(vtrUsuario->iNumAmigos==0)
		{
			vtrUsuario->vtrAmigos=(regContacto *)malloc(sizeof(regContacto));
			vtrUsuario->vtrAmigos[0].iNumCuenta=iNumCuenta;
			vtrUsuario->iNumAmigos=1;
			printf("Se ha añadido a %s a tu lista de contactos\n", vtrBaseUsuarios[iNumCuenta-1].cNombre);
		}
		else if(bContactoExiste(iNumCuenta, vtrUsuario)==false)
		{
			int iUltimoIndice=vtrUsuario->iNumAmigos;
			vtrUsuario->vtrAmigos=(regContacto *)realloc(vtrUsuario, sizeof(regContacto)*(iUltimoIndice+1));
			vtrUsuario->vtrAmigos[iUltimoIndice].iNumCuenta=iNumCuenta;
			vtrUsuario->iNumAmigos++;
			printf("Se ha añadido a %s a tu lista de contactos\n", vtrBaseUsuarios[iNumCuenta-1].cNombre);
		}
		else
			printf("El usuario %s ya es tu contacto\n", vtrBaseUsuarios[iNumCuenta-1].cNombre);
	}
	else
		printf("Cuenta Invalida\n");
		
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vVerContactos()
{
	int i;
	int iNumCuenta;
	regUsuario *vtrUsuario;
	vtrUsuario=&vtrBaseUsuarios[iSessionNumUsuario-1];
	if(vtrUsuario->iNumAmigos>0)
	{
		for(i=0; i<vtrUsuario->iNumAmigos; i++)
		{
			iNumCuenta=vtrUsuario->vtrAmigos[i].iNumCuenta;
			printf("Nombre: %s \t Número de cuenta: %d\n", vtrBaseUsuarios[iNumCuenta-1].cNombre, iNumCuenta);
		}
	}
	else
		printf("Aun no tienes ningun contacto :(\n");
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
bool bContactoExiste(int iNumCuenta, regUsuario *vtrUsuario)
{
	int i;
	for(i=0; i<vtrUsuario->iNumAmigos; i++)
	{
		if(vtrUsuario->vtrAmigos[i].iNumCuenta==iNumCuenta)
		{
			return true;
		}
	}
	return false;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vIngresarDinero()
{
	float fCantidad=fObtenerNumero("Ingrese la cantidad que desea añadir a su cuenta");
	vIngresar(fCantidad);
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vIngresar(float fCantidad)
{
	vTransferir(iSessionNumUsuario, iSessionNumUsuario, fCantidad);
	vtrBaseUsuarios[iSessionNumUsuario-1].fDinero+=fCantidad;
	printf("Se ha ingresado $%f a su cuenta\n", fCantidad);
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vRetirarDinero()
{
	float fCantidad=fObtenerNumero("Ingrese la cantidad que desea retirar de su cuenta");
	if(fCantidad<=vtrBaseUsuarios[iSessionNumUsuario-1].fDinero)
	{
		vRetirar(fCantidad);
	}
	else
		printf("No cuenta con los fondos para hacer ese retiro\n");
		
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vRetirar(float fCantidad)
{
	vTransferir(iSessionNumUsuario, iSessionNumUsuario, fCantidad*-1);
	vtrBaseUsuarios[iSessionNumUsuario-1].fDinero-=fCantidad;
	printf("Se ha retiraddo $%f de su cuenta\n", fCantidad);
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vHacerTransferencia()
{
	regTransaccion vtrNuevaTransaccion;
	int iCuentaDestino=iObtenerNumero("Ingrese el número de cuenta de destino");
	int iCuentaRemitente=iSessionNumUsuario;
	float fImporte=fObtenerNumero("Ingrese la cantidad a transferir");
	if(iCuentaDestino<=iNumeroUsuarios)
	{
		if(fImporte<=vtrBaseUsuarios[iSessionNumUsuario-1].fDinero)
		{
			vTransferir(iCuentaDestino, iCuentaRemitente, fImporte);
		}
		else
			printf("No cuenta con los fondos suficientes para realizar esa transacción\n");
	}
	else
		printf("Cuenta invalida\n");
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vTransferir(int iDestino, int iRemitente, float fCantidad)
{	
	bool bDest;
	regUsuario *vtrUsuario, *vtrDestino;
	vtrUsuario=&vtrBaseUsuarios[iSessionNumUsuario-1];
	bDest=(iDestino<=iNumeroUsuarios && iDestino!=iRemitente)?true:false;
	if(bDest)
	{
		regTransaccion vtrTransaccion;
		vtrTransaccion.iCuentaDestino=iDestino;
		vtrTransaccion.iCuentaRemitente=iRemitente;
		vtrTransaccion.fImporte=fCantidad;
		
		vtrDestino=&vtrBaseUsuarios[iDestino-1];
		
		vtrUsuario->fDinero-=fCantidad;
		vtrDestino->fDinero+=fCantidad;
		
		vtrUsuario->vtrHistorial=(vtrUsuario->iNumTransferencias==0)?(regTransaccion *)malloc(sizeof(regTransaccion)):vtrUsuario->vtrHistorial;
		vtrDestino->vtrHistorial=(vtrDestino->iNumTransferencias==0)?(regTransaccion *)malloc(sizeof(regTransaccion)):vtrDestino->vtrHistorial;
		
		vtrUsuario->iNumTransferencias++;
		vtrDestino->iNumTransferencias++;
		
		vtrUsuario->vtrHistorial=(regTransaccion *)realloc(vtrUsuario->vtrHistorial, sizeof(regTransaccion)*(vtrUsuario->iNumTransferencias));
		vtrUsuario->vtrHistorial[vtrUsuario->iNumTransferencias-1]=vtrTransaccion;
		
		vtrDestino->vtrHistorial=(regTransaccion *)realloc(vtrDestino->vtrHistorial, sizeof(regTransaccion)*(vtrDestino->iNumTransferencias));
		vtrDestino->vtrHistorial[vtrUsuario->iNumTransferencias-1]=vtrTransaccion;
		
	}
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vVerTransferencias()
{
	int i, iIndiceDestino, iIndiceRemitente;
	regUsuario *vtrUsuario;
	vtrUsuario=&vtrBaseUsuarios[iSessionNumUsuario-1];
	if(vtrUsuario->iNumTransferencias>0)
	{
		float fCantidad;
		for(i=0; i<vtrUsuario->iNumTransferencias; i++)
		{
			iIndiceDestino=vtrUsuario->vtrHistorial[i].iCuentaDestino-1;
			iIndiceRemitente=vtrUsuario->vtrHistorial[i].iCuentaRemitente-1;
			fCantidad=vtrUsuario->vtrHistorial[i].fImporte;
			printf("De %s para %s: $%f\n", vtrBaseUsuarios[iIndiceRemitente].cNombre, vtrBaseUsuarios[iIndiceDestino].cNombre, fCantidad);
		}
	}
	else
		printf("Aun no haces ninguna transferencia\n");
		
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
bool bLogin()
{
	bool logged=false;
	int iNumCuenta=iObtenerNumero("Ingrese su número de cuenta");
	if(iNumCuenta<=iNumeroUsuarios)
	{
		char *cPassword=cObtenerTexto("Ingrese su contraseña", 0);
		logged=(strcmp(cPassword, vtrBaseUsuarios[iNumCuenta-1].cPassword)==0)?true:false;
		if(logged==true)
		{
			printf("Las contraseñas coinciden\n");
			iSessionNumUsuario=iNumCuenta;
		}
		else
			printf("Las contraseñas no coinciden\n");
	}
	else
		printf("Numero de cuenta inexistente\n");
		
	system("PAUSE");
	return logged;
	
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vRegistro(){
		char *cNombre=cObtenerTexto("Ingrese su nombre", 0);
		char *cPassword=cObtenerTexto("Ingrese su contraseña (Minimo 8 Caracteres)", 8);
		int iNumeroCuenta=iGenerarNumeroCuenta();
		
		vRegistrar(cNombre, cPassword, iNumeroCuenta);
		
		printf("Su numero de cuenta es: %d\n", iNumeroCuenta);
		system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vRegistrar(char *cNombre, char *cPassword, int iNumCuenta)
{
	
	vtrBaseUsuarios[iNumCuenta-1].cNombre=(char *)malloc(sizeof(char));
	vtrBaseUsuarios[iNumCuenta-1].cPassword=(char *)malloc(sizeof(char));
	
	regUsuario *vtrUsuario;
	vtrUsuario=&vtrBaseUsuarios[iNumCuenta-1];
	
	strcpy(vtrUsuario->cNombre, cNombre);
	strcpy(vtrUsuario->cPassword, cPassword);
	
	vtrUsuario->iNumCuenta=iNumCuenta;
	vtrUsuario->fDinero=0;
	vtrUsuario->iNumAmigos=0;
	vtrUsuario->iNumTransferencias=0;
	iNumeroUsuarios++;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vVerUsuarios()
{
	int i;
	
	if(iNumeroUsuarios>0)
	{
		for(i=0; i<iNumeroUsuarios; i++)
		{
			printf("Nombre: %s \t Número de Cuenta: %d\n", vtrBaseUsuarios[i].cNombre, vtrBaseUsuarios[i].iNumCuenta);
		}	
	}
	else
		printf("Aún no hay usuarios registrados\n");
		
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
char *cObtenerTexto(char *cPeticion, int iLongitud)
{
	char *cTexto;
	bool bValido=false;
	cTexto=(char *)malloc(sizeof(char));
	do
	{
		fflush(stdin);
		system("CLS");
		printf("%s: ", cPeticion);
		scanf("%[^\n]", cTexto);
		
		if(strcmp(cTexto, ""))
		{
			bValido=(iLongitud<=0)?true:false;
			if(bValido==false)
			{
				bValido=(strlen(cTexto)>=iLongitud)?true:false;
			}
		}
	}while(!bValido);
	return cTexto;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
int iObtenerNumero(char *peticion)
{
	char *cNumero;
	int iNumero;
	cNumero=(char *)malloc(sizeof(char));
	do
	{
		fflush(stdin);
		printf("%s: ", peticion);
		scanf("%s", cNumero);
		iNumero=atoi(cNumero);
	}while(iNumero<=0);
	return iNumero;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
float fObtenerNumero(char *peticion)
{
	char *cNumero;
	float fNumero;
	cNumero=(char *)malloc(sizeof(char));	
	do
	{
		fflush(stdin);
		printf("%s: ", peticion);	
		scanf("%s", cNumero);
		fNumero=atof(cNumero);
	}while(fNumero<=0);
	return fNumero;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
int iGenerarNumeroCuenta()
{
	if(iNumeroUsuarios==0)
	{
		vtrBaseUsuarios=(regUsuario *)malloc(sizeof(regUsuario));
		return iNumeroUsuarios+1;
	}
	else{
		vtrBaseUsuarios=(regUsuario *)realloc(vtrBaseUsuarios, sizeof(regUsuario)*(iNumeroUsuarios+1));
		return iNumeroUsuarios+1;
	}
}



