/*Paulo H. Langone
  SENAI Anchieta  CST Eletrônica Industrial
  EXTUN4 */
 
// Inclusão das bibliotecas
#include <stdio.h>
#include <string.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "teams.h"

// Define e configura displays
#define SCREEN_WIDTH      128
#define SCREEN_HEIGHT     64
#define OLED_RESET        4
#define SCREEN_ADDRESS    0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Notas da melodia de fim de jogo
#define NOTE_G5           783
#define NOTE_C6           1046
#define NOTE_E6           1318
#define NOTE_G6           1567

//Definição dos pinos
#define BUZZER_PIN        16
#define BUTTON_0_PIN      13
#define BUTTON_1_PIN      12
#define BUTTON_2_PIN      14
#define BUTTON_3_PIN      27
#define BUTTON_4_PIN      26
#define BUTTON_5_PIN      25
#define BUTTON_6_PIN      33
#define BUTTON_7_PIN      32
#define SELECTION_C_PIN   5
#define SELECTION_B_PIN   2
#define SELECTION_A_PIN   15

/*
 * Declaração das variáveis globais
*/
// Tempo de exibição dos pares durante o início da rodada
int TEMPO_EXIBICAO = 2000;

// Vetor que armazena a ordem das posições sorteadas para os pares
static int teamsIndex[8];

// Vetor que armazena o nome dos times nas posições sorteadas em teamsIndex, é utilizada para verificar o acerto ou erro da jogada em loop()
char* shuffledTeams[8];

// Arrays ordenados dos times para ser utilizado em printTeams()
char* tournments[8] = {"brasileirao", "bundesliga", "fwc", "laliga", "mls", "premier_league", "serie_a", "uafa"};
char* brasileirao_teams[8] = {"corinthians", "palmeiras", "santos", "sao_paulo", "corinthians", "palmeiras", "santos", "sao_paulo"};
char* premier_league_teams[8] = {"liverpool", "manchester_city", "manchester_united", "tottenham", "liverpool", "manchester_city", "manchester_united", "tottenham"};
char* laliga_teams[8] = {"atletico_bilbao", "atletico_madrid", "barcelona", "real_madrid", "atletico_bilbao", "atletico_madrid", "barcelona", "real_madrid"};
char* bundesliga_teams[8] = {"bayer_leverkusen", "bayer_munique", "borussia", "wolfsburg", "bayer_leverkusen", "bayer_munique", "borussia", "wolfsburg"};
char* serie_a_teams[8] = {"atalanta", "inter_milao", "juventus", "milan", "atalanta", "inter_milao", "juventus", "milan"};
char* fwc_teams[8] = {"argentina", "brasil", "franca", "holanda", "argentina", "brasil", "franca", "holanda"};
char* mls_teams[8] = {"orlando_city", "la_galaxy", "chicago_fire", "inter_miami"};
char* uafa_teams[8] = {"al_ahli", "al_fateh", "al_hilal", "al_tai", "al_ahli", "al_fateh", "al_hilal", "al_tai"};

/*
 * Função para realizar a multiplexação do barramento de dados (SDA) do I2C conforme pinos de controle de seleção
 * @param index - índice do display que se deseja selecionar (0-7)
*/
void selDisp(int index) {
  if(index == 0) {
  digitalWrite(SELECTION_A_PIN, LOW);
  digitalWrite(SELECTION_B_PIN, LOW);
  digitalWrite(SELECTION_C_PIN, LOW);
  }
  else if(index == 1) {
  digitalWrite(SELECTION_A_PIN, HIGH);
  digitalWrite(SELECTION_B_PIN, LOW);
  digitalWrite(SELECTION_C_PIN, LOW);
  }
  else if(index == 2) {
  digitalWrite(SELECTION_A_PIN, LOW);
  digitalWrite(SELECTION_B_PIN, HIGH);
  digitalWrite(SELECTION_C_PIN, LOW);
  }
  else if(index == 3) {
  digitalWrite(SELECTION_A_PIN, HIGH);
  digitalWrite(SELECTION_B_PIN, HIGH);
  digitalWrite(SELECTION_C_PIN, LOW);
  }
  else if(index == 4) {
  digitalWrite(SELECTION_A_PIN, LOW);
  digitalWrite(SELECTION_B_PIN, LOW);
  digitalWrite(SELECTION_C_PIN, HIGH);
  }
  else if(index == 5) {
  digitalWrite(SELECTION_A_PIN, HIGH);
  digitalWrite(SELECTION_B_PIN, LOW);
  digitalWrite(SELECTION_C_PIN, HIGH);
  }
  else if(index == 6) {
  digitalWrite(SELECTION_A_PIN, LOW);
  digitalWrite(SELECTION_B_PIN, HIGH);
  digitalWrite(SELECTION_C_PIN, HIGH);
  }
  else if(index == 7) {
  digitalWrite(SELECTION_A_PIN, HIGH);
  digitalWrite(SELECTION_B_PIN, HIGH);
  digitalWrite(SELECTION_C_PIN, HIGH);
  }
}

