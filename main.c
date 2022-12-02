#include <stdio.h>
#include <stdlib.h>

struct tProduto
{
  int codigo;
  char nome[10];
  float valor;
  char deletado;
};

int menu();
void adicionar();
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
      printf("\nAdicionar\n");
      adicionar();
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

    case 6:
      printf("Organizar\n");
      organizar();
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
    printf("1. Adicionar\n");
    printf("2. Listar\n");
    printf("3. Buscar\n");
    printf("4. Alterar\n");
    printf("5. Excluir\n");
    printf("6. Organizar\n");
    printf("0. Sair\n\n");
    printf("Digite sua opcao: ");
    scanf("%d", &opcao);
    if ((opcao < 0) || (opcao > 6))
      printf("Opcao selecionada incorreta!\n");
  } while ((opcao < 0) || (opcao > 6));

  return opcao;
}

void adicionar()
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
  printf("Digite o nome do produto: \n");
  fflush(stdin);
  gets(produtos.nome);
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
    printf("Arquivo nao existe ou está vazio.");
    return;
  }

  while (fread(&produtos, sizeof(produtos), 1, arq))
    printf("Cod. %d - Produto: %-8s - Valor R$ %4.2f - Deletado?(%c)\n", produtos.codigo, produtos.nome, produtos.valor, produtos.deletado);

  fclose(arq);
}

void buscar()
{

  FILE *arq = fopen("produtos.txt", "rb");
  if (arq == NULL)
  {
    printf("Arquivo nao existe ou está vazio.");
    return;
  }

  struct tProduto produtos;
  int cod, encontrado = 0;
  printf("\nDigite o codigo que procura: \n");
  scanf("%d", &cod);

  while (fread(&produtos, sizeof(produtos), 1, arq))
  {
    if ((cod == produtos.codigo) && (produtos.deletado != '*'))
    {
      printf("Cod. %d - Produto: %-8s - Valor R$ %4.2f\n", produtos.codigo, produtos.nome, produtos.valor);
      encontrado = 1;
    }
  }

  if (!encontrado)
    printf("\nCodigo nao cadastrado!!\n");

  fclose(arq);
}

void alterar()
{
  FILE *arq = fopen("produtos.txt", "r+b");
  if (arq == NULL)
  {
    printf("Arquivo nao existe ou está vazio.");
    return;
  }

  struct tProduto produtos;
  int cod, encontrado = 0;
  printf("\nDigite o codigo que deseja alterar: \n");
  scanf("%d", &cod);

  while (fread(&produtos, sizeof(produtos), 1, arq))
  {
    if (cod == produtos.codigo)
    {
      printf("Cod. %d - Produto: %-8s - Valor R$ %4.2f\n", produtos.codigo, produtos.nome, produtos.valor);
      encontrado = 1;

      fseek(arq, sizeof(struct tProduto) * -1, SEEK_CUR);
      printf("\nDigite o novo nome: \n");
      fflush(stdin);
      gets(produtos.nome);
      printf("\nDigite o novo preco: \n");
      scanf("%f", &produtos.valor);

      fwrite(&produtos, sizeof(produtos), 1, arq);
      fseek(arq, sizeof(produtos) * 0, SEEK_END);
      return;
    }
  }

  if (!encontrado)
    printf("\nCodigo do produto nao cadastrado.\n");

  fclose(arq);
}

void excluir()
{

  FILE *arq = fopen("produtos.txt", "r+b");
  if (arq == NULL)
  {
    printf("Arquivo nao existe ou está vazio.");
    return;
  }

  struct tProduto produtos;
  int cod, encontrado = 0;
  char certeza;
  printf("\nDigite o codigo que deseja EXCLUIR: \n");
  scanf("%d", &cod);

  while (fread(&produtos, sizeof(produtos), 1, arq))
  {
    if (cod == produtos.codigo)
    {
      printf("Cod. %d - Produto: %-8s - Valor R$ %4.2f\n", produtos.codigo, produtos.nome, produtos.valor);
      encontrado = 1;

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

  if (!encontrado)
    printf("\nCodigo nao cadastrado!!\n");

  fclose(arq);
}

void organizar()
{
  struct tProduto produtos;
  FILE *arq = fopen("produtos.txt", "rb");
  if (arq == NULL)
  {
    printf("Arquivo nao existe!");
    return;
  }

  FILE *arqB = fopen("backupProdutos.txt", "wb");
  if (arqB == NULL)
  {
    printf("Erro ao abrir arquivo.");
    return;
  }

  while (fread(&produtos, sizeof(produtos), 1, arq))
  {
    if (produtos.deletado != '*')
    {

      fseek(arqB, sizeof(produtos) * -1, SEEK_CUR);
      fwrite(&produtos, sizeof(produtos), 1, arqB);
      fseek(arqB, sizeof(produtos) * 0, SEEK_END);
    }
  }

  fclose(arq);
  fclose(arqB);

  remove("produtos.txt");
  int teste = 0;
  teste = rename("backupProdutos.txt", "produtos.txt");
  if (teste == 0)
  {
    printf("\nArquivo renomeado.\n");
    printf("\nArquivo organizado. %d \n", teste);
  }
  else
    printf("\nErro ao renomear arquivo. %d \n", teste);
  return;
}