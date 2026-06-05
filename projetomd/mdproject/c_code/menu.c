#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Implementar as funções para o menu de encriptação e desencriptação, utilizando as funções auxiliares implementadas em menu_example.c
 * As funções a serem implementadas são:
 * - primo
 * - mdc
 * - gerarChavePub
 * - mod_pow
 * - encotrarD
 */

/* ==============================================================
   ==================== MANIPULADORAS DE ARQUIVOS ===============
   ==============================================================*/


/**
 * Dado n e e, cria um arquivo de texto onde a chave é salva
 * função já implementada pelo monitor, não é necessário alterar
 * @param n: o valor de n da chave pública
 * @param e: o valor de e da chave pública
 */
void criarChavePub(long long n, long long e)
{
    FILE *file;

    file = fopen("chavePub.txt", "w");
    fprintf(file, "%lld %lld", n, e);
    fclose(file);
    return;
}
/**
 * Salva a mensagem encriptada em um arquivo de texto
 * função já implementada pelo monitor, não é necessário alterar
 * @param mensagemencriptada: array com a mensagem encriptada
 * @param tamanho: tamanho do array mensagemencriptada
 * @return 0 se a mensagem foi salva com sucesso, 1 caso contrário
*/
int salvarEmArquivo(long long mensagemencriptada[], int tamanho)
{
    FILE *file;
    file = fopen("textEncript.txt", "w");
    if (file == NULL){
        return 1;
    }

    for (int i = 0; i < tamanho; i++){
        fprintf(file, "%lld ", mensagemencriptada[i]);
    }

    fclose(file);
    return 0;
}

/**
 * Salva a mensagem desencriptada em um arquivo de texto
 * função já implementada pelo monitor, não é necessário alterar
 * @param mensagemdesencriptada: array com a mensagem desencriptada
 * @param tamanho: tamanho do array mensagemdesencriptada
 * @return 0 se a mensagem foi salva com sucesso, 1 caso contrário
 */
int salvarEmArquivoD(char mensagemdesencriptada[], int tamanho)
{
    FILE *file;
    file = fopen("textDencript.txt", "w");

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    for (int i = 0; i < tamanho; i++)
    {
        if(mensagemdesencriptada[i] != ' ') fprintf(file, "%c", mensagemdesencriptada[i]);
        else fprintf(file," ");
    }

    fclose(file);
    return 0;
}


/* ==============================================================
   ==================== FUNÇÕES BASE ============================
   ==============================================================*/
// Essas a gente que implementa

int primo(long long n)
{
    //Função para verificar se um número é primo
    //Retorna 1 para verdadeiro e 0 para falso
    for (int i = 2; i != n; i++)
    {
        if(n % i == 0) return 0;
    }
    return 1;
}

int mdc(long long n1, long long n2)
{
    //Implementar função para calcular o máximo divisor comum
}

int gerarChavePub(long long primo1, long long primo2, long long expoente){
    long long n; // variabel para armazenar o valor de n

    //Implementar lógica para ler a chave pública
    //Lembrar de não aceitar valores pequenos de tal forma que p*q < 256
    criarChavePub(n, expoente);
    return 0;
}

long long mod_pow(long long base, long long exponent, long long modulus)
{
    //implementar função de exponenciação modular
}


long long encotrarD(long long e, long long p, long long q)
{
    // D é o inverso de e mod (p-1)(q-1)
    // e já é primo em relação a (p-1)(q-1), então o mdc entre eles é 1
    long long m = (p-1)*(q-1);

    /**  usamos euclides estendido para encontrar o inverso de e mod m
     * - para cada linha vale a relação r=s*e+t*m
     * - para avançar para a proxima linha precisamos das duas linhas anteriores
     *    - q = r0/r1 -> divisão inteira
     *    - r2 = r0 - q*r1; // s2 = s0 - q*s1 // t2 = t0 - q*t1
     */

    long long r0 = e, s0 = 1, t0 = 0; // valores da primeira linha -> satisfaz e=s0*e+t0*m
    long long r1 = m, s1 = 0, t1 = 1; // valores da segunda linha  -> satisfaz m=s1*e+t1*m

    while(r1!=0) // acaba quando chegamos a 0 como resto
    {
        long long q = (r0 / r1);    // a divisão já é inteira
        long long r2 = r0 - q * r1; // proxima linha do algoritimo de euclides
        long long s2 = s0 - q * s1; // "s" da linha atual
        long long t2 = t0 - q * t1; // "t" da linha atual

        // avançamos para a proxima linha
        r0 = r1; s0 = s1; t0 = t1; r1 = r2; s1 = s2; t1 = t2;
    }
    // ao chegar em r1=0, a linha anterior é a que satisfaz a equação mdc(e,m)=s*e+t*m
    // então r0 = mdc(e,m), mas se o mdc(e,m) não for 1, não existe inverso
    if(r0!=1)
    {
        return -1; // sinalizamos erro, mdc(e,[p-1]*[q-1]) não pode ser diferente de 1
    }

    // caso não surja um erro,  o "s" que queremos será, nesse ponto, o s0 e ele é o inverso de e mod m
    long long d = ((s0 % m) + m ) % m; // ajustamos para que d seja positivo menor que m

    return d; // retornamos o "d" achado
}

