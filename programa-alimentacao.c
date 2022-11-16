#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


const int MAX=15;// limitando o tamanho do vetor
int ordenado = 0;// vari�vel p/ controlar a ordena��o dos dados

// Tipo de dado especial (Registro)
typedef struct TProduto{
    long int codigo;
    int categoria, quantidade;
    char nome[40], desc[40];
    float valorUnitario, totalValorProduto;
}Tproduto;
// Escopo do programa
void leitura(Tproduto estoque[], int *tamanho);//gera o arquivo .dat na primeira vez
void gravacao(Tproduto estoque[], int tamanho);// realiza a grava��o dos dado no arquivo
int pesquisa(Tproduto estoque[], int codigo, int *tamanho);// busca e retorna a posi��o do produto no vetor
int pesquisabinaria(Tproduto estoque[], int chave, int tamanho);
int vazio(int tamanho);// fun��o auxiliar para da pesquisa bin�ria
void ordena(Tproduto estoque[], int tamanho); //ordena o cadastro dos produtos por c�digo
void cadastrar(Tproduto estoque[], int *tamanho); //faz o cadastro dos produtos

// Relatorios
void relatorioGeral(Tproduto estoque[], int *tamanho);
void relatorioCat(Tproduto estoque[], int *tamanho);
void custoTotal(Tproduto estoque[], int *tamanho);

//exibe os dados do produto
void mostra(Tproduto estoque[], int chave); 
void mostraCusto(Tproduto estoque[], int index);


int main(){
	setlocale(LC_ALL,"");
	Tproduto estoque[MAX];
    int tamanho=0, opcao, opc;
//    char nome[20];
    leitura(estoque, &tamanho);// abre o arquivo da base de dados
    do{
        printf ("------------------------------------------------------------\n");
		printf ("             CONTROLE DE COMPRA - ALIMENTA��O               \n");
		printf ("------------------------------------------------------------\n");
		printf ("0 - SAIR\n");
        printf ("1 - Cadastrar\n");
        printf ("2 - Relat�rio Geral\n");
        printf ("3 - Relat�rio por categoria\n");
    	printf ("4 - Custo total\n");
        printf ("\nESCOLHA A OPCAO DESEJADA:\n");
        scanf ("%d",&opcao);
        system("cls");
        switch (opcao){
            case 1:{
            	cadastrar(estoque, &tamanho);
            	gravacao(estoque, tamanho);
				break;
			}        	
            case 2:{
            	relatorioGeral(estoque, &tamanho);
				break;
			}
			case 3:{
				relatorioCat(estoque, &tamanho);
				break;
			}
			case 4:{
				custoTotal(estoque, &tamanho);
				break;
			}
            case 0: 
				printf("Obrigado por usar nosso sistema.\n");
				system("pause");
				exit(0);
				break;
            default: printf("OPCAO INVALIDA!");
        }
    }while (opcao!=0);
             gravacao(estoque, tamanho);// grava ap�s realizada as opera��es
	return 0;
}


void leitura(Tproduto estoque[], int *tamanho){
    FILE *arquivo;
  	arquivo= fopen("estoque.dat", "a+b");	   /* abrimos para anexar, bin�rio */ 
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
//le os dados do arquivo
    while(!feof(arquivo)){
       fread (&estoque[*tamanho], sizeof (Tproduto) , 1, arquivo);
       (*tamanho)++;
    }
	(*tamanho)--;
//fecha o arquivo
	fclose(arquivo);
	return;
}


void gravacao(Tproduto estoque[], int tamanho){
    FILE *arquivo;
    int i;
    arquivo= fopen("estoque.dat", "w+b");	   /* abre e apaga o conte�do do arquivo,bin�rio */ 
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
// grava a estrutura no arquivo
    for(i=0;i<tamanho;i++)  
       fwrite(&estoque[i], sizeof(Tproduto), 1, arquivo);
// Fecha o arquivo de dados
	fclose(arquivo);
	return;
}


