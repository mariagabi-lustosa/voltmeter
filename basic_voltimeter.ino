int valorPorta = 0;
float tensao;
float r1 = 3000000.0, r2 = 100000.0;
float refArduino = 5.0; //Vout
float fator = ((r1 + r2) / r2); //divisor de tensao
int counterN = 0; //conta a quantidade de valores de "tensao" lidos
float counterPot = 0.0; //incrementa o somatorio dos valores de potencia calculados => energia


void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
}


void leTensao()
{
  //medindo o valor da tensao a cada ciclo
  valorPorta = analogRead(A0);
  tensao = valorPorta * (refArduino / 1023) * fator; //conversao do valor lido atraves da porta analogica
  Serial.println("Valor da tensao em V: ");
  Serial.println(tensao, 3);
  Serial.println();
 
  counterN++;
  delay(1000); //medições são realizadas a cada 1 segundo
}


void somatorioPot()
{
  //calculo da energia gerada da forma mais simples possivel: como somatorio dos valores das potencias
  //isso so e possivel devido ao temporizador
  float pot = pow(tensao, 2) / r1;
  counterPot += pot;
 
  Serial.println("Energia total em J: ");
  Serial.println(counterPot, 3);
  Serial.println();
}


void loop(void)
{
  if (digitalRead(2) == HIGH) { //o programa so ira comecar quando o botao for acionado
	while (counterN < 300) { //o programa ira rodar por 300s = 5min
  	leTensao();
  	somatorioPot();
	}

  }
}
