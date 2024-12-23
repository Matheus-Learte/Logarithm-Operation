#include <stdio.h>
#include <stdlib.h>//Biblioteca em que temos as funções de alocação de memória
#include <string.h>//Biblioteca em que temos funções que lidam com strings como a strlen() 
#include <math.h>//Biblioteca em que temos a função log()

/* Programa que consiste em receber uma foto para ser proecessada e tentar revelar
informações ocultas*/

//Definição do tipo FOTO
typedef struct picture_ FOTO;

//Estrutura que guarda todas as informações da foto
struct picture_{
    char* nome; //Nome do arquivo
    int tipo; //Modo que o arquivo é armazenado(caractere ou binário)
    int X;  //Tamanho da foto em pixels na horizontal
    int Y; //Tamanho da foto em pixels na vertical
    int max; //Maior byte
   unsigned char** foto; //Matriz que contém todos os pixels da foto
};

/*Essa função tem o intuito ler o nome do arquivo, abri-lo para leitura e guardar o tamanho da foto, o maior byte que existe
e guardar cada byte em uma matriz*/
void abri(FOTO* foto){
    char buffer[256];
    FILE *fp;

    //Leitura do nome do arquivo
    scanf("%s", buffer);
    foto->nome=malloc(sizeof(char)*(strlen(buffer)+2));
    strcpy(foto->nome, buffer);

    //Abertura do arquivo para leitura
    fp=fopen(foto->nome, "r");

    //Leitura do modo como os bytes são armazenados, ou seja, se são armazenados em formato de caractere ou binário
    fscanf(fp, "%s", buffer);

    fseek(fp, 1, SEEK_CUR);

    if(!strcmp(buffer, "P2")){
        foto->tipo=2;
    }else
        foto->tipo=5;
    
    //Leitura para ignorar uma linha do arquivo
    fscanf(fp, "%[^\n]", buffer);

    //Leitura das dimensões da foto
    fscanf(fp, "%d", &foto->X); 
    fscanf(fp, "%d", &foto->Y); 

    //Leitura do maior byte da foto
    fscanf(fp, "%d", &foto->max);

    //Alocação da matriz
    foto->foto=(unsigned char**)malloc(sizeof(unsigned char*)*foto->Y);

    for(int i=0; i<foto->Y; i++){
        foto->foto[i]=malloc(sizeof(unsigned char)*foto->X);
    }

    int aux;

    //Leitura de cada um dos bytes
    if(foto->tipo==2){
        //Armazenado no formato ASCII
        for(int i=0; i<foto->Y; i++){
            for(int j=0; j<foto->X; j++){
                fscanf(fp, "%d", &aux);
                foto->foto[i][j]=aux;
            }
        }
    }else{
        //Armazenado no formato binário
        fseek(fp, 1, SEEK_CUR);

        for(int i=0; i<foto->Y; i++){
            for(int j=0; j<foto->X; j++){
                fread(&foto->foto[i][j], 1, 1, fp);
            }
        }
    }

    fclose(fp);
}

/*Função que pega cada um dos pixels da foto e processa elas se utilizando da função y=c*log(1+x), tal que c=255/log(1+max),
x é o pixel da foto e y é o novo pixel*/
void operacao(FOTO* foto){
    double c=255.0/(log(1.0+foto->max)); //Criando e calculando a constante c que será utilizada no processamento da cada pixel
    int pixel;

    for(int i=0; i<foto->Y; i++){
        for(int j=0; j<foto->X; j++){
            pixel=c*log(1+foto->foto[i][j]);//Processando cada pixel

            if(pixel>255){
                pixel=255;
            }

            foto->foto[i][j]=pixel;//Guardando esse novo pixel na mesma posição do pixel antes de ser processado
        }
    }
}

/*Função que comanda a chamada de cada uma das funções que faram a leitura, o processamento de cada um dos bytes
e a impressão da foto em um novo arquivo*/
int main(void){
    FOTO foto;
    FILE *fp;

    abri(&foto);
    operacao(&foto);

    //Novo nome do arquivo
    int tam= strlen(foto.nome);
    int i=tam-1;
    
    while(foto.nome[i+1]!='.'){
        foto.nome[i+1]=foto.nome[i];
        i--;
    }

    foto.nome[i+1]='C';

    //Criando um novo arquivo para a foto processada
    fp=fopen(foto.nome, "w");

    //Cabeçalho da foto
    fprintf(fp, "P2\n");
    fprintf(fp, "# CREATOR: Image Generator Math-Férias\n");
    fprintf(fp, "%d %d\n", foto.X, foto.Y);
    fprintf(fp, "%d\n", foto.max);

    //Escrevendo cada pixel no novo arquivo (o novo arquivo sempre será armazenado no formato ASCII)
    for(int i=0; i<foto.Y; i++){
        for(int j=0; j<foto.X; j++){
            fprintf(fp, "%d\n", foto.foto[i][j]);
        }
    }

    fclose(fp);

return 0;
}
