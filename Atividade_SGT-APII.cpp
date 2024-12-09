/*
    Sistema de Gerenciamento de Tarefas - Atividade
    IFPR - TB | Algoritmos e Programação II
    Autor: Ricardo Hideki Tsugikava Junior
    Prof. Berssanete
*/

#include <iostream> //Biblioteca para entrada e saída de dados
#include <ctime> //Biblioteca para manipulação de tempo
#include <locale.h> //Biblioteca para linguagem
#include <string> //Biblioteca para manipulação de cadeia de caracteres
#include <vector> //Biblioteca para manipulação de vetores
#include <memory> //Biblioteca para manipulação de memória
#include <algorithm> //Biblioteca para manipulação de dados (em especial vetores)
#include <fstream> //Biblioteca para leitura e escrita de arquivos

#pragma warning(disable : 4996) //Desabilita um problema que envolve o uso do ctime ao pegar a data exata do computador.

using namespace std; //Adiciona o std como padrão no início de cada comando

time_t timestamp = time(&timestamp); //Cria uma varíavel do ctime
struct tm datetime = *localtime(&timestamp); //Cria um struct com todas as informações sobre o tempo
string arquivoSalvoNome = "arquivoTarefas.bin"; //Constante para definir o nome do arquivo para ser salvo

struct Tarefa { //Struct sobre tarefas;
    int codigo; 
    string nome; 
    string descricao;
    string dataCriacao;
    int status;           // Escolha do int ao armazenar para ficar mais simples a manipulação dos dados por parte do usuário
    int prioridade;       //

    void exibirTarefa() const { //Função para exibir os itens do struct, otimizando o processo ao precisar mostrar esses valores
        string prioridadeString = ""; //String temporaria para armazenar o atual estado da prioridade (pois se trata de um int e não string)
        string statusString = ""; //      ^ Mesma coisa só que com os status

        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "Código: " << codigo << endl;
        cout << "Nome: " << nome << endl;
        cout << "Descrição: " << descricao << endl;
        cout << "Data de Criação: " << dataCriacao << endl;
        switch (status) { //switch para escrever o status correto na tela
        case 1:
            statusString = "Pendente";
            break;
        case 2:
            statusString = "Em andamento";
            break;
        case 3:
            statusString = "Finalizado";
            break;
        default:
            prioridadeString = "Erro!";
            break;
        }

        cout << "status: " << statusString << endl;

        switch (prioridade) { //switch para escrever a prioridade correta na tela
        case 1: 
            prioridadeString = "Baixa";
            break;
        case 2:
            prioridadeString = "Média";
            break;
        case 3:
            prioridadeString = "Alta";
            break;
        default:
            prioridadeString = "Erro!";
            break;
        }
        cout << "prioridade: " << prioridadeString << endl;
    }
};

string pegarDataAtual() { //Função para pegar a data atual de acordo com o computador do usuário
    string diaAtual, mesAtual, anoAtual, data;

    if (datetime.tm_mday <= 9) { //if para adicionar "0" para números de uma casa decimal;
        diaAtual = "0" + to_string(datetime.tm_mday); //pega o dia e transforma em string e adiciona o 0 na frente caso seja unidade.
    }
    else {
        diaAtual = to_string(datetime.tm_mday); //pega o dia do mês
    }

    mesAtual = to_string(datetime.tm_mon + 1); //pega o mês do ano em um range de 0..11, precisando adicionar +1 para pegar o mês correto.

    anoAtual = to_string(datetime.tm_year + 1900);//a função tm_year retorna a quantidade da anos após 1900, então ao adicionarmos 1900 a esse valor temos o ano atual.

    data = diaAtual + "/" + mesAtual + "/" + anoAtual; //Formata tudo para salvar como string "dia/mes/ano"

    return data; //retorna a string
}

shared_ptr<Tarefa> pesquisarVetor(vector<shared_ptr<Tarefa>>& tarefas, int codigoPes) { //Entra com o vetor de tarefas e o código a ser pesquisado.
    for (const auto& tarefa : tarefas) { //Para cada item do array...
        if (tarefa->codigo == codigoPes) { //Se o tarefa->codigo foi igual ao id pedido pelo usuário
            return tarefa; //retorna a tarefa
        }
    }
    return nullptr; //Caso não haja, retorne null.
}

