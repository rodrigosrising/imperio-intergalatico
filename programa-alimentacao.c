#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


const int MAX=15;// limitando o tamanho do vetor
int ordenado = 0;// variável p/ controlar a ordenação dos dados

// Tipo de dado especial (Registro)
typedef struct TProduto{
    long int codigo;
    int categoria, quantidade;
    char nome[40], desc[40];
    float valorUnitario, totalValorProduto;
}Tproduto;
// Escopo do programa
void leitura(Tproduto estoque[], int *tamanho);//gera o arquivo .dat na primeira vez
void gravacao(Tproduto estoque[], int tamanho);// realiza a gravação dos dado no arquivo
int pesquisa(Tproduto estoque[], int codigo, int *tamanho);// busca e retorna a posição do produto no vetor
int pesquisabinaria(Tproduto estoque[], int chave, int tamanho);
int vazio(int tamanho);// função auxiliar para da pesquisa binária
void ordena(Tproduto estoque[], int tamanho); //ordena o cadastro dos produtos por código
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
		printf ("             CONTROLE DE COMPRA - ALIMENTAÇÃO               \n");
		printf ("------------------------------------------------------------\n");
		printf ("0 - SAIR\n");
        printf ("1 - Cadastrar\n");
        printf ("2 - Relatório Geral\n");
        printf ("3 - Relatório por categoria\n");
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
             gravacao(estoque, tamanho);// grava após realizada as operações
	return 0;
}


void leitura(Tproduto estoque[], int *tamanho){
    FILE *arquivo;
  	arquivo= fopen("estoque.dat", "a+b");	   /* abrimos para anexar, binário */ 
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
    arquivo= fopen("estoque.dat", "w+b");	   /* abre e apaga o conteúdo do arquivo,binário */ 
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
    return -1; // não encontrou
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
	
	//Verifica se o arquivo está cheio
	if(*tamanho == MAX){
		printf("Arquivo cheio! \n");
		return;
	}
	
	// solicita o codigo do produto
	do{
		printf("Código do produto--------------------------: ");
		scanf("%ld", &aux.codigo);
		fflush(stdin);
		
		if(aux.codigo < 0){
			printf("O código não pode ser negativo.\n");
		}	
		
	}while(aux.codigo < 0);
	
	// Verifica se já existe um produto com o mesmo código
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
			printf("O campo nome não pode ser vazio. \n");	
		}
		
	}while(aux.nome[0] == '\0');
	
	
	// Especificação do produto
	do{
		printf("Especificação------------------------------: ");
		gets(aux.desc);
		fflush(stdin);
		if(aux.desc[0] == '\0'){
			printf("O campo nome não pode ser vazio. \n");	
		}
		
	}while(aux.desc[0] == '\0');
	
	
	// Quantidade do produto
	do{
		printf("Quantidade---------------------------------: ");
		scanf("%i", &aux.quantidade);
		fflush(stdin);
		if(aux.quantidade < 0){
			printf("A quantidade não pode ser negativa.\n");	
		}
	}while(aux.quantidade < 0);
	
	
	// Categoria do produto
	do{
		printf("Categoria:\n[1] refeição\n[2] bebidas\n[3] sobremesas\n");
		printf("Selecione uma Categoria--------------------: ");
		scanf("%i", &aux.categoria);
		fflush(stdin);
		if(aux.categoria != 1 && aux.categoria != 2 && aux.categoria != 3)
			printf("Digite uma categoria válida\n");
	}while(aux.categoria != 1 && aux.categoria != 2 && aux.categoria != 3);
	
	
	// Valor unitário do produto
	do{
		printf("Valor unitário-----------------------------: ");
		scanf("%f", &aux.valorUnitario);
		fflush(stdin);
		if(aux.valorUnitario < 0){
			printf("O valor unitário não pode ser negativo.\n");
		}
	}while(aux.valorUnitario < 0);
	
	aux.totalValorProduto = aux.valorUnitario * aux.quantidade;
		
	
	int validaCadastro;
	printf("As informações estão corretas?\n 1 - Sim   0 - Não \n");
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


//Função de relatório geral
void relatorioGeral(Tproduto estoque[], int *tamanho){
	int index, pos, cod;
	pos = pesquisabinaria(estoque, cod, *tamanho);
	
	printf ("\n");
	printf ("                          *********************     RELATÓRIO GERAL     ********************* \n");
	printf ("\n");
	printf("-------------------------------------------------------------------------------------------------------------------------\n");
	printf(" Código     | Nome                           | Especificação                  | Categoria  | Quantidade  | Valor Unitário \n");
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


//Função de relatório por categoria
void relatorioCat(Tproduto estoque[], int *tamanho){
	int index, pos, cod, opc;
	pos = pesquisabinaria(estoque, cod, *tamanho);
	printf ("\n");
	printf ("                        *****************     RELATÓRIO POR CATEGORIA     ***************** \n");
	printf ("\n");
	
	// Categoria do produto
	do{
		printf("Categoria:\n[1] refeição\n[2] bebidas\n[3] sobremesas\n");
		printf("Selecione uma Categoria------------------: ");
		scanf("%i", &opc);
		fflush(stdin);
		if(opc != 1 && opc != 2 && opc != 3)
			printf("Digite uma categoria válida\n");
	}while(opc != 1 && opc != 2 && opc != 3);
	
	printf("-------------------------------------------------------------------------------------------------------------------------\n");
	printf(" Código    | Nome                           | Especificação                  | Categoria  | Quantidade  | Valor Unitário \n");
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


//Função de custos do produto
void custoTotal(Tproduto estoque[], int *tamanho){
	int index, pos, cod, opc;
	float valorTotal = 0;
	
	pos = pesquisabinaria(estoque, cod, *tamanho);
	printf ("\n");
	printf ("            ***********************     CUSTO TOTAL     *********************** \n");
	printf ("\n");
	printf("----------------------------------------------------------------------------------------------\n");
	printf(" Código     | Nome                           | Quantidade  | Valor Unitário     | Total       \n");
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


// Função para exibir dados do produto
void mostra(Tproduto estoque[], int index){
	printf(" %-12ld %-32s %-32s %-12i %-13i R$ %.2f \n", estoque[index].codigo, estoque[index].nome, estoque[index].desc, estoque[index].categoria, estoque[index].quantidade, estoque[index].valorUnitario);
	return;
}


// Função para exibir custos do produto
void mostraCusto(Tproduto estoque[], int index){
	printf(" %-12ld %-32s %-13i R$ %-17.2f R$ %.2f \n", estoque[index].codigo, estoque[index].nome, estoque[index].quantidade, estoque[index].valorUnitario, estoque[index].totalValorProduto);
	return;
}