/* ==============================================================
   == CRIPTOGRAFAR E DESCRIPTOGRAFAR, LIDAR COM LETRAS ==========
   ==============================================================*/

/**
 * Encripta a mensagem utilizando a chave pública (n, e)
 * função já implementada pelo monitor, não é necessário alterar
 * @param mensagem: a mensagem a ser encriptada
 * @param mensagemencriptada: array onde a mensagem encriptada será armazenada
 * @param n: o valor de n da chave pública
 * @param e: o valor de e da chave pública
 */
void encriptar(char *mensagem, long long mensagemencriptada[], long long n, long long e)
{
    for (int i = 0; mensagem[i] != '\0'; i++){
        mensagemencriptada[i] = mensagem[i];
        mensagemencriptada[i] = mod_pow(mensagemencriptada[i], e, n);
    }
}

/**
 * Descriptografa a mensagem utilizando a chave privada (d, n)
 * função já implementada pelo monitor, não é necessário alterar
 * @param mensagemencriptada: array com a mensagem encriptada
 * @param tamanho: tamanho do array mensagemencriptada
 * @param d: o valor de d da chave privada
 * @param n: o valor de n da chave privada
 */
void descriptografar(long long mensagemencriptada[], int tamanho, long long d, long long n)
{
    for (int i = 0; i < tamanho; i++)
    {
        mensagemencriptada[i] = mod_pow(mensagemencriptada[i], d, n);
    }
}

/**
 * Converte a mensagem encriptada para ASCII
 * função já implementada pelo monitor, não é necessário alterar
 * @param mensagem: array onde a mensagem ASCII será armazenada
 * @param mensagemencriptada: array com a mensagem encriptada
 * @param tamanho: tamanho dos arrays
 */
void convertascii(char mensagem[], long long mensagemencriptada[], int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        mensagem[i] = (char)mensagemencriptada[i];
    }
}

/**
 * Converte uma string de números separados por espaços para um array de long long
 * função já implementada pelo monitor, não é necessário alterar
 * @param str: a string a ser convertida
 * @param a: o array onde os números serão armazenados
 * @param tamanho: o tamanho do array
 */
void converterParaLongLong(char *str, long long a[], long long *tamanho)
{
    char *token = strtok(str, " ");
    long long i = 0;
    while (token != NULL)
    {
        a[i] = atoll(token);
        i++;
        token = strtok(NULL, " ");
    }
    *tamanho = i;
}


/* ==============================================================
   ============== COMUNICAÇÃO COM O SERVIDOR ====================
   ==============================================================*/

/**
 * limpa o buffer de entrada para evitar problemas na comunicação com o servidor
 * função já implementada pelo monitor, não é necessário alterar
*/
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * função chamada pelo servidor para encriptar a mensagem utilizando a chave pública (n, e)
 * função já implementada pelo monitor, não é necessário alterar
 * @param mensagem: a mensagem a ser encriptada
 * @param n: o valor de n da chave pública
 * @param e: o valor de e da chave pública
 */
int encriptarMenu(char* mensagem, long long n, long long e)
{
    int tamanho = (int)strlen(mensagem);
    long long mensagemencriptada[tamanho];
    encriptar(mensagem, mensagemencriptada, n, e);
    salvarEmArquivo(mensagemencriptada, tamanho);
    return 0;
}

/**
 * função chamada pelo servidor para desencriptar a mensagem utilizando a chave privada (d, n)
 * função já implementada pelo monitor, não é necessário alterar
 * @param p: um dos números primos utilizados na geração das chaves
 * @param q: o outro número primo utilizados na geração das chaves
 * @param e: o valor de e da chave pública
 */
int desencriptarMenu(long long p, long long q, int e){
    long long mensagemenc[100000], D, tamanho;
    char mensagem[10000];
    char linha[10000];
    int i = 0;

    FILE *file;
    file = fopen("textEncript.txt", "r");
    

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    fgets(mensagem, sizeof(mensagem), file);
    if (mensagem[strlen(mensagem) - 1] == '\n'){
        mensagem[strlen(mensagem) - 1] = '\0';
    }

    fclose(file);

    converterParaLongLong(mensagem, mensagemenc, &tamanho);
    
    long long n = p * q;

    D = encotrarD(e, p, q);
    descriptografar(mensagemenc, tamanho, D, n);
    convertascii(mensagem, mensagemenc, tamanho);
    salvarEmArquivoD(mensagem, tamanho);
    return 0;
}