void imprimirMenu() { //Menu inicial do programa
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
    cout << "Sistema de Gerenciamento de Tarefas\n";
    cout << "Criado por: Deky\n";
    cout << "Dia: ";
    cout << pegarDataAtual() << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
    cout << endl;
    cout << "1. Listar Tarefas\n";
    cout << "2. Cadastrar Tarefas\n";
    cout << "3. Buscar Tarefa por código\n";
    cout << "4. Editar tarefa\n";
    cout << "5. Excluir tarefa\n";
    cout << "6. Ondernar tarefas por código ou prioridade\n";
    cout << "7. Salvar Tarefas\n";
    cout << "8. Carregar Tarefas\n";
    cout << "0. Sair do programa\n";
    cout << endl;
    cout << "Insira uma opção: ";
}

int checarInteiro(string txtEntrada) { //Função para checar se o input se trata de um inteiro
    int numero;

    while (true) { //Enquanto verdade...
        cin >> numero; //Tenta adicionar um input para uma varíavel inteira

        if (!cin.fail()) { //Se não falhar...
            cin.ignore(); //Limpa Buffer
            return numero; //Retorna varíavel
        }

        //Caso haja uma falha ao ler input...
        cout << endl;
        cout << "Insira um valor válido!!\n"; //Avisa o usuário
        cout << endl;
        cin.clear(); // Limpa o estado de erro
        cin.ignore(); //Limpa o buffer

        cout << txtEntrada; //Imprime o texto inicial e começa de novo
    }
}

int checarMinMaxInteiros(string txtEntrada, int min, int max) { //Função para checar se o usuário enviou um inteiro dentro dos parâmetros especificados
    int numero;
    numero = checarInteiro(txtEntrada); //Lê se o número é realmente um intero

    while (true) { //Enquanto verdade...
        if (!(numero < min) && !(numero > max)) { //Se o valor não estiver fora dos parametroes, retorne o número
            return numero;
        }

        //Caso esteja fora...
        cout << endl;
        cout << "Insira um valor válido!!\n"; //Avisa o usuário
        cout << endl;
        cout << txtEntrada; //Imprime o texto inicial
        numero = checarInteiro(txtEntrada); //Pede um novo número inteiro.
    }

    return numero;
}

void listarTarefas(vector<shared_ptr<Tarefa>>& tarefas) { //Passa o vetor de tarefas para listar todas as tarefas

    system("cls"); //Limpa tela

    if (tarefas.empty()) { //Caso não tenha nenhum item no vetor, imprime a mensagem abaixo e encerra a função.
        cout << "Não há tarefas cadastrada até o momento.\n" << endl;
        system("pause");
        return;
    }

    for (const auto& tarefa : tarefas) { //Para cada item em tarefas execute a função "exibirTarefa()".
        tarefa->exibirTarefa();
    }

    cout << endl;
    system("pause"); //Pausa no sistema para o usuário ler as tarefas.
}

