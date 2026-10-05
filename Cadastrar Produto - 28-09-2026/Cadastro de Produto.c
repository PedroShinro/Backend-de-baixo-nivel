#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Estrutura do produto */
typedef struct{
    int codigo;
    char nome[100];
    int qtd;
    float preco;
}Produto;

/* Identificadores dos campos */
#define ID_CODIGO 101
#define ID_NOME 102
#define ID_QTD 103
#define ID_PRECO 104

/*Identificadores dos botões */
#define ID_CADASTRAR 201
#define ID_LIMPAR 202
#define ID_LISTAR 203
#define ID_SAIR 204

/* Campos da interface */
HWND campoCodigo;
HWND campoNome;
HWND campoQtd;
HWND campoPreco;

/* FUNÇÃO PARA LIMPAR OS CAMPOS */
void limparCampos(){
    SetWindowText(campoCodigo, "");
    SetWindowText(campoNome, "");
    SetWindowText(campoQtd, "");
    SetWindowText(campoPreco, "");
    SetFocus(campoCodigo);
}

/* FUNÇÃO PARA CADASTRAR PRODUTO */
void cadastrarProduto(HWND janela){
    Produto p;
    char textoCodigo[20], textoQtd[20], textoPreco[30];
    
    /* Pegando os valores digitados */
    GetWindowText(campoCodigo, textoCodigo, sizeof(textoCodigo));
    GetWindowText(campoNome, p.nome, sizeof(p.nome));
    GetWindowText(campoQtd, textoQtd, sizeof(textoQtd));
    GetWindowText(campoPreco, textoPreco, sizeof(textoPreco));
    
    /* Verificando campos vazios */
    if (strlen(textoCodigo) == 0 || strlen(p.nome) == 0 || strlen(textoQtd) == 0 || strlen(textoPreco) == 0){
        MessageBox(janela, "Preencha todos os campos!", "Aviso", MB_OK | MB_ICONWARNING);
        
        return;
    }
    
    /* Convertendo texto para número */
    p.codigo = atoi(textoCodigo);
    p.qtd = atoi(textoQtd);
    p.preco = atof(textoPreco);
    
    /* Abrindo o arquivo */
    FILE*arquivo;
    arquivo = fopen("produto_interface.txt", "a");
    
    if(arquivo == NULL){
        MessageBox(janela, "Erro ao abrir o arquivo.", "Erro", MB_OK | MB_ICONWARNING);
        
        return;
    }
    
    /* Gravando no arquivo */
    fprintf(arquivo, "%d;%s;%d;%.2f\n", p.codigo, p.nome, p.qtd, p.preco);
    fclose(arquivo);
    
    MessageBox(janela, "Produto cadastrado com sucesso!", "Cadastro", MB_OK | MB_ICONINFORMATION);
    limparCampos();
}

/* FUNÇÃO PARA LISTAR PRODUTOS */
void listarProdutos(HWND janela){
    FILE *arquivo;
    Produto p;
    char linha[300];
    char resultado[500] = "";
    arquivo = fopen("produto_interface.txt", "r");
    
    if(arquivo == NULL){
        MessageBox(janela, "Nenhum produto cadastrado", "Produtos", MB_OK | MB_ICONINFORMATION);
        return;
    }
    
    strcat(resultado, "PRODUTOS CADASTRADOS\n\n");
    while(fscanf(arquivo, "%d;%99[^;];%d;%f\n", &p.codigo, p.nome, &p.qtd, &p.preco) == 4){
        sprintf(linha, 
            "Codigo: %d\n"
            "Nome: %s\n"
            "Quantidade: %d\n"
            "Preco R$ %.2f\n"
            "--------------------\n",
            p.codigo, p.nome, p.qtd, p.preco);
        
        strcat(resultado, linha);
    }
    
    fclose(arquivo);
    
    MessageBox(janela, resultado, "Lista de Produtos", MB_OK | MB_ICONINFORMATION);
    
}

