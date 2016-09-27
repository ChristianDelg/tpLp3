#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
/* Variable global con el pid del proceso hijo */
int pid = 0;
/* Rutina de tratamiento del mensaje SIGINT desde donde se va a mandar el mensaje
al proceso hijo para que éste muera */
void gestor_SIGINT(int senhal){
    printf("Rutina de tratamiento de SIGINT\n");
    if(pid != 0)
    {
        kill(pid, SIGUSR1);
        exit(0);
    }
}
void gestor_hijo(int senhal){
    exit(0);
}

main(void){
    int codigo_error=0;
    struct sigaction gestion;
    /* Primero se instala el gestor de la señal SIGINT */
    gestion.sa_handler = gestor_SIGINT;
    gestion.sa_mask = 0;
    gestion.sa_flags = SA_ONESHOT;
    codigo_error = sigaction(SIGINT, gestion, 0);
    if(codigo_error == SIG_ERR){
        perror("Error al definir el gestor de SIGINT");
        exit(-1);
    }
    pid = fork();
    if(pid == -1){
        printf("Error en la creación del hijo\n");
        exit( 1 );
    }
    else if(pid == 0){
        /* Código del hijo */
        /* Primero se desactiva la respuesta a SIGINT */
        gestion.sa_handler = SIG_IGN;
        gestion.sa_mask = 0;
        gestion.sa_flags = SA_ONESHOT;
        codigo_error = sigaction(SIGINT, gestion, 0);
        if(codigo_error == SIG_ERR)
        {
            perror("Error al definir el gestor de SIGINT");
            exit(-1);
        }
        /* Segundo se instala el gestor de SIGUSR1 */
        gestion.sa_handler = gestor_hijo;
        gestion.sa_mask = 0;
        gestion.sa_flags = SA_ONESHOT;
        codigo_error = sigaction(SIGUSR1,gestion,0);
        if(codigo_error == SIG_ERR){
            perror("Error al definir el gestor de SIGUSR1");
            exit(-1);
        }
        /* Código */
        while(1);
    }
    else{
        /* Código del padre */
        while(1);
    }
}