void cadastrarTarefa(int posicao, vector<shared_ptr<Tarefa>>& tarefas){ //passa o vetor de tarefas para cadastrar uma nova tarefa

    system("cls"); //limpa a tela

    int codigoTemp;

    auto novaTarefa = make_shared<Tarefa>(); //cria um ponteiro para o vetor de tarefas

    cout << "Insira o código da tarefa (num int): "; //Pede para inserir um código para tarefa
    codigoTemp = checarInteiro("Insira o código da tarefa (num int): ");

    auto encontrarTarefa = pesquisarVetor(tarefas, codigoTemp); //Pesquisa se a tarefa existe

    while (encontrarTarefa) { //Se existir uma tarefa com esse nome
       cout << endl;
       cout << "Este código já pertence a uma tarefa, por favor insira outro!\n" << endl; //Informa o problema ao usuário
       cout << "Insira o código da tarefa (num int): "; //Pede um novo código para o usuário
       codigoTemp = checarInteiro("Insira o código da tarefa (num int): ");
       encontrarTarefa = pesquisarVetor(tarefas, codigoTemp); //Checa novamente se há um código repetido.
    }

    novaTarefa->codigo = codigoTemp; //Adiciona o código ao struct temporário

    cout << endl;

    cout << "Insira o nome da tarefa: "; //Pede para inserir um nome para tarefa
    getline(cin, novaTarefa->nome);

    cout << endl;

    cout << "Escreva uma pequena descrição para a tarefa: "; //Pede para inserir uma descrição para tarefa
    getline(cin, novaTarefa->descricao);

    cout << endl;

    novaTarefa->dataCriacao = pegarDataAtual(); //Pega o dia, mês e ano do computador para adicionar na tarefa

    cout << "1 - pendente\n2 - em andamento\n3 - concluída\n";
    cout << "Insira o status atual da tarefa (num int): "; //Pede para inserir um status entre "pendente", "em andamento" e "concluída" para tarefa.
    novaTarefa->status = checarMinMaxInteiros("Insira o status atual da tarefa (num int): ", 1, 3);

    cout << endl;

    cout << "1 - baixa\n2 - média\n3 - alta\n";
    cout << "Insira a prioridade atual da tarefa (num int): "; //Pede para inserir uma propriedade para a tarefa entre "baixa", "comum" e "alta".
 
    novaTarefa->prioridade = checarMinMaxInteiros("Insira a prioridade atual da tarefa (num int): ", 1, 3);

    cout << endl;

    tarefas.push_back(novaTarefa); //Insere as informações no array de tarefas.

    cout << "Tarefa criada com sucesso!\n" << endl;

    system("pause"); //Pausa o sistema, pedindo para o usuário apertar qualquer botão para prosseguir.

    cout << endl;
    cout << endl;
}

void buscarTarefaID(vector<shared_ptr<Tarefa>>& tarefas) { //Passa o vetor tarefas para pesquisar uma tarefa em específico
    int idPesquisar; //inteiro que guarda o código da tarefa

    system("cls"); //Limpa a tela

    cout << "Insira o código que deseja pesquisar: "; //Pega o código que o usuário deseja.
    idPesquisar = checarInteiro("Insira o código que deseja pesquisar: "); //Checa se o número é inteiro

    cout << endl;

    auto encontrarTarefa = pesquisarVetor(tarefas, idPesquisar); //Pesquisa se a tarefa existe

    if (encontrarTarefa) { //Caso exista
        encontrarTarefa->exibirTarefa(); //Pesquisa a tarefa
    } else {
        cout << "Nenhuma tarefa foi encontrada com este código!\n";
    }
      
    cout << endl;
    system("pause"); //Pausa no sistema para o usuário ler as tarefas.
}

void editarTarefa(vector<shared_ptr<Tarefa>>& tarefas) { //Passa o vetor para editar uma tarefa
    int idPesquisar;
    string escolha;
    
    system("cls"); //Limpa a tela

    cout << "Insira o código da tarefa que deseja editar: "; //Pega o código que o usuário deseja.
    idPesquisar = checarInteiro("Insira o código da tarefa que deseja editar: ");

    cout << endl;

    auto encontrarTarefa = pesquisarVetor(tarefas, idPesquisar); //Pesquisa se a tarefa existe

    if (encontrarTarefa) {

        string structNomes[6] = { "código", "nome", "descrição", "data de criação", "status", "prioridade" }; //Array de nomes do struct para otimização

        encontrarTarefa->exibirTarefa();//exibe a tarefa para o usuário para ajudá-lo na alteração.
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

        cout << endl;

        for (int i = 0; i < 6; i++) { //i = 1, se i for menor que 6, aumenta em 1. Foi utilizado para otimizar o processo de pegar todas as alterações.
            cout << "Deseja alterar o/a " << structNomes[i] << "? (\"S\" - Sim | \"N\" - Não): ";
            cin >> escolha;
            cin.ignore();
            if (escolha == "s" || escolha == "S") {
                cout << "Insira o/a novo(a) " << structNomes[i] << ": ";
                switch (i) { //Switch para selecionar o valor que o usuário deseja alterar de acordo com o valor escolhido.
                case 0: //Para valores que aceitam int ele checa se é um int
                    encontrarTarefa->codigo = checarInteiro("Insira o/a novo(a) " + structNomes[i] + ": ");
                    break;
                case 1://Para valores que são string eles apenas adicionam
                    getline(cin, encontrarTarefa->nome);
                    break;
                case 2:
                    getline(cin, encontrarTarefa->descricao);
                    break;
                case 3:
                    getline(cin, encontrarTarefa->dataCriacao);
                    break;
                case 4://Para valores pré-definios ele checa se é um int e está dentro dos parâmetros
                    encontrarTarefa->status = checarMinMaxInteiros("Insira o/a novo(a) " + structNomes[i] + ": ", 1, 3);
                    break;
                case 5:
                    encontrarTarefa->prioridade = checarMinMaxInteiros("Insira o/a novo(a) " + structNomes[i] + ": ", 1, 3);
                    break;
                default:
                    break;
                }
            }

            cout << endl;
        }


    }
    else {
        cout << "Nenhuma tarefa foi encontrada com este código!\n";
    }    

    cout << endl;

    cout << "Edição realizada!\n";

    system("pause");
}