/*
 * Função que identifica qual botão foi pressionado
 * @return - número inteiro de 0 a 7 representando o botão pressionado
*/
int checkButton(){
  // Variáveis auxiliares de escopo
  bool botaoPressionado = false;
  int aux = 0;
  int button;
  
  while (true){

    // Botão 0
    if(digitalRead(BUTTON_0_PIN) == LOW && aux == 0){
      Serial.println("Botão 0");
      button = 0;
      aux = 1;
    }
    if(digitalRead(BUTTON_0_PIN) == HIGH && aux == 1){
      aux = 0 ;
      delay(100);
      break;
    }

    // Botão 1
    if(digitalRead(BUTTON_1_PIN) == LOW && aux == 0){
       Serial.println("Botão 1");
      button = 1;
      aux = 1;
    }
    if(digitalRead(BUTTON_1_PIN) == HIGH && aux == 1){
      aux = 0;
      delay(100);
      break;
    } 

    // Botão 2
    if(digitalRead(BUTTON_2_PIN) == LOW && aux == 0){
      Serial.println("Botão 2");
      button = 2;
      aux = 1;
    }
    if(digitalRead(BUTTON_2_PIN) == HIGH && aux == 1){
      aux = 0;
      delay(100);
      break;
    } 

    // Botão 3
    if(digitalRead(BUTTON_3_PIN) == LOW && aux == 0){
      Serial.println("Botão 3");
      button = 3;
      aux = 1;
    }
    if(digitalRead(BUTTON_3_PIN) == HIGH && aux == 1){
      aux = 0;
      delay(100);
      break;
    } 

    // Botão 4
    if(digitalRead(BUTTON_4_PIN) == LOW && aux == 0){
      Serial.println("Botão 4");
      button = 4;
      aux = 1;
    }
    if(digitalRead(BUTTON_4_PIN) == HIGH && aux == 1){
      aux = 0;
      delay(100);
      break;
    }

    // Botão 5
    if(digitalRead(BUTTON_5_PIN) == LOW && aux == 0){
      Serial.println("Botão 5");
      button = 5;
      aux = 1;
    }
    if(digitalRead(BUTTON_5_PIN) == HIGH && aux == 1){
      aux = 0;
      delay(100);
      break;
    }

    // Botão 6
    if(digitalRead(BUTTON_6_PIN) == LOW && aux == 0){
      Serial.println("Botão 6");
      button = 6;
      aux = 1;
    }
    if(digitalRead(BUTTON_6_PIN) == HIGH && aux == 1){
      aux = 0;
      delay(100);
      break;
    }

    // Botão 7
    if(digitalRead(BUTTON_7_PIN) == LOW && aux == 0){
      Serial.println("Botão 7");
      button = 7;
      aux = 1;
    }
    if(digitalRead(BUTTON_7_PIN) == HIGH && aux == 1){
      aux = 0;
      delay(100);
      break;
    }
  }
  return button;     
}

