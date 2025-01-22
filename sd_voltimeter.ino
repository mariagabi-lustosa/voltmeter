#include <SPI.h>
#include <SD.h>


/*
* Declarações de variáveis
*/
int valorPorta = 0;
float tensao;
float r1 = 3000000.0, r2 = 100000.0;
float refArduino = 5.0; //Vout
int counterN = 0;
File dataFile;
const int chipSelect = 10;
String fileName = "";
int index = 0;
int arquivoAtual = 1;


void setup()
{
 Serial.begin(9600);
 pinMode(2, INPUT_PULLUP);

 while (!Serial); //esperando a conexão da porta serial
  Serial.print("Iinicializando cartão SD...\n");

 if (!SD.begin(chipSelect)) { //enquanto a porta não for reconhecida, fica nesse loop
   Serial.println("Inicialização falhou\n");
   while(true);
 }
  Serial.println("Cartão SD inicializado com sucesso.\n");

 if (index == 0) {
   if (SD.exists("dados_1.txt")) {
     while(SD.exists("dados_"+String(arquivoAtual)+".txt")) {
       arquivoAtual++;
     }
   }
   fileName = "dados_" + String(arquivoAtual) + ".txt";
   dataFile = SD.open(fileName, FILE_WRITE);

   if (dataFile) {
     dataFile.println("Tensão (V)");
     Serial.print("Salvando dados em: ");
     Serial.println(fileName);
   } else {
     Serial.println("Falha ao abrir o arquivo");
   }
 }
}


/*
* Essa função realiza a medição da tensão Vin através da porta analógica
* do arduino.
* O valor recebido é convertido de analógico para digital, através da relação:
* resolução = Vref / 2^n, em que n é a quantidade de bits de resolução do modelo utilizado.
* No caso, a resolução do arduino nano é 10 bits.
*/
float leTensao()
{
 //medindo o valor da tensão a cada ciclo
 valorPorta = analogRead(A0);
 tensao = valorPorta * (refArduino / 1023) * ((r1 + r2) / r2);
  counterN++; //conta a quantidade de valores de "tensão" lidos
  delay(10);
 return tensao;
}


void loop(void) {
 if (digitalRead(2) == HIGH) {
   if (dataFile && (counterN < 12000)) {
     float tensaoAtual = leTensao();
     Serial.println(tensaoAtual, 10);
     dataFile.println(tensaoAtual, 10);
     index++;
   }
  
   if (((digitalRead(2) == LOW) && (index != 0)) || counterN >= 12000) {
     index = 0;
     if (dataFile) {
       Serial.println(counterN);
      
       dataFile.close();
       Serial.println("Se chegou aqui, funcionou.");
     }
   }
 }
}