void excluirTarefa(vector<shared_ptr<Tarefa>>& tarefas) { //Passa o vetor para excluir uma tarefa

    int codigoEscolhido;
    string confirmacao;

    system("cls");

    cout << "Insira o código da tarefa que deve ser excluída: "; //Pedo o código da tarefa que o usuário deseja excluir
    codigoEscolhido = checarInteiro("Insira o código da tarefa que deve ser excluída: ");

    auto encontrarTarefa = pesquisarVetor(tarefas, codigoEscolhido); //Pesquisa se a tarefa existe

    if (encontrarTarefa) { //Se a tarefa existir...
        encontrarTarefa->exibirTarefa(); //Exibe a tarefa.
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

        cout << endl;

        cout << "Deseja mesmo excluir essa tarefa? (S - Sim | N - Não): "; //Pergunta ao usuário se ele deseja realmente excluir está tarefa.
        getline(cin, confirmacao);

        if (confirmacao == "s" || confirmacao == "S") { //Caso sim
            tarefas.erase(find(tarefas.begin(), tarefas.end(), encontrarTarefa)); //Procura e apagar a tarefa do vetor de tarefas.
        }

        cout << "Tarefa excluída com sucesso!\n";

    }
    else {
        cout << "Nenhuma tarefa foi encontrada com este código!\n";
    }

    system("pause");
}

bool compararPorCodigo(const shared_ptr<Tarefa> &a, const shared_ptr<Tarefa> &b) { return a->codigo < b->codigo; } //Comparação para ordenar por códigos
bool compararPorPrioridade(const shared_ptr<Tarefa>& a, const shared_ptr<Tarefa>& b) { return a->prioridade > b->prioridade; } //Comparação para ordenar por prioridade

void ordernarTarefas(vector<shared_ptr<Tarefa>>& tarefas) { //Passa o vetor para ordenar as tarefas
    int escolha{};

    system("cls");

    while (escolha != 1 && escolha != 2) { //While para checar se os valores enviados pelo usuário estão de acordo.

        cout << "Você deseja ordenar por código ou por prioridade?\n";
        cout << "1 - Código\n";
        cout << "2 - Prioridade\n";
        cout << "Opção: ";
        escolha = checarInteiro("Opção: "); //Checa inteiro do usuário

        cout << endl;

        switch (escolha) { //De acordo com a escolha do usuário...
        case 1:
            cout << "Ordenando por código...\n";
            sort(tarefas.begin(), tarefas.end(), compararPorCodigo); //Ordena por código
            break;
        case 2:
            cout << "Ordenando por prioridade...\n";
            sort(tarefas.begin(), tarefas.end(), compararPorPrioridade); //Ordena por prioridade
            break;
        default:
            cout << "Insira um valor válido\n"; //Caso não seja um valor válido repete.
            break;
        }
    }

    cout << "Tarefas ordenadas!\n";
    system("pause");
}

