
#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5
#define INDEFINIDO -1
#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500
#define TAMANHO_SEQUENCIA 4


#define BOTAO_VERDE 8
#define BOTAO_AMARELO 9
#define BOTAO_VERMELHO 10
#define BOTAO_AZUL 11



enum Estados{
  PRONTO_PARA_PROX_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_SUCESSO,
  JOGO_FINALIZADO_FALHA
};

int sequenciaLuzes[TAMANHO_SEQUENCIA];
int rodada = 0;
int leds_respondidos = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo(); 
}

void iniciaPortas(){
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);

  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);

}



void loop() {
  switch(estadoAtual()){
    case PRONTO_PARA_PROX_RODADA:
    Serial.println("Pronto para a proxima rodada");
    preparaNovaRodada();
    break;
    case USUARIO_RESPONDENDO:
    Serial.println("Usuario respondendo");
    processaRespostaUsuario();
    break;
    case JOGO_FINALIZADO_SUCESSO:
    Serial.println("Jogo finalizado com sucesso");
    jogoFinalizadoSucesso();
    break;
    case JOGO_FINALIZADO_FALHA:
    Serial.println("Jogo finalizado com falha");
    jogoFinalizadoFalha();
    break;

  }
  delay(MEIO_SEGUNDO);
}


void preparaNovaRodada(){
  rodada++;
  leds_respondidos = 0;
  if(rodada<=TAMANHO_SEQUENCIA){
    tocaLedsRodada();
  }
}


void processaRespostaUsuario(){
  int resposta = checaRespostaJogador();
  
  if(resposta == INDEFINIDO){
    return;
  }

  if(resposta == sequenciaLuzes[leds_respondidos]){
    leds_respondidos++;
  }else{
    Serial.println("resposta errada");
    rodada = TAMANHO_SEQUENCIA+2;
  }
  
}

int estadoAtual(){
 if(rodada <= TAMANHO_SEQUENCIA){
  if(leds_respondidos==rodada){
    return PRONTO_PARA_PROX_RODADA;
  }else{
    return USUARIO_RESPONDENDO;
  }
  
 }else if(rodada == TAMANHO_SEQUENCIA + 1){
    return JOGO_FINALIZADO_SUCESSO;
 }else{
  return JOGO_FINALIZADO_FALHA;
 }
}

void tocaLedsRodada(){
  for(int indice=0; indice<rodada; indice++){
    piscaLed(sequenciaLuzes[indice]);
  }
}

int checaRespostaJogador(){
    if(digitalRead(BOTAO_VERDE) == LOW){
        return piscaLed(LED_VERDE);
    }
    if(digitalRead(BOTAO_AMARELO) == LOW){
        return piscaLed(LED_AMARELO);
    }
    if(digitalRead(BOTAO_VERMELHO) == LOW){
        return piscaLed(LED_VERMELHO);
    }
    if(digitalRead(BOTAO_AZUL) == LOW){
        return piscaLed(LED_AZUL);
    }

    return INDEFINIDO;
}

void jogoFinalizadoSucesso(){
  piscaLed(LED_VERDE);
  piscaLed(LED_AMARELO);
  piscaLed(LED_VERMELHO);
  piscaLed(LED_AZUL);
  delay(MEIO_SEGUNDO);
}

void iniciaJogo(){
  int jogo = analogRead(0);
  randomSeed(jogo);
  for(int indice=0; indice<TAMANHO_SEQUENCIA; indice++){
    sequenciaLuzes[indice] = sorteiaCor();
  }
}

int sorteiaCor(){
  return random(LED_VERDE, LED_AZUL+1); //2 - 5 (6)
}

void jogoFinalizadoFalha(){
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  delay(MEIO_SEGUNDO);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  delay(MEIO_SEGUNDO);
}

int piscaLed(int portaLed){
  digitalWrite(portaLed, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(portaLed, LOW);
  delay(MEIO_SEGUNDO);
  return portaLed;
}
