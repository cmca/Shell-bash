
//Listing 9
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

//#include "slinkedlist.h"
static char *my_argv[100], *my_envp[100];

//________________Funcoes da lista de Jobs_______________________________
typedef struct node {
	int PID;// fork retorna
	int JID;// eu crio
	int status;
	char cmd[30];
	struct node * prev;
	struct node * next;
} ListNode;

/*
	Cria uma lista vazia, contendo apenas o sentinela, e o retorna.
*/
ListNode * listCreate (void){
    ListNode *a;
    a = (ListNode*) malloc(sizeof(ListNode));
    /*Faz com que o sentinela aponte como proximo elemento e como elemento anterior ele mesmo.*/
    a->prev = a;
    a->next = a;
    a->PID = 0;
    a->JID = 0;
    a->status = 0;
    return(a);
}
/*
	Aloca um nó na memória e o retorna.
	JID e PID sao passados por parâmetro.
	prev e next = NULL.
*/
ListNode * listNodeAlloc (int pid,int jid, char* cmd, int status){
    ListNode *a;
    a = (ListNode*) malloc(sizeof(ListNode));
    /*Apenas aloca o nó com uma key, logo esse nó não aponta para nada*/
    a->prev = NULL;
    a->next = NULL;
    strcpy(a->cmd, cmd);
    a->PID = pid;
    a->JID = jid;
    a->status = status;
    return(a);
}
/*
	Insere o nó x, passado por referência, antes do sentinela. Ou seja no fim da lista
*/
void listInsertTail (ListNode * sentinel, ListNode * x){
    /*Coloca o novo nó como antecessor do sentinela , o sentinela como seu sucessor,o antigo antecessor
    do sentinela como antecessor do novo nó e faz o novo nó ser sucessor do antigo antecessor do sentinela.*/
    x->prev=sentinel->prev;
    x->next = sentinel;
    x->prev->next = x;
    sentinel->prev = x;
}

void find_int(char* v, int k){
	int n = 0;
	int tam =0 ;
	while(k>0){
		tam++;
		k = (int)(k/10);
	}
}
/*
	Remove todos os nós da lista, menos o sentinela.
*/
void listEmpty (ListNode * sentinel){
    ListNode *a;
    a = sentinel->next;
    
    //char cBuffer[10];

    /*Enquanto 'a' nao for sentinela ele sera removido da lista.*/
    while(a!=sentinel)
    {
    	
        /*Faz com que o sucessor de 'a' se torne sucessor de sentinel*/
        sentinel->next = a->next;
        /*Faz com que o antecessor do sucessor de 'a' seja o sentinel.*/
        a->next->prev = sentinel;
        free(a);
        a = sentinel->next;
    }
}
/*
	Remove todos os nós da lista, inclusive o sentinela.
*/
void listTerminate (ListNode * sentinel){
	//char matar[30] = "/bin/kill -9 -1";
	char matar[30] = "/bin/kill -9 -1";
	if(fork() == 0) {
    		execve(matar, my_argv, my_envp);
			//execve("/usr/bin/kill", argv, my_envp);
			exit(1);
		} else {
			wait(NULL);
		}
    listEmpty(sentinel);/*Primeiro remove todos os elementos da lista.*/
    free(sentinel);/*Depois desaloca o sentinela da memoria*/
}
/*
	Retorna o número de elementos da lista.
*/
int listCount (ListNode * sentinel){
    ListNode *a;
    a = (ListNode*) malloc(sizeof(ListNode));
    /*De inicio a recebe a posiçao do primeiro elemento apos o sentinela.*/
    a= sentinel->next;
    int cont=0;
    /*Enquanto 'a' nao for o sentinel o contador sera aumentado.*/
    while(a!=sentinel)
    {
        cont++;
        a=a->next;
    }
    return (cont);
}
/*
	Imprime os elementos da lista.
*/
void listPrint (ListNode * sentinel){
    ListNode *a;
    a = (ListNode*) malloc(sizeof(ListNode));

    int n = listCount (sentinel);//recebe o numero de elementos na lista

    /*'a' recebe o endereço do primeiro nó apos o sentinel.*/
    a= sentinel->next;
    
    char sinal;//char para o sinal '+' ou '-', para os processos mais recentes

    char estados[3][15];// possiveis estados para o programa
    strcpy(estados[0],"Done");
    strcpy(estados[1],"Running");
    strcpy(estados[2],"Terminated");
    
    a= sentinel->next;
    while(a!=sentinel)
    {
    	sinal = ' ';
    	if(n==2){
    		sinal = '-';
    	}
    	if(n==1){
    		sinal = '+';
    	}
    	n--;
    	//print foi adequado para imitar o padrao do print do ubuntu
        printf("[%d]%c\t%d\t%s\t%s\n",a->JID,sinal, a->PID, estados[a->status], a->cmd);
        /*Faz com que 'a' seja movido para o nó seguinte*/
        a=a->next;
    }
}
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