void salvarTarefas(vector<shared_ptr<Tarefa>>& tarefas) { //Recebe o vetor tarefas para salvá-las
    ofstream arquivoTarefas(arquivoSalvoNome, ios::binary); //cria um ofstream/arquivo para binários

    system("cls");

    cout << "Salvando tarefas...\n";

    try {
        if (!arquivoTarefas.is_open()) { //Se o arquivo não abriu...
            throw runtime_error("Erro ao abrir arquivo para escrita!\n"); //Escreve um erro na tela
            system("pause");
            return; //Encerra a operação
        }

        int quantTarefas = tarefas.size(); //Pega o tamanho do vetor tarefas
        arquivoTarefas.write(reinterpret_cast<const char*>(&quantTarefas), sizeof(quantTarefas)); //Transforma em binário

        //Utilização da Serialização Manual
        //Escolhi utilizar esse método após ter problemas ao transforma variáveis mais complexas, como strings, para binário
        //Desta forma o algoritmo transforma cada parte do struct individualmente seguindo os parâmetros necessários.
        for (const auto& tarefa : tarefas) { 
            arquivoTarefas.write(reinterpret_cast<const char*>(&tarefa->codigo), sizeof(tarefa->codigo)); //transforma os inteiros em binários
            arquivoTarefas.write(reinterpret_cast<const char*>(&tarefa->status), sizeof(tarefa->status)); 
            arquivoTarefas.write(reinterpret_cast<const char*>(&tarefa->prioridade), sizeof(tarefa->prioridade));

            // Salvar `nome` (String)
            int tamanhoNome = tarefa->nome.size(); //pega o tamanho do nome
            arquivoTarefas.write(reinterpret_cast<const char*>(&tamanhoNome), sizeof(tamanhoNome)); //transforma o tamanho em binário
            arquivoTarefas.write(tarefa->nome.c_str(), tamanhoNome); //transforma o nome em binário

            // Salvar `descricao`
            int tamanhoDescricao = tarefa->descricao.size(); //    ^^^ Faz a mesma coisa que a explicação anterior.
            arquivoTarefas.write(reinterpret_cast<const char*>(&tamanhoDescricao), sizeof(tamanhoDescricao));
            arquivoTarefas.write(tarefa->descricao.c_str(), tamanhoDescricao);

            // Salvar `dataCriacao`
            int tamanhoData = tarefa->dataCriacao.size();
            arquivoTarefas.write(reinterpret_cast<const char*>(&tamanhoData), sizeof(tamanhoData));
            arquivoTarefas.write(tarefa->dataCriacao.c_str(), tamanhoData);
        }

        arquivoTarefas.close(); //Fecha o arquivo

        cout << endl;
        cout << "Tarefas Salvas com sucesso!\n";
        system("pause");
    }
    catch(const exception& e) { //Caso ocorra algum erro, imrpime a mensagem de erro e fecha o arquivo.
        cout << "Erro: " << e.what() << endl;
        if (arquivoTarefas.is_open()) {
            arquivoTarefas.close();
        }
    }
}

void carregarTarefas(vector<shared_ptr<Tarefa>>& tarefas) {
    ifstream arquivoTarefas(arquivoSalvoNome, ios::binary); //Cria a ferramenta de leitura de binário

    if (!arquivoTarefas) { //Caso não exista um arquivo
        cout << "Arquivo não encontrado!\n"; //Informa o usuário e retorna
        return;
    }

    int quantTarefas;
    arquivoTarefas.read(reinterpret_cast<char*>(&quantTarefas), sizeof(quantTarefas)); //Pega o tamanho do vetor  tarefas em binário e traduz

    for (int i = 0; i < quantTarefas; i++) { //Faz a operção inversa citada na função de salvar tarefas.
        auto tarefa = make_shared<Tarefa>();

        arquivoTarefas.read(reinterpret_cast<char*>(&tarefa->codigo), sizeof(tarefa->codigo));
        arquivoTarefas.read(reinterpret_cast<char*>(&tarefa->status), sizeof(tarefa->status));
        arquivoTarefas.read(reinterpret_cast<char*>(&tarefa->prioridade), sizeof(tarefa->prioridade));

        // Carregar `nome`
        int tamanhoNome;
        arquivoTarefas.read(reinterpret_cast<char*>(&tamanhoNome), sizeof(tamanhoNome));
        tarefa->nome.resize(tamanhoNome);
        arquivoTarefas.read(&tarefa->nome[0], tamanhoNome);

        // Carregar `descricao`
        int tamanhoDescricao;
        arquivoTarefas.read(reinterpret_cast<char*>(&tamanhoDescricao), sizeof(tamanhoDescricao));
        tarefa->descricao.resize(tamanhoDescricao);
        arquivoTarefas.read(&tarefa->descricao[0], tamanhoDescricao);

        // Carregar `dataCriacao`
        int tamanhoData;
        arquivoTarefas.read(reinterpret_cast<char*>(&tamanhoData), sizeof(tamanhoData));
        tarefa->dataCriacao.resize(tamanhoData);
        arquivoTarefas.read(&tarefa->dataCriacao[0], tamanhoData);

        tarefas.push_back(tarefa); //Adiciona ao vetor de tarefas
    }

    arquivoTarefas.close(); //Fecha o arquivo
    cout << endl;
    cout << "Arquivos lido com sucesso!";
    cout << endl;
    system("pause");
    cout << endl;
}

