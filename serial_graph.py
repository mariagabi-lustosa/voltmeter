import serial
import matplotlib.pyplot as plt
import time

arduino_port = '/dev/ttyUSB0'  #settando a porta serial (no Windows é 'COM3')
baud_rate = 9600  #o valor aqui e no programa do arduino deve ser igual

r1 = 3000000
r2 = 100000

ser = serial.Serial(arduino_port, baud_rate)

#configurando o gráfico
plt.ion()
plt.plot(figsize=(10, 6))
x_vals = []
y_vals = []

instante_inicial = time.time()
energia = 0.0  #a energia acumulada inicial é zero

while True:
   try:
       #leitura da potência enviada pelo Arduino
       linha = ser.readline().decode('utf-8').strip() #recebendo o valor enviado pela porta serial
       if linha:
           potencia = float(linha)
           tempo_total = time.time() - instante_inicial
           print(f"Potência: {potencia} W")

           #integração numérica pelo método dos trapézios
           if len(x_vals) > 0:
               delta_t = tempo_total - x_vals[-1]
               energia += (potencia + y_vals[-1]) * delta_t / 2  #trapézio
           print(f"Energia acumulada: {energia} J")

           #armazena a leitura no gráfico
           x_vals.append(tempo_total)
           y_vals.append(potencia)

           #atualiza o gráfico
           plt.plot(x_vals, y_vals, color='green', linewidth=2.0)
           plt.fill_between(x_vals, y_vals, color='lightgreen', alpha=0.5)
           plt.xlabel("Tempo (s)", fontsize=12, fontweight='bold')
           plt.ylabel("Potência (W)", fontsize=12, fontweight='bold')
     plt.title("Potência gerada em função do tempo", fontsize=14, fontweight='bold')
           plt.show
           plt.pause(0.01)

   except KeyboardInterrupt:
       print("Interrompido pelo usuário") # por exemplo digitar ctrl + c, no terminal do VS Code mesmo
       break

ser.close()
