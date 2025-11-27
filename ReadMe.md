# Sistema de Controle de Impressora Elgin

Sistema em C para controle e gerenciamento de impressoras Elgin através da DLL `E1_Impressora01.dll`. Desenvolvido para Windows, oferece interface de linha de comando para operações de impressão, incluindo texto, QR Code, código de barras e documentos fiscais SAT.

## Requisitos

- Windows (compilado para Win32/Win64)
- DLL `E1_Impressora01.dll` (fornecida pela Elgin)
- Compilador C compatível (GCC, MinGW ou MSVC)
- Impressora Elgin compatível (ex: i9)

## Funcionalidades

- ✅ Configuração de conexão (USB/Ethernet)
- ✅ Impressão de texto formatado
- ✅ Geração de QR Code
- ✅ Impressão de código de barras
- ✅ Impressão de XML SAT e Cancelamento SAT
- ✅ Controle de gaveta (padrão e Elgin)
- ✅ Sinal sonoro (beep)
- ✅ Controle de corte e avanço de papel

## Uso

Execute o programa e navegue pelo menu interativo:

```
MENU PRINCIPAL
1 - Configurar Conexao
2 - Abrir Conexao
3 - Imprimir Texto
4 - Imprimir QRCode
5 - Imprimir Codigo de Barras
6 - Imprimir XML SAT
7 - Imprimir XML Cancelamento SAT
8 - Abrir Gaveta Elgin
9 - Abrir Gaveta
10 - Sinal Sonoro
0 - Fechar Conexao e Sair
```

### Fluxo Básico

1. **Configurar Conexão** (Opção 1)
   - Tipo: 1 (USB) ou 2 (Ethernet)
   - Modelo: ex: `i9`
   - Conexão: `USB` ou IP da impressora
   - Parâmetro: geralmente `0`

2. **Abrir Conexão** (Opção 2)
   - Estabelece comunicação com a impressora

3. **Realizar Operações** (Opções 3-10)
   - Execute as operações desejadas

4. **Fechar Conexão** (Opção 0)
   - Encerra a conexão e libera recursos

## Exemplos de Uso

### Impressão de Texto Simples

```
Escolha: 3
Digite o texto para imprimir:
Olá, mundo!
[Texto será impresso, papel avançado e cortado]
```

### Impressão de QR Code

```
Escolha: 4
Conteudo do QRCode: https://github.com
[QR Code será gerado e impresso]
```

### Impressão de Código de Barras

```
Escolha: 5
[Código de barras padrão será impresso: {A012345678912]
```

## Configurações Padrões

| Parâmetro | Valor Padrão |
|-----------|--------------|
| Tipo de Conexão | USB (1) |
| Modelo | i9 |
| Conexão | USB |
| Parâmetro | 0 |

## Funções da DLL

O sistema utiliza as seguintes funções da DLL Elgin:

- `AbreConexaoImpressora` - Estabelece conexão
- `FechaConexaoImpressora` - Encerra conexão
- `ImpressaoTexto` - Imprime texto formatado
- `ImpressaoQRCode` - Gera QR Code
- `ImpressaoCodigoBarras` - Imprime código de barras
- `ImprimeXMLSAT` - Imprime cupom SAT
- `ImprimeXMLCancelamentoSAT` - Imprime cancelamento SAT
- `AbreGavetaElgin` - Abre gaveta (método Elgin)
- `AbreGaveta` - Abre gaveta (método padrão)
- `SinalSonoro` - Emite beeps
- `AvancaPapel` - Avança papel
- `Corte` - Corta papel

## Parâmetros de Impressão

### Texto
- Alinhamento: Esquerda
- Tamanho: 4 (médio)
- Estilo: Negrito

### QR Code
- Tamanho: 6
- Margem: 4

### Código de Barras
- Tipo: 8 (CODE128)
- Altura: 100
- Largura: 2
- HRI: 3 (abaixo)

## Tratamento de Erros

O sistema verifica:
- Carregamento da DLL
- Resolução de símbolos/funções
- Estado de conexão antes de operações
- Existência de arquivos XML

## Limitações Conhecidas

- Requer DLL proprietária da Elgin
- Apenas Windows
- Arquivos XML devem estar no mesmo diretório do executável
- Tamanho máximo de texto: 255 caracteres
- Tamanho máximo de XML: 10.000 caracteres

## Troubleshooting

### "Erro ao carregar E1_Impressora01.dll"
- Verifique se a DLL está no mesmo diretório do executável
- Verifique se a versão da DLL é compatível (32/64 bits)

### "Falha ao abrir conexao"
- Verifique se a impressora está ligada e conectada
- Confirme se o tipo de conexão está correto
- Para Ethernet, verifique o IP da impressora

### "Arquivo XMLSAT.xml não encontrado"
- Coloque o arquivo XML no mesmo diretório do executável

## Licença

Este código é fornecido para fins educacionais e de desenvolvimento.

## Suporte

Para questões relacionadas à DLL ou impressoras Elgin, consulte:

## Autores
- Eduardo Marcondes Cassanha Ferreira Peralta
- Guilherme da Silva Cordeiro
- Julia Viegas dos Santos
- Murilo Cavalcanti dos Santos
- Murilo Pysklewitz Fuzikawa

- [Site Oficial da documentação] (https://elgindevelopercommunity.github.io/modules.html)
- Documentação da DLL E1_Impressora01

---

**Nota**: Este é um projeto educacional. Para uso em produção, implemente validações adicionais, tratamento robusto de erros e logs de operação.