/*
 * Função para exibir nos displays os emblemas dos times do torneio selecionado
 * @param **selected_tournment_teams_vector - aponta para o vetor allArray do torneio selecionado, que por sua vez aponta para os vetores de pixels das imagens
 * @param **teams_vector - aponta para o vetor de nomes dos times do torneio selecionado, este vetor será "embaralhado" e utilizado em loop() para verificar as jogadas
*/
void printTeams(unsigned char** selected_tournment_teams_vector, char** teams_vector){

  //Cria array de exibição dos display 
  for (int i = 0; i < 8; i++) {
    int randomNum;
    bool repeatedNum;
                              
    do{
      repeatedNum = false;
      randomNum = random(8); 

      for (int j = 0; j < i; j++) {
        if (teamsIndex[j] == randomNum) {
          repeatedNum = true;
          break;
        }
      }
    } while (repeatedNum);
    teamsIndex[i] = randomNum;
  }

  //Laço para exibir os times nos diplays
  for(int i=0;i<8;i++){
    selDisp(i);
    display.clearDisplay();
    display.invertDisplay(true);
    display.drawBitmap(0, 0, selected_tournment_teams_vector[teamsIndex[i]], 128, 64, SSD1306_WHITE);     
    display.display();
    display.invertDisplay(false);
    shuffledTeams[i] = teams_vector[teamsIndex[i]];
  } 
}

void setup() {

  Serial.begin(115200);
  
  // Inicialização dos pinos
  pinMode(SELECTION_A_PIN, OUTPUT);
  pinMode(SELECTION_B_PIN, OUTPUT);
  pinMode(SELECTION_C_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_0_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);
  pinMode(BUTTON_4_PIN, INPUT_PULLUP);
  pinMode(BUTTON_5_PIN, INPUT_PULLUP);
  pinMode(BUTTON_6_PIN, INPUT_PULLUP);
  pinMode(BUTTON_7_PIN, INPUT_PULLUP);

  // Inicialização dos displays no endereço 0x3C do I2C
  Wire.begin();
  for(int i=0;i<8;i++){
    selDisp(i);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
  }  
}