/* FUNÇÃO QUE CONTROLA A JANELA */
LRESULT CALLBACK WindowProcedure(HWND janela, UINT mensagem, WPARAM wParam, LPARAM lParam){
    
    switch (mensagem){
        case WM_CREATE:
            
            /* TITULO */
            CreateWindow("STATIC", "SISTEMA DE CADASTRO DE PRODUTOS", WS_VISIBLE | WS_CHILD, 75, 20, 300, 30, janela, NULL, NULL, NULL);
            
            /* CODIGO */
            CreateWindow("STATIC", "Codigo: ", WS_VISIBLE | WS_CHILD, 30, 70, 100, 25, janela, NULL, NULL, NULL);
            campoCodigo = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 140, 70, 220, 25, janela, (HMENU) ID_CODIGO, NULL, NULL);
            
            /* NOME */
            CreateWindow("STATIC", "Nome: ", WS_VISIBLE | WS_CHILD, 30, 110, 100, 25, janela, NULL, NULL, NULL);
            campoNome = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 110, 220, 25, janela, (HMENU) ID_NOME, NULL, NULL);
            
            /* QUANTIDADE */
            CreateWindow("STATIC", "Quantidade: ", WS_VISIBLE | WS_CHILD, 30, 150, 100, 25, janela, NULL, NULL, NULL);
            campoQtd = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 140, 150, 220, 25, janela, (HMENU) ID_QTD, NULL, NULL);
            
            /* Preço */
            CreateWindow("STATIC", "Preco: ", WS_VISIBLE | WS_CHILD, 30, 190, 100, 25, janela, NULL, NULL, NULL);
            campoPreco = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 190, 220, 25, janela, (HMENU) ID_PRECO, NULL, NULL);
            
            /* BOTÕES */
            CreateWindow("BUTTON", "Cadastrar", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 30, 250, 100, 35, janela, (HMENU) ID_CADASTRAR, NULL, NULL);
            
            CreateWindow("BUTTON", "Listar", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 250, 250, 100, 35, janela, (HMENU) ID_LISTAR, NULL, NULL);
            
            /* BOTÕES LIMPAR E SAIR */
            CreateWindow("BUTTON", "Limpar", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 140, 250, 100, 35, janela, (HMENU) ID_LIMPAR, NULL, NULL);
            
            CreateWindow("BUTTON", "Sair", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 360, 250, 80, 35, janela, (HMENU) ID_SAIR, NULL, NULL);
            
            break;
        
        /* EVENTOS DOS BOTÕES */
        case WM_COMMAND:
            switch (LOWORD(wParam)){
                case ID_CADASTRAR:
                    cadastrarProduto(janela);
                    break;
                
                case ID_LIMPAR:
                    limparCampos();
                    break;
                    
                case ID_LISTAR:
                    listarProdutos(janela);
                    break;
                    
                case ID_SAIR:
                    DestroyWindow(janela);
                    break;
            }
            break;
        
        /* FECHAR JANELA */
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        
        default:
            return DefWindowProc(janela, mensagem, wParam, lParam);
    }
    return 0;
}

/* FUNÇÃO PRINCIPAL */
int WINAPI WinMain(HINSTANCE hInstancia, HINSTANCE hInstanciaAnterior, LPSTR argumentos, int exibirJanela){
    WNDCLASS wc = {0};
    
    /* Configurando a janela */
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstancia;
    wc.lpszClassName = "JanelaCadastroProdutos";
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    /* Registrando a janela */
    if(!RegisterClass(&wc)){
        MessageBox(NULL, "Erro ao registrar a janela.", "Erro", MB_OK);
        return 0;
    }
    
    /* Criando a janela principal */
    HWND janela = CreateWindow("JanelaCadastroProdutos", "Cadastro de Produtos", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 500, 360, NULL, NULL, hInstancia, NULL);
    
    if(janela == NULL){
        MessageBox(NULL, "Erro ao criar a janela.", "Erro", MB_OK);
        
        return 0;
    }
    
    /* Exibindo a janela */
    ShowWindow(janela, exibirJanela);
    UpdateWindow(janela);
    
    /* Loop principal */
    MSG mensagem;
    
    while(GetMessage(&mensagem, NULL, 0, 0)){
        TranslateMessage(&mensagem);
        DispatchMessage(&mensagem);
    }
    return 0;
}






































