#include "arena.h"
#include <cstring>
#include <string>
#include "exceptions.h"

Arena::Arena(){
    for(int i = 0; i < QTD_MAX_PLAYERS; i++){
        cout << "Insira o nome do jogador " << (i+1) << ": " ;
        string nome_player;
        cin >> nome_player;
        cout << endl;
        Player player(nome_player);
        set_player(player);
    }

    inicializa_pokemons();
}

void Arena::inicializa_pokemons(){
    //Pokemons tipo neve
    _lista_pokemons.push_back(Pokemon("Nevasca", vector<Ataque>{Ataque("Nevasca",10), Ataque("Nevasca", 6), Ataque("Nevasca", 3)}, 80, "neve"));
    _lista_pokemons.push_back(Pokemon("Rei_Gelado", vector<Ataque>{Ataque("Rei_Gelado", 15), Ataque("Rei_Gelado", 8), Ataque("Rei_Gelado", 5)}, 46, "neve"));
    _lista_pokemons.push_back(Pokemon("Mandaloriano", vector<Ataque>{Ataque("Mandaloriano", 20), Ataque("Mandaloriano", 14), Ataque("Mandaloriano", 8)},32,"neve"));

    //Pokemons tipo fogo
    _lista_pokemons.push_back(Pokemon("Esquentadinho", vector<Ataque>{Ataque("Esquentadinho", 8),Ataque("Esquentadinho", 6),Ataque("Esquentadinho", 2)}, 76, "fogo"));
    _lista_pokemons.push_back(Pokemon("Charizard", vector<Ataque>{Ataque("Charizard", 17), Ataque("Charizard", 7), Ataque("Charizard", 4)}, 52, "fogo"));
    _lista_pokemons.push_back(Pokemon("Monark", vector<Ataque>{Ataque("Monark", 19), Ataque("Monark", 13), Ataque("Monark", 10)}, 29, "fogo"));

    //Pokemons tipo agua
    _lista_pokemons.push_back(Pokemon("Zé_Gotinha", vector<Ataque>{Ataque("Zé_Gotinha", 9),Ataque("Zé_Gotinha", 7),Ataque("Zé_Gotinha", 3)}, 77, "agua"));
    _lista_pokemons.push_back(Pokemon("Kratos", vector<Ataque>{Ataque("Kratos", 13),Ataque("Kratos", 9),Ataque("Kratos", 5)}, 48, "agua"));
    _lista_pokemons.push_back(Pokemon("Mbappe", vector<Ataque>{Ataque("Mbappe", 29),Ataque("Mbappe", 20),Ataque("Mbappe", 15)}, 25, "agua"));
    _qtd_pokemons_disponiveis = _lista_pokemons.size();
}

void Arena::exibe_pokemons_disponiveis(){
    cout << "Lista de Pokemons disponíveis :" << endl;
    int contador = 0;
    for(Pokemon pokemon: _lista_pokemons){
        string vlr_ataques;
        for(int i = 1; i <= pokemon.get_quantidade_ataques(); i++){
            vlr_ataques += " " + to_string(pokemon.get_ataque(i).get_dano());
        }
        contador++;
        cout << "Pokemon " << contador <<
            " | | Nome: " << pokemon.get_nome() <<
            " | | Ataques" << vlr_ataques <<
            " | | Vida: " << pokemon.get_vida() <<
            " | | Elemento: "<< pokemon.get_elemento()
        << endl;
    }
}

Player Arena::get_player(int index_player){
    if(index_player <= 0 || index_player > _qtd_players){
        throw PlayerNaoEncontradoException();
    }
    return _players[index_player - 1];
}

void Arena::set_player(Player player){
    if(_qtd_players >= QTD_MAX_PLAYERS){
        throw NumeroMaximoPlayersException();
    }
    _players.push_back(player);
    _qtd_players++;
}

int Arena::balanceamento_por_elemento(const char* elemento1, const char* elemento2){
    // neve ganha de agua // agua ganha de fogo // fogo ganha de neve
    // se a função retorna 0 , ninguem tem vantagem ; se retorna 1 , player 1 tem vantagem , se retorna 2 , player 2 tem vantagem 
    int comparavel = 0;
    int compneve = 0;
    int compfogo = 0;
    int compagua = 0;

    comparavel = strcmp(elemento1,elemento2);

    if(comparavel == 0){
        return 0;
    }

    //comparador para primeiro = fogo 
    compfogo = strcmp(elemento1,"fogo");

    if(compfogo == 0){    
        comparavel = strcmp(elemento1,elemento2);
        if(comparavel>0){
            return 2;
        }else { 
            return 1; 
        }
    }
    //comparador para primeiro = agua 
    compagua = strcmp(elemento1,"agua");

    if(compagua ==0){    
        comparavel = strcmp(elemento2,"fogo");
        if(comparavel==0){
            return 1;
        }
        return 2;
    }

    //comparador para primeiro = neve

    compneve = strcmp(elemento1,"neve");

    if(compneve ==0){
        comparavel = strcmp(elemento2,"fogo");
        if(comparavel==0){
            return 2;
        }
        return 1;
    }
    return -255;
}

