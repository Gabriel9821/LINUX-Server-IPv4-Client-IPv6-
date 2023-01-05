#include <netinet/in.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include"client.c"
int main()
{
         clipv6();//Apelam clientul
	 FILE *fisier;// declaram un pointer spre fisier, aceasta declarare este necesara pentru comunicarea intre fisier si program
	 fisier=fopen("index.html","r");//deschidere fisiser in modul citire
         int rc,send_data,recv_data=0, serverPort=22222;  // stabilim portul prin variabila serverPort

         struct sockaddr_in server_address, client_address; //descriptor adresa socket

         socklen_t dimensiune_adresa_client; // socklen_t returneaza marimea spatiului de stocare al genericului sockaddr_storage

  	 char *msg="Comanda neimplementata!\n"; //mesajul returnat in cazul trimiterii unei comenzi

         int length=strlen(msg); //lungimea mesajului pt comanda neimplementata
 
         char mess[2000]; //un sir de caractere de 2000
 
         int sock = socket(AF_INET, SOCK_STREAM, 0); //creaza un punct terminal de comunicare, pe ipv4

         if(sock < 0)//se verifica posibilitatea de a deschide socketul. in cazul in care nu este posibil se iese din program
         {
                 printf(" Nu se poate deschide socketul!\n");
		 return 0;
         }
         server_address.sin_family=AF_INET;  //ipv4
         server_address.sin_addr.s_addr= htonl(INADDR_ANY); //inaddr_any lasa programul sa mearga fara o adresa ip specificata
         server_address.sin_port=htons(serverPort); //port server

	 if(bind(sock,(struct sockaddr *)&server_address, sizeof(server_address)) <0)// leaga socket-ul de port; asigneaza o adresa de protocol l    ocal a unui socket
         {
                 printf(" Eroare in functia bind\n");
                 return 0;
         }

         int clienti_coada=9;
         if(listen(sock, clienti_coada)!=0) //listen anunta disponibilitatea serverului de a accepta conectari,fixeaza  dimensiunea cozii de aste    ptare (9)
         {
                 printf("Apelare afunctiei listen nu a reusit\n");
                 return 0;
         }
         while(1)
         {
                 dimensiune_adresa_client= sizeof(client_address);//preia dimensiunea structurii

                 int acc = accept(sock, (struct sockaddr *)&client_address, &dimensiune_adresa_client);
                 // accept stabileste statusul de acceptare a conexiunii
                 // extrage prima conexiune din coada de asteptare, creaza un nou socket cu aceleasi date ca socketul specificat
                 // si aloca un nou descriptor pentru acel socket
                 // daca reuseste returneaza o val >0
		 //
                 if(acc < 0)
                 {
			 printf("Eroare la acceptare\n");
                         return 0;
                 }
                 else
                 {
                         printf("---Conexiunea a fost acceptata!---\n");
                         do
                         {
                                 //recv primeste date pe socket
                                 recv_data = recv(acc, mess, sizeof(mess), 0);
                                 //in caz de succes, recv returneaza lungimea mesajului in bytes
                                 //valoarea 0 indica o conexiune incheiata
                                 //valoarea -1 indica o conexiune esuata

                                 if(recv_data > 0) {
                                         if(strncmp(mess,"06#",3)!=0)//verifica daca comanda trimisa nu este cea buna
                                                 send_data = send(acc, msg, length, 0); //in cazul in care comanda nu este cea asteptata programu    l returneaza mesajul de eroare stocat in msg
                                         else {
                                                 fisier=fopen("index.html","r");
                                                 char a[255];
                                                 while(!feof(fisier))//parcurge fisierul cat timp nu a ajuns la capatul lui
                                                 {
                                                         fgets(a,255,fisier);//255 dim maxima de caractere cu a se preia fiecare linie
                                                         send(acc,a,strlen(a),0);//trimitem clientului datele receptionate din fisier
                                                 }
                                         fclose(fisier);// inchide fisierul dupa terminarea parcurgerii
					 }
                                 }
                                 if(recv_data == 0){
					 printf("---Conexiune terminata!---\n");
                                          close(acc);
                                          break;
                                 }
 
                         } while(recv_data > 0); //cat timp exista un mesaj, trimite date
		 }
		 //daca nu exista un mesaj, sau nu s-a primit unul, se inchide conexiunea
         }
          return 0;
  }