int pesquisabinaria(Tproduto estoque[], int chave, int tamanho){
    if(vazio(tamanho)) //vetor vazio
       return -1;       
    if (! ordenado){
        ordena(estoque,tamanho);
        ordenado=1;
    }
    int inicio=0,final=tamanho, meio;
    while (inicio<=final){
        meio=(int)(inicio+final)/2;
        if (estoque[meio].codigo==chave)
           return meio; // encontrou
        if (estoque[meio].codigo<chave)
           inicio=meio+1;
        else
           final=meio-1;
    }
    return -1; // n�o encontrou
}


int pesquisa(Tproduto estoque[], int codigo, int *tamanho){
	if(*tamanho == 0){
		return -2;
	}
	int i;
	for(i; i < *tamanho; i++){
		if(estoque[i].codigo == codigo){
			return 1;
		}
	}
	return -1;
}


int vazio(int tamanho){
     if(tamanho==0){
        printf("\nREGISTRO VAZIO!\n");
        return 1;
     }
     return 0;
}


void ordena(Tproduto estoque[], int tamanho){
	int i,j;
    Tproduto aux;
    for(i=0;i<tamanho-1;i++)
        for(j=i+1;j<tamanho;j++)
          	if (estoque[i].codigo>estoque[j].codigo){
              	aux=estoque[i];
              	estoque[i]=estoque[j];
              	estoque[j]=aux;
            }
}


void cadastrar(Tproduto estoque[], int *tamanho){
	Tproduto aux;
	
	//Verifica se o arquivo est� cheio
	if(*tamanho == MAX){
		printf("Arquivo cheio! \n");
		return;
	}
	
	// solicita o codigo do produto
	do{
		printf("C�digo do produto--------------------------: ");
		scanf("%ld", &aux.codigo);
		fflush(stdin);
		
		if(aux.codigo < 0){
			printf("O c�digo n�o pode ser negativo.\n");
		}	
		
	}while(aux.codigo < 0);
	
	// Verifica se j� existe um produto com o mesmo c�digo
	if(pesquisa(estoque, aux.codigo, tamanho) > 0){
		printf("\nPRODUTO JA EXISTE\n");
		return;	
	}
	
	// Nome do produto
	do{
		printf("Nome---------------------------------------: ");
		gets(aux.nome);
		fflush(stdin);
		if(aux.nome[0] == '\0'){
			printf("O campo nome n�o pode ser vazio. \n");	
		}
		
	}while(aux.nome[0] == '\0');
	
	
	// Especifica��o do produto
	do{
		printf("Especifica��o------------------------------: ");
		gets(aux.desc);
		fflush(stdin);
		if(aux.desc[0] == '\0'){
			printf("O campo nome n�o pode ser vazio. \n");	
		}
		
	}while(aux.desc[0] == '\0');
	
	
	// Quantidade do produto
	do{
		printf("Quantidade---------------------------------: ");
		scanf("%i", &aux.quantidade);
		fflush(stdin);
		if(aux.quantidade < 0){
			printf("A quantidade n�o pode ser negativa.\n");	
		}
	}while(aux.quantidade < 0);
	
	
	// Categoria do produto
	do{
		printf("Categoria:\n[1] refei��o\n[2] bebidas\n[3] sobremesas\n");
		printf("Selecione uma Categoria--------------------: ");
		scanf("%i", &aux.categoria);
		fflush(stdin);
		if(aux.categoria != 1 && aux.categoria != 2 && aux.categoria != 3)
			printf("Digite uma categoria v�lida\n");
	}while(aux.categoria != 1 && aux.categoria != 2 && aux.categoria != 3);
	
	
	// Valor unit�rio do produto
	do{
		printf("Valor unit�rio-----------------------------: ");
		scanf("%f", &aux.valorUnitario);
		fflush(stdin);
		if(aux.valorUnitario < 0){
			printf("O valor unit�rio n�o pode ser negativo.\n");
		}
	}while(aux.valorUnitario < 0);
	
	aux.totalValorProduto = aux.valorUnitario * aux.quantidade;
		
	
	int validaCadastro;
	printf("As informa��es est�o corretas?\n 1 - Sim   0 - N�o \n");
	scanf("%i", &validaCadastro);
	fflush(stdin);
	if(validaCadastro == 1){
		
		estoque[*tamanho] = aux;
		(*tamanho) ++;
		ordenado = 0;
		
		printf("\nCadastro Efetuado com sucesso!\n\n");
		system("cls");
	}
}


