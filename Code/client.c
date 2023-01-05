#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

int clipv6() {
	FILE *fisier;
	fisier=fopen("index.html","w");//deschidere fisier in modul de scriere
	int informatieAdresa, sock;
	struct addrinfo hints, *res, *p;
        memset(&hints,0,sizeof(hints));//ne asiguram ca memoria este goala, memset umple blocul de memorie cu valori de 0 incepand de la adresa de inceput specificata de  hints pe lungim ea de bytes precizata

	hints.ai_family = AF_INET6;//AF_INET PERMITE FOLOSIREA IPV6
	hints.ai_socktype = SOCK_STREAM;//SOCK_STREAM se foloseste pt TCP
	//Socket client
	sock = socket(AF_INET6,SOCK_STREAM,0);

	if(sock ==- 1)
	{
		printf("Creare socket esuata!!\n");
		return 0;
	}
     //	else printf("Creare socket reusita!!\n");

	//Socket pt conectare la server
	informatieAdresa = getaddrinfo("www.traceroute6.net", "80", &hints, &res);// extrage adresa de internet specificata, 80 reprezinta portul http ,in res este un poiner spre inceputullistei de adrese ,hints pointer la o structura addrinfo
	if(informatieAdresa !=0 )
	{
		printf("Eroare la realizarea legaturii!!\n");
		return 0;
	}
	
	if(connect(sock,res->ai_addr,res->ai_addrlen)==-1)//testam conectivitatea socketului ipv6 la site
	{
		printf("Eroare la realizarea conexiunii!!\n");
		return 0;
	}
     	else { printf("Conexiune realizata cu success!!\n"); }
	
	//Trimitere date
	
	char *comanda = "GET / HTTP/1.0\r\n\r\n"; 

	//Send returneaza Zero sau >zero, fiind nr de bytes trimisi
	if(send(sock, comanda, strlen(comanda),0) == -1)//se trimite o cerere catre server 
	{	
		printf("Data Netrmise!!\n");
		return 0;
	}
//	else printf("Date Trimise!!\n");


	//Primire raspuns	

	char raspuns[2000];
	int verificaRaspuns,i;
	do{
		 verificaRaspuns = recv(sock, raspuns, 2000-1, 0);//0 se pune ddeoarece acceptam orice tip de mesaj

		//in caz ca verificaRaspuns ia valoarea minus unu, inseamna ca nu se poate primi un raspuns
		//in caz ca verificaRaspuns este zero, s-a incheiat conexiunea
		//in cazul in care verificaPrimit nu este nici una din aceste valori,
		// el va fi = cu nr de bytes primit
		if(verificaRaspuns>0)
		{
			for( i=0;i<verificaRaspuns;i++)
 	                         fprintf(fisier,"%c",raspuns[i]);
		} 
		else if(verificaRaspuns == 0) 
			printf("Conexiunea s-a incheiat!!\n");
		else	
			 printf("Eroare!!\n");
		raspuns[verificaRaspuns] = '\0';//deoarece sirul raspuns nu are predefint la finai caracaterul /0 trebuie pus de noi si il punem pe pozitia verifRasp pt ca 
	        // verificare raspuns reprezinta nr de biti al site-u
	}while(verificaRaspuns>0);
	fclose(fisier);//inchidere fisier
}