void atualizarTarefas(vector<shared_ptr<Tarefa>>& tarefas) {
    system("cls");
    vector<shared_ptr<Tarefa>> tarefasTemp; //Cria um vetor temporário de tarefas

    carregarTarefas(tarefasTemp); //Carrega todas as tarefas do arquivo .bin

    if (tarefasTemp.size() == 0) { //Caso ocorra uma falha na leitura, retorna um aviso ao usuário
        cout << "Erro ao atualizar as tarefas! Verifique se o arquivo \"arquivoTarefas.bin\" está na pasta do programa!" << endl;
        system("pause");
        return;
    }

    for (const auto& tarefa : tarefasTemp) { //Para cada item do vetor tarefasTempo...
        auto tarefaEncontrada = pesquisarVetor(tarefas, tarefa->codigo); //Checa se há uma tarefa com o mesmo código
        if (tarefaEncontrada) { //Se a tarefa for encontrada...
            int tarefaCodigoAntes = tarefa->codigo; //Cria uma varíavel com o valor antigo
            while (pesquisarVetor(tarefas, tarefa->codigo)) { //Enquanto tarefa encontrada foi igual a um item do vetor
                tarefa->codigo += 1; //Adiciona 1 ao código
            }
            cout << "Tarefa com código repetido identificada!\n";
            cout << "Alterando código da tarefa " << tarefa->nome << " de " << tarefaCodigoAntes << " para " << tarefa->codigo << endl; //Avisa o usuário sobre a mudança
        }
        tarefas.push_back(tarefa); //Adiciona tarefa ao vetor principal.
    }

    cout << endl;
    cout << "Todos as tarefas foram carregadas com sucesso!";
    system("pause");
}

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");

    vector<shared_ptr<Tarefa>> tarefas; //Declara um vetor de tarefa chamado tarefas com uso de ponteiros inteligentes (shared_ptr)
    int escolha, posicao = 0;


    while (true) { //While para uso contínuo do programa
        imprimirMenu(); //Imprime o menu principal
        escolha = checarInteiro("Insira uma opção: "); //Escolha da operação do usuário

        cout << endl;

        switch (escolha) //Switch da escolha
        {
        case 1:
            cout << "Listar Tarefas\n";
            listarTarefas(tarefas);
            system("cls");
            break;
        case 2:
            cout << "Cadastrar Tarefas\n";
            cadastrarTarefa(posicao, tarefas);
            system("cls");
            break;
        case 3:
            cout << "Buscar Tarefa por código\n";
            buscarTarefaID(tarefas);
            system("cls");
            break;
        case 4:
            cout << "Editar Tarefa\n";
            editarTarefa(tarefas);
            system("cls");
            break;
        case 5:
            cout << "Excluir Tarefa\n";
            excluirTarefa(tarefas);
            system("cls");
            break;
        case 6:
            cout << "Ordernar tarefas por código ou prioridade\n";
            ordernarTarefas(tarefas);
            system("cls");
            break;
        case 7:
            cout << "Salvar tarefas\n";
            salvarTarefas(tarefas);
            system("cls");
            break;
        case 8:
            cout << "Carregar Tarefas\n";
            atualizarTarefas(tarefas);
            system("cls");
            break;
        case 0:
            cout << "Até a próxima!\n";
            return 0;
        default:
            cout << "Insira um valor válido!\n";
            system("pause");
            system("cls");
            break;
        }
    }
}