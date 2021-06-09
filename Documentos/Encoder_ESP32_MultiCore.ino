//Pinos usados como entradas paras as fases dos dois encoder
int e1A = 36;
int e1B = 39;
int e2A = 34;
int e2B = 35;

//Variáveis para acumular a quantidade de pulsos dentro e fora da interrupção, respsctivamente 
volatile double pulsos1 = 0;
volatile double pulsos2 = 0;
double salvaP1 = 0;
double salvaP2 = 0;

//Variáveis para alocar as interrupções em cores diferentes
static byte coreEncoder1 = 0; //número do core onde vão rodar cada uma das interruoções dos encoders
static byte coreEncoder2 = 1;
int tamFila1E2 = 16000;///* número de palavras a serem alocadas para uso com a pilha da tarefa */

// Para configurar seções críticas (interrupções de ativação e interrupções de desativação não disponíveis)
// usado para desabilitar e interromper interrupções
portMUX_TYPE mux1 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE mux2 = portMUX_INITIALIZER_UNLOCKED;


//Funções que serão chamadas quando ocorrer uma interrupção.
//Interrompe a rotina de serviço
//IRAM_ATTR --> é utilizado para indicar que esse trecho de código ficará na seção do barramento de instruções da RAM (maior velocidade) e não na Flash 
//portENTER_CRITICAL_ISR /´portEXIT_CRITICAL_ISR --> Isso é necessário porque a variável que vamos usar também é alterada pelo loop principal, 
void IRAM_ATTR leEncoder1A(void * pvParameters){ //pvParametersfoi declarado como null pois não é necessário
  portENTER_CRITICAL_ISR(&mux1);
  if (digitalRead(e1A) == digitalRead(e1B)) {
    pulsos1 ++;
  }
  else {
    pulsos1--;
  }
  portEXIT_CRITICAL_ISR(&mux1);  
}

void IRAM_ATTR leEncoder1B(void * pvParameters){ //pvParametersfoi declarado como null pois não é necessário
  portENTER_CRITICAL_ISR(&mux1);
  if (digitalRead(e1B) == digitalRead(e1A)) {
    pulsos1 --;
  }
  else {
    pulsos1++;
  }
  portEXIT_CRITICAL_ISR(&mux1);  
}

void IRAM_ATTR leEncoder2A(void * pvParameters){ //pvParametersfoi declarado como null pois não é necessário
  portENTER_CRITICAL_ISR(&mux2);
  if (digitalRead(e2A) == digitalRead(e2B)) {
    pulsos2 ++;
  }
  else {
    pulsos2--;
  }
  portEXIT_CRITICAL_ISR(&mux2);    
}

void IRAM_ATTR leEncoder2B(void * pvParameters){ //pvParametersfoi declarado como null pois não é necessário
  portENTER_CRITICAL_ISR(&mux2);
  if (digitalRead(e2B) == digitalRead(e2A)) {
    pulsos2 --;
  }
  else {
    pulsos2++;
  }
  portEXIT_CRITICAL_ISR(&mux2);    
}

void setup() {

  Serial.begin(250000);
  //os encoderes tem saída do tipo coletor aberto e serão utilizados pullups externos de 22k
  pinMode(e1A,INPUT);
  pinMode(e1B,INPUT);
  pinMode(e2A,INPUT);
  pinMode(e2B,INPUT);
  
  attachInterrupt(digitalPinToInterrupt(e1A), leEncoder1A, CHANGE);//interrupção fase A encoder 1
  attachInterrupt(digitalPinToInterrupt(e1B), leEncoder1B, CHANGE);//interrupção fase B encoder 1
  attachInterrupt(digitalPinToInterrupt(e2A), leEncoder2A, CHANGE);//interrupção fase A encoder 2
  attachInterrupt(digitalPinToInterrupt(e2B), leEncoder2B, CHANGE);//interrupção fase B encoder 2

  //###########################################################################################################################################
  //FUNÇÕES DE ALOCAÇÃO DAS INTERRUPÇÕES EM NÚCLEOS DIFERENTES
  //Diz-se que as interrupções rodam no núcleo onde a função chamada é implementada
  //portanto basta atribuir o núcleo à função que a interupção a seguirá

  //Função que lê a interrupção da fase A do encoder 1 no core 0
  xTaskCreatePinnedToCore(
                  leEncoder1A,   /* função que implementa a tarefa */
                  "E1A", /* nome da tarefa */
                  tamFila1E2,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
                  NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
                  3,          /* prioridade da tarefa (0 a N) */
                  NULL,       /* referência para a tarefa (pode ser NULL) */
                  coreEncoder1);         /* Núcleo que executará a tarefa */
                  
  delayMicroseconds(500000); //tempo para a tarefa iniciar

  //Função que lê a interrupção da fase B do encoder 1 no core 0
  xTaskCreatePinnedToCore(
                leEncoder1B,   /* função que implementa a tarefa */
                "E1B", /* nome da tarefa */
                tamFila1E2,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
                NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
                3,          /* prioridade da tarefa (0 a N) */
                NULL,       /* referência para a tarefa (pode ser NULL) */
                coreEncoder1);         /* Núcleo que executará a tarefa */
                  
  delayMicroseconds(500000); //tempo para a tarefa   
  
  //Função que lê a interrupção da fase A do encoder 2 no core 1
  xTaskCreatePinnedToCore(
                leEncoder2A,   /* função que implementa a tarefa */
                "E2A", /* nome da tarefa */
                tamFila1E2,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
                NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
                3,          /* prioridade da tarefa (0 a N) */
                NULL,       /* referência para a tarefa (pode ser NULL) */
                coreEncoder2);         /* Núcleo que executará a tarefa */
                  
  delayMicroseconds(500000); //tempo para a tarefa   //Função que lê a interrupção da fase B do encoder 1 no core 0

  //Função que lê a interrupção da fase B do encoder 2 no core 1
  xTaskCreatePinnedToCore(
                leEncoder2B,   /* função que implementa a tarefa */
                "E2B", /* nome da tarefa */
                tamFila1E2,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
                NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
                3,          /* prioridade da tarefa (0 a N) */
                NULL,       /* referência para a tarefa (pode ser NULL) */
                coreEncoder2);         /* Núcleo que executará a tarefa */
                  
  delayMicroseconds(500000); //tempo para a tarefa iniciar

  //####################################################################################################################

  

}

void loop() {
    
  portENTER_CRITICAL_ISR(&mux1); // início da seção crítica
    salvaP1 = pulsos1;
  portEXIT_CRITICAL_ISR(&mux1); // fim da seção crítica
  
  portENTER_CRITICAL_ISR(&mux2); // início da seção crítica
    salvaP2 = pulsos2;
  portEXIT_CRITICAL_ISR(&mux2); // fim da seção crítica

  Serial.print("Encoder 1 = ");
  Serial.print(salvaP1);
  Serial.print("\t Encoder 2 = ");
  Serial.println(salvaP2);

  delay(10);
}
