#include <stdio.h>
#include <stdlib.h>

struct tProduto
{
  int codigo;
  char descricao[10];
  float valor;
  char deletado;
};

int menu();
void inclusao();
void listar();
void buscar();
void alterar();
void excluir();
void organizar();
void copiar();

int main(void)
{
  int op;

  do
  {
    op = menu();
    switch (op)
    {

    case 1:
      printf("\nIncluir\n");
      inclusao();
      break;

    case 2:
      printf("Listar\n");
      listar();
      break;

    case 3:
      printf("Buscar\n");
      buscar();
      break;

    case 4:
      printf("Alterar\n");
      alterar();
      break;

    case 5:
      printf("Excluir\n");
      excluir();
      break;
    }
  } while (op != 0);

  return 0;
}

int menu(void)
{

  int opcao;
  do
  {
    printf("\nMENU PARA CADASTRO DE PRODUTOS\n");
    printf("1. Incluir\n");
    printf("2. Listar\n");
    printf("3. Buscar\n");
    printf("4. Alterar\n");
    printf("5. Excluir\n");
    printf("0. Sair\n\n");
    printf("Digite sua opcao: ");
    scanf("%d", &opcao);
    if ((opcao < 0) || (opcao > 5))
      printf("Opcao selecionada incorreta!\n");
  } while ((opcao < 0) || (opcao > 5));

  return opcao;
}

void inclusao()
{
  struct tProduto produtos;

  FILE *arq = fopen("produtos.txt", "ab");
  if (arq == NULL)
  {
    printf("Erro ao abrir arquivo");
    return;
  }
  printf("Digite o codigo do produto: \n");
  scanf("%d", &produtos.codigo);
  printf("Digite a descricao do produto: \n");
  fflush(stdin);
  gets(produtos.descricao);
  printf("Digite o valor do produto: \n");
  scanf("%f", &produtos.valor);
  fwrite(&produtos, sizeof(produtos), 1, arq);
  fclose(arq);
}

void listar()
{

  struct tProduto produtos;
  FILE *arq = fopen("produtos.txt", "rb");
  if (arq == NULL)
  {
    printf("Arquivo inexistente ou vazio!");
    return;
  }

  while (fread(&produtos, sizeof(produtos), 1, arq))
    printf("Cod. %d - Produto: %-8s - Valor R$ %4.2f - Deletado?(%c)\n", produtos.codigo, produtos.descricao, produtos.valor, produtos.deletado);

  fclose(arq);
}

void buscar()
{

  FILE *arq = fopen("produtos.txt", "rb");
  if (arq == NULL)
  {
    printf("Arquivo inexistente!");
    return;
  }

  struct tProduto produtos;
  int cod, achei = 0;
  printf("\nDigite o codigo que procura: \n");
  scanf("%d", &cod);

  while (fread(&produtos, sizeof(produtos), 1, arq))
  {
    if ((cod == produtos.codigo) && (produtos.deletado != '*'))
    {
      printf("Cod. %d - Produto: %-8s - Valor R$ %4.2f\n", produtos.codigo, produtos.descricao, produtos.valor);
      achei = 1;
    }
  }

  if (!achei)
    printf("\nCodigo nao cadastrado!!\n");

  fclose(arq);
}

void alterar()
{
  FILE *arq = fopen("produtos.txt", "r+b");
  if (arq == NULL)
  {
    printf("Arquivo inexistente!");
    return;
  }

  struct tProduto produtos;
  int cod, achei = 0;
  printf("\nDigite o codigo que deseja alterar: \n");
  scanf("%d", &cod);

  while (fread(&produtos, sizeof(produtos), 1, arq))
  {
    if (cod == produtos.codigo)
    {
      printf("Cod. %d - Produto: %-8s - Valor R$ %4.2f\n", produtos.codigo, produtos.descricao, produtos.valor);
      achei = 1;

      fseek(arq, sizeof(struct tProduto) * -1, SEEK_CUR);
      printf("\nDigite a nova descricao: \n");
      fflush(stdin);
      gets(produtos.descricao);
      printf("\nDigite o novo preco....: \n");
      scanf("%f", &produtos.valor);

      fwrite(&produtos, sizeof(produtos), 1, arq);
      fseek(arq, sizeof(produtos) * 0, SEEK_END);
      return;
    }
  }

  if (!achei)
    printf("\nCodigo nao cadastrado!!\n");

  fclose(arq);
}

void excluir()
{

  FILE *arq = fopen("produtos.txt", "r+b");
  if (arq == NULL)
  {
    printf("Arquivo inexistente!");
    return;
  }

  struct tProduto produtos;
  int cod, achei = 0;
  char certeza;
  printf("\nDigite o codigo que deseja EXCLUIR: \n");
  scanf("%d", &cod);

  while (fread(&produtos, sizeof(produtos), 1, arq))
  {
    if (cod == produtos.codigo)
    {
      printf("Cod. %d - Produto: %-8s - Valor R$ %4.2f\n", produtos.codigo, produtos.descricao, produtos.valor);
      achei = 1;

      printf("\nTem certeza que quer excluir este produto? s/n \n");
      fflush(stdin);
      scanf("%c", &certeza);
      if (certeza == 's')
      {
        produtos.deletado = '*';
        printf("\nProduto excluido com Sucesso! \n");
        fseek(arq, sizeof(struct tProduto) * -1, SEEK_CUR);
        fwrite(&produtos, sizeof(produtos), 1, arq);
        fseek(arq, sizeof(produtos) * 0, SEEK_END);
        return;
      }
      else if (certeza == 'n')
        return;
    }
  }

  if (!achei)
    printf("\nCodigo nao cadastrado!!\n");

  fclose(arq);
}