//Variavel global para evitar de alterar muito o codigo base
// criacao da lista de processos para o comando jobs

ListNode * JOBS;
int FLAG = 0;// flag para determinar se o programa deve ser executado em background ou nao


extern int errno;

char *get_place();

typedef void (*sighandler_t)(int);

static char *search_path[10];

/*Ao receber o sinal de interrupção ou o sinal de fim de execução de comando
essa função printa "[ G5 ]" para marcar o reinicio do ciclo, e depois 
limpa o buffer de entrada, para aguardar o proximo comando*/
void handle_signal(int signo)
{
	printf("\nSO2@G6:%s$ ",get_place());
	fflush(stdout);
}
/*
	Como o proprio nome diz, preenche o argv. Basicamente a função serve para colocar a entrada digitada, 
	na variável my_argv[index] e apartir disso o programa usa essa varaivel para executar o comando. 
	O programa irá unir o comando de entrada aos possíveis path para localizar o comando desejado
*/
void fill_argv(char *tmp_argv)
{
	char *foo = tmp_argv;
	int index = 0;
	char ret[100];
	bzero(ret, 100);
	while(*foo != '\0') {
		if(index == 10)
			break;

		if(*foo == ' ') {
			if(my_argv[index] == NULL)
				my_argv[index] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
			else {
				bzero(my_argv[index], strlen(my_argv[index]));
			}
			strncpy(my_argv[index], ret, strlen(ret));
			strncat(my_argv[index], "\0", 1);
			bzero(ret, 100);
			index++;
		} else {
			strncat(ret, foo, 1);
		}
		foo++;
		/*printf("foo is %c\n", *foo);*/
	}
	my_argv[index] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
	strncpy(my_argv[index], ret, strlen(ret));
	strncat(my_argv[index], "\0", 1);
}

//Copia as configurações do sistema. Tem como objetivo obter os possíveis PATH's do sistema
void copy_envp(char **envp)
{
	int index = 0;
	for(;envp[index] != NULL; index++) {
		my_envp[index] = (char *)malloc(sizeof(char) * (strlen(envp[index]) + 1));
		memcpy(my_envp[index], envp[index], strlen(envp[index]));
	}
}

//Separa tudo que foi copiado, de modo a localizar apenas aquelas que possuam PATH no nome.
/*
-> 	O erro do sistema é não tratar o fato de haver mais que uma string com PATH no nome.
	Logo para corrigir o erro basta realizar esse tratamento.

->	O PATH que procuramos é o PATH dos executáveis. No caso o nome dessa variável é apenas "PATH", logo devemos
	buscar uma variavel cujo nome seja apenas PATH
*/
int tratamento(char *vet){
	int k=0;
	//garante que o nome da string é PATH
	if(vet[0]=='P' && vet[1]=='A' && vet[2]=='T' && vet[3]=='H'){
		k=1;
	}

	return k;
}
void get_path_string(char **tmp_envp, char *bin_path)
{
	int count = 0;
	char *tmp;
	while(1) {
		tmp = strstr(tmp_envp[count], "PATH");
		if(tmp == NULL) {
			count++;
		} else {
			if(tratamento(tmp_envp[count]) == 1){
				break;
			}
			else{
				count++;
			}
		}
	}
        strncpy(bin_path, tmp, strlen(tmp));
}


/*
	Após a identificação da variável PATH, essa função separa todos os caminhos que a variável armazenava. Esses caminhos serão
	armazenados na variável search_path. Após separados cada caminho ocupará uma linha da matriz. Desse modo facilitando e
	agilizando a busca futura.
*/
void insert_path_str_to_search(char *path_str) 
{
	int index=0;
	char *tmp = path_str;
	char ret[100];

	while(*tmp != '=')
		tmp++;
	tmp++;
	bzero(ret, 100);//modificacao para evitar lixo no primeiro endereço de ret
	while(*tmp != '\0') {
		if(*tmp == ':') {
			strncat(ret, "/", 1);
			search_path[index] = (char *) malloc(sizeof(char) * (strlen(ret) + 1));
			strncat(search_path[index], ret, strlen(ret));
			strncat(search_path[index], "\0", 1);
			index++;
			bzero(ret, 100);
		} else {
			strncat(ret, tmp, 1);
		}
		tmp++;
	}
	//modificacao para que seja pego a ultima parte do path que estava sendo esquecida
	strncat(ret, "/", 1);
	search_path[index] = (char *) malloc(sizeof(char) * (strlen(ret) + 1));
	strncat(search_path[index], ret, strlen(ret));
	strncat(search_path[index], "\0", 1);
	index++;
	bzero(ret, 100);

	//search_path[0] = (char *) malloc(sizeof(char) * (6));
	//search_path[0] = "/bin/";
}


