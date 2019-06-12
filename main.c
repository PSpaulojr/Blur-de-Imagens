#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <imageprocessing.h>

#define N_PROCESSOS  3


void avaliar_vizinhanca_red(int k, int z, imagem *img, imagem img2, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img2.r[j*(img2.width) + i];
    }
  }
  soma /= 2*N*2*N;
  (img->r)[z*(img2.width) + k] = soma;
  //printf("img2 -> r = %f\n", img2.r[z*(img->width) + k] );
  //printf("img -> r = %f\n", img->r[z*(img->width) + k] );
}

void avaliar_vizinhanca_blue(int k, int z, imagem *img, imagem img2, int N){
  float soma= 0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img2.b[j*(img2.width) + i];
    }
  }
  soma /= 2*N*2*N;
  (img->b)[z*(img2.width) + k] = soma;
}

void avaliar_vizinhanca_green(int k, int z, imagem *img, imagem img2, int N){
  float soma = 0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img2.g[j*(img2.width) + i];
    }
  }
  soma /= 2*N*2*N;
  (img->g)[z*(img2.width) + k] = soma;
}

int main(){

    imagem img;
    imagem *img2;

    
    int nucleo;

    pid_t filho[N_PROCESSOS];

    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANON;

    img2 = (imagem*) mmap(NULL, sizeof(imagem), protection, visibility, 0, 0);
    img = abrir_imagem("data/cachorro.jpg");

    img2->width = img.width;
    img2->height = img.height;

   /* printf("img.width = %d\n", img.width );
    printf("img3 -> width = %d\n", img3->width );
    printf("img.r[j*(img.r) + i] = %f\n", img.r[50*(img2.width) + 50] );
    printf("img3->r[j*(img3->r) + i] = %f\n", img3->r[50*(img.width) + 50] );  */
    


    if( (img.width) > 600){
        nucleo = 10;
    }
    else nucleo = 3; 

    printf("Nucleo é: %d\n",nucleo);


    /*Criar processos filhos*/

    for(int k = 0; k < N_PROCESSOS ; k++){
        filho[k] = fork();

        if (filho[k]==0) {

            /* Processo filho(k) Opera */ 
            printf("Filho[%d]\n", k);
            int i, j;

            for (int i = k; i<(img.width); i += 3) {
                for (int j = 0; j<(img.height); j++) {
                    if( (i >= nucleo) && (j >= nucleo) && ( (img.width) - i > nucleo) && ( (img.height) - j > nucleo) ){
                        avaliar_vizinhanca_red(i, j, img2, img, nucleo);
                        avaliar_vizinhanca_blue(i, j, img2, img, nucleo);
                        avaliar_vizinhanca_green(i, j, img2, img, nucleo);
                    }
                }
            }
            exit(0);
        }
    }


    for (int k = 0; k < N_PROCESSOS ; k++){
        waitpid (filho[k], NULL, 0);
    }
    printf("img3->r[j*(img3->r) + i] = %f\n", img3->r[50*(img.width) + 50] );
    img = (*img3);

    salvar_imagem("cachorro-out.jpg", &img);
    liberar_imagem(&img);
    liberar_imagem(&img2);
    
    return 0;
}