// Ganhou recebe as vidas e compara se é maior que 0;
bool Arena::ganhou(int vida1,int vida2){
    if(vida1 <= 0 || vida2 <= 0){
        string nom_jogador_vencedor;
        if(vida1 == 0){
            nom_jogador_vencedor = get_player(2).get_nome();
        }else{
            nom_jogador_vencedor = get_player(1).get_nome();
        }
        cout << "Jogador "<< nom_jogador_vencedor << " é o vencedor!!!" << endl; 
        return true;
    }

    return false;
}

// Ataque recebe as duas vidas, o dano que uma irá receber e a vez que indica quem vai receber o dano. 
//A função retorna o vida atual do player que recebeu o dano.
int Arena::ataque(int life1, int life2, int dano, int vez){
    if(vez == 0){
        life2 = life2 - dano;
        return life2;
    }else{   
        life1 = life1 - dano;
        return life1;
    }  
}

void Arena::game(){
    int i = 0;
    int vantagem;
    
    int life1 = 0;
    int life2 = 0;
    
    life1 = get_player(1).get_pokemon(1).get_vida();
    life2 = get_player(2).get_pokemon(1).get_vida();
   
    vantagem = balanceamento_por_elemento(get_player(1).get_pokemon(1).get_elemento(), get_player(2).get_pokemon(1).get_elemento());
    
    while(!ganhou(life1, life2)){
        
        
        int desviar = 0;
        int ataque_usado = 0;
        int danoso = 0;
        int vez_de = 0;

        // Interface para mostrar as informações dos jogadores.
        
        cout << get_player(1).get_nome() << endl;
        cout << get_player(1).get_pokemon(1).get_nome() << " Vida: " << life1 << endl;
        cout << endl;
        
        cout << get_player(2).get_nome() << endl;
        cout << get_player(2).get_pokemon(1).get_nome() << " Vida: " << life2 << endl;
        cout << "---------------------------------------------------------------------------------------------------" << endl;

        cout << "Agora é a vez de " ;

        // Se i é par será a vez do jogador  

        if(i % 2 == 0){ // Ataque do primeiro jogador.
            vez_de = 0;
            cout << get_player(1).get_nome() << endl;
            cout << "Escolha o ataque a ser usado :" << endl;
            get_player(1).get_pokemon(1).exibe_ataques();

            cin >> ataque_usado;
            danoso  = get_player(1).get_pokemon(1).get_ataque(ataque_usado).get_dano();
            desviar = get_player(2).get_pokemon(1).desviou(danoso);

            cout << "---------------------------------------------------------------------------------------------------" << endl;

            if(desviar == 1){
                cout << "O " << get_player(2).get_pokemon(1).get_nome() << " do jogador "<<get_player(2).get_nome() <<" desviou!" << endl;
            }else{
                if(vantagem == 1){
                    danoso = danoso * 1.25;
                }
            life2 = ataque(life1,life2, danoso,vez_de);
                cout << "O " << get_player(2).get_pokemon(1).get_nome() << " do jogador " << get_player(2).get_nome() << " recebeu " << danoso << " de dano"<< endl;
            }
        }
        else{ // Ataque do segundo jogador.
            vez_de = 1;
            cout << get_player(2).get_nome() << endl;
            cout << "Escolha o ataque a ser usado :" << endl;
            get_player(2).get_pokemon(1).exibe_ataques();
            cin >> ataque_usado;

            danoso = get_player(2).get_pokemon(1).get_ataque(ataque_usado).get_dano();

            desviar = get_player(1).get_pokemon(1).desviou(danoso);
            
            cout << "---------------------------------------------------------------------------------------------------" << endl;

            if(desviar==1){
                cout << "O " << get_player(1).get_pokemon(1).get_nome() << " do jogador " << get_player(1).get_nome() << " desviou!" << endl;
            }else{
                if(vantagem == 2){
                    danoso = danoso * 1.25;
                }
            life1 = ataque(life1,life2, danoso,vez_de);
                cout << "O " << get_player(1).get_pokemon(1).get_nome() << " do jogador " << get_player(1).get_nome() << " recebeu " << danoso << " de dano" << endl;
            }
        }

        cout << "---------------------------------------------------------------------------------------------------" << endl;

        i++;
    }
}

//Método que cria uma janela para os players escolherem seus pokémons e que atribui pokemons à esses players.
void Arena::players_escolhem_pokemon(){
    for(auto it = _players.begin(); it != _players.end(); it++){
        cout << "---------------------------------------------------------------------------------------------------" << endl;
        exibe_pokemons_disponiveis();
        cout << endl;
        int pokemon_escolhido;
        cout <<  it->get_nome() << ", selecione seu pokemon: ";
        cin >> pokemon_escolhido;
        if(pokemon_escolhido > _qtd_pokemons_disponiveis || pokemon_escolhido <= 0){
            throw OpcaoInvalidaException();
        }
        it->set_pokemon(_lista_pokemons[pokemon_escolhido - 1]);
    }
}