/*
	Essa função recebe o comando digitado pelo usuário(cmd). E então copia para uma variável auxiliar(ret) 
	um dos caminhos contidos em search_path, e depois acrescenta esse comando ao fim 
*/
int attach_path(char *cmd)
{

	char ret[100];
	int index=0;
	int fd;
	bzero(ret, 100);
	for(index=0;search_path[index]!=NULL;index++) {
		strcpy(ret, search_path[index]);
		strncat(ret, cmd, strlen(cmd));
		if((fd = open(ret, O_RDONLY)) > 0) {
			strncpy(cmd, ret, strlen(ret));
			close(fd);
			return 0;
		}
	}
	return 0;
}

/*
	Retorna o estado atual dos processos que estao em background
	desse modo podemos atualizar a lista de jobs
*/
void get_status(){
	ListNode * x;// sera o node auxiliar
	int status;
	pid_t pid;// pid pego da funcao

	x = JOBS->next;
	while(x!= JOBS){
		/*
			WNOHANG: return immediately if no child has exited.
			WUNTRACED:also return if a child has stopped (but not traced via ptrace(2)). Status for traced 
				children which have stopped is provided even if this option is not specified.
		*/
		pid = waitpid(x->PID,&status,WNOHANG|WUNTRACED);
		if(pid == x->PID){
			/*
				WIFEXITED: This macro returns a nonzero value if the child process terminated normally 
					with exit or _exit.
				-> Nesse caso o processo filho terminou normalmente, logo o processo pai foi concluido.
					Portanto marcamos como concluido.
			*/
			if (WIFEXITED(status)){
               	x->status = 0;
			} else if (WIFSIGNALED(status)) {
			/*
				WIFSIGNALED: This macro returns a nonzero value if the child process terminated because it 
					received a signal that was not handled.
				-> Como o processo filho foi terminado por meio de sinal de interrupcao, marcamos o processo
					pai como terminado 
			*/
				x->status = 2;
           	} else if (WIFSTOPPED(status)) {
             /*
				WIFSIGNALED: This macro returns a nonzero value if the child process is stopped.
				-> Nesse caso nao fazemos nada, uma vez que esse estado esta ligado ao processo filho.
			*/
           	}
                	
		}
		x = x->next;
    }
}
char cmd_entrada[30];//ultimo comando digitado no shell
int next_jid_count = 1;//como o nome diz, marcara o proximo jif a ser atribuido
// faz a chamada do executavel da funcao
void call_execve(char *cmd)
{
	ListNode * x;// sera o node auxiliar
	int i, j;
	int pid = fork();//pega o pid do process
	if(pid == 0) {
		i = execve(cmd, my_argv, my_envp);
		printf("errno is %d\n", errno);
		if(i < 0) {
			printf("%s: %s\n", cmd, "command not found");
			exit(1);		
		}
	} else {
		// So preciso chamar wait se o comando não tiver '&' no final
		x = listNodeAlloc (pid,next_jid_count,cmd_entrada, 1);
		listInsertTail (JOBS, x);
		next_jid_count++;//incrementa o contador
		if(FLAG == 0){
			waitpid(pid,NULL,0);//precisa ser NULL pois tem de esperar qualquer filho
			x->status = 0;
		}
	}

}

void free_argv()
{
	int index;
	for(index=0;my_argv[index]!=NULL;index++) {
		bzero(my_argv[index], strlen(my_argv[index])+1);
		my_argv[index] = NULL;
		free(my_argv[index]);
	}
}

/*-----------------------------------------------------------------------------------------------------------*/

/*	Alteracoes para a parte 2	*/

// funcao retorna '1' se o comando possui '&'' no final ou '0' caso contrario
int find_bg(char *aux_jobs){
	
	char *tmp = aux_jobs;
	char *ret = (char *)malloc(sizeof(char) * 100);
	int helper =0;//guardara o numero de caracteres da string

	bzero(ret, 100);//modificacao para evitar lixo no primeiro endereço de ret
	while(*tmp != '\0') {
		helper++;
		tmp++;
	}
	strcpy(cmd_entrada,aux_jobs);
	if(aux_jobs[helper-1] == '&'){
		aux_jobs[helper-2] = '\0';
		//strcpy(cmd_entrada,aux_jobs);
		return 1;
	}else{
		//strcpy(cmd_entrada,aux_jobs);
		return 0;
	}
}


/*
	Essa será a funcao que desenvolvera o comando 'cd' para mudar de diretorio
*/
void func_cd(char *tmp_argv){
	int index=0;
	char *tmp = tmp_argv;
	char ret[100];

	while(*tmp != ' ')
		tmp++;
	tmp++;
	bzero(ret, 100);//modificacao para evitar lixo no ret
	while(*tmp != '\0') {
		strncat(ret, tmp, 1);
		tmp++;
	}

	if(chdir(ret) < 0){
		printf("bash: cd: %s: No such file or directory\n",ret );
	}
}

