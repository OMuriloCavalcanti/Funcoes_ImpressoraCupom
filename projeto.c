#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ======================= Config DLL ======================= */
static HMODULE g_hDll = NULL;

/* Convenção de chamada (Windows): __stdcall */
#ifndef CALLCONV
#  define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
typedef int (CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (CALLCONV *FechaConexaoImpressora_t)(void);
typedef int (CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int (CALLCONV *Corte_t)(int);
typedef int (CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int (CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (CALLCONV *AvancaPapel_t)(int);
typedef int (CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int (CALLCONV *AbreGaveta_t)(int, int, int);
typedef int (CALLCONV *SinalSonoro_t)(int, int, int);
typedef int (CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int (CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (CALLCONV *InicializaImpressora_t)(void);

/* ======================= Ponteiros ======================= */
static AbreConexaoImpressora_t        AbreConexaoImpressora        = NULL;
static FechaConexaoImpressora_t       FechaConexaoImpressora       = NULL;
static ImpressaoTexto_t               ImpressaoTexto               = NULL;
static Corte_t                        Corte                        = NULL;
static ImpressaoQRCode_t              ImpressaoQRCode              = NULL;
static ImpressaoCodigoBarras_t        ImpressaoCodigoBarras        = NULL;
static AvancaPapel_t                  AvancaPapel                  = NULL;
static AbreGavetaElgin_t              AbreGavetaElgin              = NULL;
static AbreGaveta_t                   AbreGaveta                   = NULL;
static SinalSonoro_t                  SinalSonoro                  = NULL;
static ImprimeXMLSAT_t                ImprimeXMLSAT                = NULL;
static ImprimeXMLCancelamentoSAT_t    ImprimeXMLCancelamentoSAT    = NULL;
static InicializaImpressora_t         InicializaImpressora         = NULL;

/* ======================= Configuracao ======================= */
static int   g_tipo      = 1;
static char  g_modelo[64] = "i9";
static char  g_conexao[128] = "USB";
static int   g_parametro = 0;
static int   g_conectada = 0;

/* ======================= Utilidades ======================= */
#define LOAD_FN(h, name)                                                         \
    do {                                                                         \
        name = (name##_t)GetProcAddress((HMODULE)(h), #name);                    \
        if (!(name)) {                                                           \
            fprintf(stderr, "Falha ao resolver símbolo %s (erro=%lu)\n",         \
                    #name, GetLastError());                                      \
            return 0;                                                            \
        }                                                                        \
    } while (0)

static void flush_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ======================= Funcoes para manipular a DLL ======================= */
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll) {
        fprintf(stderr, "Erro ao carregar E1_Impressora01.dll (erro=%lu)\n", GetLastError());
        return 0;
    }

    LOAD_FN(g_hDll, AbreConexaoImpressora);
    LOAD_FN(g_hDll, FechaConexaoImpressora);
    LOAD_FN(g_hDll, ImpressaoTexto);
    LOAD_FN(g_hDll, Corte);
    LOAD_FN(g_hDll, ImpressaoQRCode);
    LOAD_FN(g_hDll, ImpressaoCodigoBarras);
    LOAD_FN(g_hDll, AvancaPapel);
    LOAD_FN(g_hDll, AbreGavetaElgin);
    LOAD_FN(g_hDll, AbreGaveta);
    LOAD_FN(g_hDll, SinalSonoro);
    LOAD_FN(g_hDll, ImprimeXMLSAT);
    LOAD_FN(g_hDll, ImprimeXMLCancelamentoSAT);
    LOAD_FN(g_hDll, InicializaImpressora);

    return 1;
}

static void liberarBiblioteca(void)
{
    if (g_hDll) {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ======================= Funcoes a serem implementadas pelos alunos ======================= */

//printa as opcoes do menu 0 a 9,exibindo todas de maneira elegante inclusive ;)
static void exibirMenu(void)
{
    printf("MENU PRINCIPAL \n");
    printf("1 - Configurar Conexao\n");
    printf("2 - Abrir Conexao\n");
    printf("3 - Imprimir Texto\n");
    printf("4 - Imprimir QRCode\n");
    printf("5 - Imprimir Codigo de Barras\n");
    printf("6 - Imprimir XML SAT\n");
    printf("7 - Imprimir XML Cancelamento SAT\n");
    printf("8 - Abrir Gaveta Elgin\n");
    printf("9 - Abrir Gaveta\n");
    printf("10 - Sinal Sonoro\n");
    printf("0 - Fechar Conexao e Sair\n");
}

//realiza a configuracao da conexao com a impressora, usando os parametros g_tipo, g_modelo e g_conexao
// todos usam a funcao fush_entrada() para enviar a informacao
static void configurarConexao(void)
{
    printf("Configurar Conexao:\n");
    printf("Tipo (1-USB / 2-ETHERNET): ");
    scanf("%d", &g_tipo); flush_entrada();
    
    printf("Modelo (ex: i9): ");
    scanf("%63s", g_modelo); flush_entrada();
    
    printf("Conexao (ex: USB ou IP da impressora): ");
    scanf("%127s", g_conexao); flush_entrada();
    
    printf("Parametro (geralmente 0): ");
    scanf("%d", &g_parametro); flush_entrada();

    printf("Configuracao salva!\n");
}

//verifica se esta conectada 
//usa os parametros da funcao  configurarConexao para  iniciar a conexao  ;)
static void abrirConexao(void)
{
    if(g_conectada) {
        printf("Impressora ja conectada!\n");
        return;
    }

    if(AbreConexaoImpressora(g_tipo, g_conexao, g_modelo, g_parametro) == 0) {
        g_conectada = 1;
        printf("Impressora conectada com sucesso\n");
    }
	else {
        printf("Falha ao abrir conexao\n");
    }
}

//fecha a conexao se ela estiver ativa e caso nao esteja, resulta em erro
static void fecharConexao(void)
{
   if (!g_conectada) {
        printf("Nenhuma conexao ativa\n");
        return;
    }
    FechaConexaoImpressora();
    g_conectada = 0;
    printf("Conexao encerrada\n");
}

//imprime um texto no papel usando preto e branco para ficar mais visivel
//usa fgets para pegar o texto digitado
//manda os parametros para ImpressaoTexto 
//chama avanca papel
//chama corte
static void imprimirTexto(void)
{
    char texto[255];
    printf("Digite o texto para imprimir: \n");
    fgets(texto, sizeof(texto), stdin);

    ImpressaoTexto(texto, 1, 4, 0);
    AvancaPapel(4);
    Corte(1);

}

//imprime um texto convertido em qr code
//pega o texto digitado com um fgets
//passa os parametros em ImpressaoQRCode
//avanca e corte do papel 

static void imprimirQRCode(void)
{
    char conteudo[255];
    printf("Conteudo do QRCode: ");
    fgets(conteudo, sizeof(conteudo), stdin);

    ImpressaoQRCode(conteudo, 6, 4);
    AvancaPapel(4);
    Corte(1);
}

//imprime um codigo de barras com as informacoes pre-definidas
//parametros direto em ImprimirCodigoBarras
//avanca e corta papel
static void imprimirCodigoBarras(void)
{
    ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);
    AvancaPapel(4);
    Corte(1);
    
}

//imprime o XMLSAT
//valida se encontrou o arquivo
static void imprimirXMLSAT(void)
{
	FILE *arq = fopen("XMLSAT.xml", "r");
    if (!arq) {
        printf("Arquivo XMLSAT.xml não encontrado!\n");
        return;
    }
    
    char xml[10000];
    fread(xml, 1, sizeof(xml), arq);
    fclose(arq);

    ImprimeXMLSAT(xml, 0);
    AvancaPapel(2);
    Corte(1);

}

//usa a chave
static void imprimirXMLCancelamentoSAT(void)
{
	const char *assinatura =
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";

    FILE *arq = fopen("CANC_SAT.xml", "r");
    if (!arq) {
        printf("Arquivo CANC_SAT.xml não encontrado!\n");
        return;
    }
    char xml[10000];
    fread(xml, 1, sizeof(xml), arq);
    fclose(arq);

    ImprimeXMLCancelamentoSAT(xml, assinatura, 0);
    AvancaPapel(2);
    Corte(1);

}

//chama a funcao especifica de abrir gaveta elgin passando os parametros fixos
static void abrirGavetaElginOpc(void)
{
    AbreGavetaElgin(1, 50, 50);
}

//chama a funcao padrao de abrir gaveta com os parametros definidos
static void abrirGavetaOpc(void)
{
    AbreGaveta(1, 5, 10);
}

//aciona o beep da impressora enviando a quantidade de beeps e o tempo
static void emitirSinalSonoro(void)
{
	SinalSonoro(4, 50, 5);
}

/* ======================= Funcao principal ======================= */

//funcao main que carrega as bibliotecas, roda o loop infinito do menu, le a escolha do usuario, limpa a tela e trata o switch
int main(void)
{
    if (!carregarFuncoes()) {
        return 1;
    }

    int opcao = 0;
    while (1) {
        
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        flush_entrada();
        system("cls");

        switch (opcao) {
	        case 1: 
			configurarConexao(); 
			break;
			
	        case 2: 
			abrirConexao(); 
			break;
			
	        case 3: 
			imprimirTexto(); 
			break;
			
	        case 4: 
			imprimirQRCode(); 
			break;
			
	        case 5: 
			imprimirCodigoBarras(); 
			break;
			
	        case 6: 
			imprimirXMLSAT();
			break;
			
	        case 7: 
			imprimirXMLCancelamentoSAT();
			break;
			
	        case 8: 
			abrirGavetaElginOpc(); 
			break;
			
	        case 9: 
			abrirGavetaOpc();
			break;
			
			case 10:
			emitirSinalSonoro(); 
			break;
			
	        case 0: 
			fecharConexao(); 
			liberarBiblioteca(); // boa pratica liberar a dll ao sair ;)
            return 0; 
			
	        default: printf("Opcao invalida!\n"); 
			break;
    	}
        
    }
}
