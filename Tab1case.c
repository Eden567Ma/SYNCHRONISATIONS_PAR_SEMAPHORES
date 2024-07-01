
  #include <stdio.h> 
  #include <unistd.h> 
  #include <stdlib.h> 
  #include <signal.h> 
  #include <libipc.h>


  #define NE          2     
  #define NR          5    

#define EMET  0
#define MUTEX 6
int RECEP[NR] = {1,2,3,4,5};
        

typedef struct{
  int a;
  int nb_recep;
}t_segpart;

t_segpart *sp;


int emet_pid[NE], recep_pid[NR];


  void handle_sigint(int sig) { 
      int i;
      for (i = 0; i < NE; i++) kill(emet_pid[i], SIGKILL); 
      for (i = 0; i < NR; i++) kill(recep_pid[i], SIGKILL); 
      det_sem(); 
      det_shm((char *)sp); 
  } 


void emetteur(int i){

  while(1){
    P(EMET);
    sleep(3);
    sp->a++;
    for (int j = 0; j < NR; j++)
      V(RECEP[j]);
    printf("Emetteur %d a fini d'ecrire\n",i);
  }
  
}

void recepteur(int i){

  while(1){
    P(RECEP[i]);
    printf("Recepteur %d lit %d\n",i,sp->a);
    P(MUTEX);
    sp->nb_recep++;
    if(sp->nb_recep == NR){
      V(EMET);
      sp->nb_recep = 0;
    }
    V(MUTEX);
  }
  
}

int main() { 
    struct sigaction action;
 
    setbuf(stdout, NULL);
    int pid;


    if( (sp = (t_segpart *) init_shm(sizeof(t_segpart))) == NULL){
      perror("init shm");
      exit(1);
    }
    sp -> a = 0;

    if( creer_sem(NR+2) == -1){
      perror("creer sem");
      exit(1);
    }


    init_un_sem(EMET,1);
    for(int i = 0; i < NR; i++)
      init_un_sem(RECEP[i],0);
    init_un_sem(MUTEX,1);

    for(int i = 0; i < NE ; i++){
      if((pid=fork()) == -1){
	perror("fork");
	exit(1);
      }
      if(pid==0)
	emetteur(i);
    }

    for(int i = 0; i < NR ; i++){
      if((pid=fork()) == -1){
	perror("fork");
	exit(1);
      }
      if(pid==0)
	recepteur(i);
    }
   sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = handle_sigint;
    sigaction(SIGINT, &action, 0); 
    
    pause();                    
    return EXIT_SUCCESS;
} 
