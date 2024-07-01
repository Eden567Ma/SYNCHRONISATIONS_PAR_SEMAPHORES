
  #include <stdio.h> 
  #include <unistd.h> 
  #include <signal.h> 
  #include <libipc.h>



  #define NE          2    
  #define NR          5    
  #define NMAX        3    
#define MUTEX 0
int EMET[NMAX]  = {1,2,8};
int RECEP[NR] = {3,4,5,6,7};


typedef struct{
  int a[NMAX];
  int nb_recep[NMAX];
}t_segpart;

    int emet_pid[NE], recep_pid[NR]; 


  void handle_sigint(int sig) 
  { int i;
  	for (i = 0; i < NE; i++) kill(emet_pid[i], SIGKILL); 
	for (i = 0; i < NR; i++) kill(recep_pid[i], SIGKILL); 
	det_sem(); 
	det_shm((char *)sp); 
 
  } 


void emetteur(int c,int i){

  while(1){
    


int main() { 
    struct sigaction action;
  
    
    setbuf(stdout, NULL);

   

    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = handle_sigint;
    sigaction(SIGINT, &action, 0); 

    pause();                     
    return EXIT_SUCCESS;
} 