//Fun��o de relat�rio geral
void relatorioGeral(Tproduto estoque[], int *tamanho){
	int index, pos, cod;
	pos = pesquisabinaria(estoque, cod, *tamanho);
	
	printf ("\n");
	printf ("                          *********************     RELAT�RIO GERAL     ********************* \n");
	printf ("\n");
	printf("-------------------------------------------------------------------------------------------------------------------------\n");
	printf(" C�digo     | Nome                           | Especifica��o                  | Categoria  | Quantidade  | Valor Unit�rio \n");
	printf("-------------------------------------------------------------------------------------------------------------------------\n");
	for(index = 0; index < *tamanho; index++){
		mostra(estoque, index);
		
//		if(index % 2 == 0){
//			system("pause");
//			system("cls");
//		}
	}
	printf ("\n");
	system("pause");
	system("cls");
}


//Fun��o de relat�rio por categoria
void relatorioCat(Tproduto estoque[], int *tamanho){
	int index, pos, cod, opc;
	pos = pesquisabinaria(estoque, cod, *tamanho);
	printf ("\n");
	printf ("                        *****************     RELAT�RIO POR CATEGORIA     ***************** \n");
	printf ("\n");
	
	// Categoria do produto
	do{
		printf("Categoria:\n[1] refei��o\n[2] bebidas\n[3] sobremesas\n");
		printf("Selecione uma Categoria------------------: ");
		scanf("%i", &opc);
		fflush(stdin);
		if(opc != 1 && opc != 2 && opc != 3)
			printf("Digite uma categoria v�lida\n");
	}while(opc != 1 && opc != 2 && opc != 3);
	
	printf("-------------------------------------------------------------------------------------------------------------------------\n");
	printf(" C�digo    | Nome                           | Especifica��o                  | Categoria  | Quantidade  | Valor Unit�rio \n");
	printf("-------------------------------------------------------------------------------------------------------------------------\n");
	for(index = 0; index < *tamanho; index++){
		if(estoque[index].categoria == opc){
			mostra(estoque, index);	
		}
	}
	printf ("\n");
	system("pause");
	system("cls");
}


//Fun��o de custos do produto
void custoTotal(Tproduto estoque[], int *tamanho){
	int index, pos, cod, opc;
	float valorTotal = 0;
	
	pos = pesquisabinaria(estoque, cod, *tamanho);
	printf ("\n");
	printf ("            ***********************     CUSTO TOTAL     *********************** \n");
	printf ("\n");
	printf("----------------------------------------------------------------------------------------------\n");
	printf(" C�digo     | Nome                           | Quantidade  | Valor Unit�rio     | Total       \n");
	printf("----------------------------------------------------------------------------------------------\n");
	for(index = 0; index < *tamanho; index++){
		mostraCusto(estoque, index);	
		valorTotal = valorTotal + estoque[index].totalValorProduto; // Faz a soma do total de cada produto
	}
	printf ("\n");
	printf (" Total acumulado --------------------------------------------------------------   R$ %.2f\n", valorTotal);
	printf ("\n");
	system("pause");
	system("cls");
}


// Fun��o para exibir dados do produto
void mostra(Tproduto estoque[], int index){
	printf(" %-12ld %-32s %-32s %-12i %-13i R$ %.2f \n", estoque[index].codigo, estoque[index].nome, estoque[index].desc, estoque[index].categoria, estoque[index].quantidade, estoque[index].valorUnitario);
	return;
}


// Fun��o para exibir custos do produto
void mostraCusto(Tproduto estoque[], int index){
	printf(" %-12ld %-32s %-13i R$ %-17.2f R$ %.2f \n", estoque[index].codigo, estoque[index].nome, estoque[index].quantidade, estoque[index].valorUnitario, estoque[index].totalValorProduto);
	return;
}