//Simplesmente retorna o diretorio atual, adequando-o para o mesmo formato de print que o terminal do ubuntu faz.
char *get_place(){
	char *aux_jobs = (char *)malloc(sizeof(char) * 100);

	getcwd(aux_jobs, 100);
	char *tmp = aux_jobs;
	char *ret = (char *)malloc(sizeof(char) * 100);
	int helper =0;

	bzero(ret, 100);//modificacao para evitar lixo no primeiro endereço de ret
	while(*tmp != '\0') {
		if(helper >2) {
			strncat(ret, tmp, 1);
		} else {
			if(*tmp == '/')
				helper++;
			if(helper==3){// O '~' so sera inserido quando tiver '/home/claudio' no comeco do caminho
							// O '~' seria uma abreviacao para o caminho do direto "raiz"
				strncat(ret, "~", 1);//tratamento para o '~'
				strncat(ret, "/", 1);
			}
		}
		tmp++;
	}
	if(helper<3 && strcmp(aux_jobs,"/home/claudio")!=0){
		return aux_jobs;
	}else{
		if(strcmp(aux_jobs,"/home/claudio")==0){//if de tratamento para o '~'
			strncat(ret, "~", 1);
		}
		strncat(ret, "\0", 1);

		return ret;
	}
}


int main(int argc, char *argv[], char *envp[])
{
	char c;
	int i, fd;
	char *tmp = (char *)malloc(sizeof(char) * 100);
	char *path_str = (char *)malloc(sizeof(char) * 256);
	char *cmd = (char *)malloc(sizeof(char) * 100);
	
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);

	copy_envp(envp);
	get_path_string(my_envp, path_str);
	insert_path_str_to_search(path_str);

	//iniciando lista de jobs
	JOBS = listCreate();

	if(fork() == 0) {
		execve("/usr/bin/clear", argv, my_envp);
		exit(1);
	} else {
		wait(NULL);
	}
	printf("SO2@G6:%s$ ",get_place());
	fflush(stdout);
	while(c != EOF) {
		c = getchar();
		switch(c) {
			case '\n': if(tmp[0] == '\0') {
					   printf("SO2@G6:%s$ ",get_place());
				   } else {// parte 2
				   		/*
				   			Comando quit e exit, simplesmente finalizamos o while que mantem o programa rodando
				   			desse modo estaremos finalizando o terminal.
				   		*/
				   		FLAG = find_bg(tmp);
				   		if((tmp[0] == 'q' && tmp[1] == 'u' && tmp[2] == 'i' && tmp[3] == 't')
				   			|| (tmp[0] == 'e' && tmp[1] == 'x' && tmp[2] == 'i' && tmp[3] == 't')){
				   			c = EOF;
				   		}else{//tratamento funcao JOBS
				   			if(tmp[0] == 'j' && tmp[1] == 'o' && tmp[2] == 'b' && tmp[3] == 's'){
				   				get_status();
				   				listPrint(JOBS);
					   		}else{
					   			//Comando cd
						   		if(tmp[0] == 'c' && tmp[1] == 'd'){
						   			if(tmp[2] == ' ' && tmp[3] != '\0'){
						   				func_cd(tmp);
						   			}else{// essa parte trata o 	caso de se digitar apenas func_cd
						   				strcpy(tmp,"cd /home/claudio");
						   				func_cd(tmp);
						   			}
						   		}else{//parte 1
									   	fill_argv(tmp);
									   	strncpy(cmd, my_argv[0], strlen(my_argv[0]));
									   	strncat(cmd, "\0", 1);
									   	if(index(cmd, '/') == NULL) {
										   	if(attach_path(cmd) == 0) {
											   	call_execve(cmd);
										   	} else {
											   	printf("%s: command not found\n", cmd);
										   	}
									   	} else {
										   	if((fd = open(cmd, O_RDONLY)) > 0) {
											   	close(fd);	
												call_execve(cmd);
										   	} else {
											   	printf("%s: command not found\n", cmd);
										   	}
									}
							   	}// fim else cd
							}//fim else jobs
						    free_argv();
						    printf("SO2@G6:%s$ ",get_place());
						    bzero(cmd, 100);
						}//fim else quit e exit
				   }
				   bzero(tmp, 100);
				   break;
			default: strncat(tmp, &c, 1);
				 break;
		}
	}
	
	listTerminate (JOBS);//desalocando a fila

	free(tmp);
	free(path_str);
	for(i=0;my_envp[i]!=NULL;i++)
		free(my_envp[i]);
	for(i=0;i<10;i++)
		free(search_path[i]);
	printf("\n");

	return 0;
}