void loop() {
  // Seleção de Torneio - Exibe os torneios disponíveis e armazena a seleçao em selectedTournment
  for(int i=0;i<8;i++){
    selDisp(i);
    display.clearDisplay();
    display.invertDisplay(true);
    display.drawBitmap(0, 0, torneios_allArray[i], 128, 64, SSD1306_WHITE);
    display.display();
    display.invertDisplay(false);
  }

  char* selectedTournment = tournments[checkButton()];
  
  Serial.println("O torneio selecionado foi: ");
  Serial.println(selectedTournment);

  /* Vetor que informa, durante o laço de jogadas, qual array (allArray) de torneio
   deve ser utilizado para exibir as imagens nos displays*/
  unsigned char** selectedTournmentArray;

  // Exibe nos displays os pares de times do torneio selecionado
  // Brasileirão
  if(selectedTournment == "brasileirao"){
    Serial.println("SELEÇÃO FEITA");
    printTeams(brasileirao_allArray, brasileirao_teams);
    selectedTournmentArray = brasileirao_allArray;
  }

  // Bundesliga
  else if(selectedTournment == "bundesliga"){
    Serial.println("SELEÇÃO FEITA");   
    printTeams(bundesliga_allArray, bundesliga_teams);
    selectedTournmentArray = bundesliga_allArray;
  }

  // Fifa World Cup
  else if(selectedTournment == "fwc"){
    Serial.println("SELEÇÃO FEITA");
    printTeams(fwc_allArray, fwc_teams);
    selectedTournmentArray = fwc_allArray;
  }

  // La Liga
  else if(selectedTournment == "laliga"){
    Serial.println("SELEÇÃO FEITA");
    printTeams(laliga_allArray, laliga_teams);
    selectedTournmentArray = laliga_allArray;
  }

  // MLS
  else if(selectedTournment == "mls"){
    Serial.println("SELEÇÃO FEITA");
    printTeams(mls_allArray, mls_teams);
    selectedTournmentArray= mls_allArray;
  }

  // Premier League
  else if(selectedTournment == "premier_league"){
    Serial.println("SELEÇÃO FEITA");
    printTeams(premierleague_allArray, premier_league_teams);
    selectedTournmentArray= premierleague_allArray;
  } 

  // Serie A
  else if(selectedTournment == "serie_a"){
    Serial.println("SELEÇÃO FEITA");
    printTeams(serie_a_allArray, serie_a_teams);
    selectedTournmentArray = serie_a_allArray;
  }

  // UAFA - Liga Árabe
  else if(selectedTournment == "uafa"){
    Serial.println("SELEÇÃO FEITA");
    printTeams(uafa_allArray, uafa_teams);
    selectedTournmentArray = uafa_allArray;
  }

  // Tempo de exibição dos pares
  delay(TEMPO_EXIBICAO);

  // Apaga os displays
  for(int i=0;i<8;i++){
    selDisp(i);
    display.clearDisplay();
    display.display();
  }

  // Início do loop de jogadas
  int pairsLeft = 4;
  while(pairsLeft != 0){

    Serial.println("Início do loop de jogadas");

    //Variáveis do escopo     
    bool selection1 = false;
    bool selection2 = false;
    int selection1Index;
    int selection2Index;

    while(!selection1 && !selection2){
      // Armazena índice do primeiro botão pressionado
      selection1Index = checkButton();

      // Verifica se o mesmo botão não foi pressionado 2x
      if(selection1Index != selection2Index && teamsIndex[selection1Index] != -1){

        // Exibe a imagem no display selecionado
        selDisp(selection1Index);
        display.clearDisplay();
        display.drawBitmap(0, 0, selectedTournmentArray[teamsIndex[selection1Index]], 128, 64, SSD1306_WHITE);
        display.display();
        selection1 = true;
        delay(1000);
      }

      // Armazena índice do segundo botão pressionado
      selection2Index = checkButton();

      // Verifica se o mesmo botão não foi pressionado 2x
      if(selection1Index != selection2Index && teamsIndex[selection2Index] != -1){

        // Exibe imagem no display selecionado
        selDisp(selection2Index);
        display.clearDisplay();
        display.drawBitmap(0, 0, selectedTournmentArray[teamsIndex[selection2Index]], 128, 64, SSD1306_WHITE);
        display.display();
        selection2 = true;
      }
    }

    /*
     * Ponteiros que apontam para o nome dos times selecionados
    */
    char* selection1Team = shuffledTeams[selection1Index];
    char* selection2Team = shuffledTeams[selection2Index];

    // Compara as duas strings para verificar se o jogador acertou os pares, e caso tenha acertado:
    if(strcmp(selection1Team,selection2Team) == 0){

      // Atribuições para serem utilizadas na verificação da seleção repetida de imagens (mesmo botão pressionado 2x)
      teamsIndex[selection1Index] = -1;
      teamsIndex[selection2Index] = -1;

      // Decremente os pares restantes, uma vez que o jogador acertou o par
      pairsLeft--;

      // Indicação sonora de acerto da jogada
      tone(BUZZER_PIN,600,500);
      delay(500);
    }

    // Caso tenha errado
    else{
      // Indicação sonora de erro da jogada
      tone(BUZZER_PIN,100,500);
      delay(500);

      /*
       * Limpa os displays da seleção, uma vez que os pares estão errados
      */
      selDisp(selection1Index);
      display.clearDisplay();
      display.display();
      selDisp(selection2Index);
      display.clearDisplay();
      display.display();      
    }
  } // Fim do loop de jogadas
  
  //Limpa displays
  for(int i=0;i<8;i++){
    selDisp(i);
    display.clearDisplay();
    display.display();
  }

  /*
   * Indicação sonora e visual de Fim de Jogo 
  */
  int melody[] = {
    NOTE_G5, NOTE_C6, NOTE_E6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_G5
  };

  int noteDurations[] = {
    4, 4, 4, 4, 4, 4, 2
  };

  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    selDisp(i);
    display.drawBitmap(0, 0, jogadorcomemora , 128, 64, SSD1306_WHITE);
    display.display();
    int noteDuration = 1000 / noteDurations[i];
    tone(BUZZER_PIN, melody[i], noteDuration);
    delay(noteDuration + 30);
    noTone(BUZZER_PIN);
    delay(10); // Pequena pausa entre as notas
    selDisp(i);
    display.clearDisplay();
  }
}

