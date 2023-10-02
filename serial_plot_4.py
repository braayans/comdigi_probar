"""
Usar distribucion de python de anaconda, ademas, instalar pyserial a esta distribucion
    - Para instalar Pyserial --> abrir powershell (Windows) y usar comando:  conda install -c main pyserial 
"""

import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque
import sys

# crear objeto serial
try:
    serialPort = serial.Serial(port = 'COM7', baudrate = 9600)
except:
    print('No se puede abrir el puerto serie.')
    sys.exit()


# crear ventana para mostrar la gráfica
fig, ax = plt.subplots()
line, = ax.plot([], [], 'ro', markersize = 3)

# datos de la gráfica
x_data = deque(maxlen = 200)
y_data = deque(maxlen = 200)

# inicializar la gráfica
def init():
    ax.set_xlim(0, 200)
    ax.set_ylim(0, 1023)
    return line,

# función que actualiza la gráfica en cada iteración
def update(frame):
    # leer dato desde puerto serie
    try:
        data = serialPort.readline().decode().strip()
        y = int(data)
    except:
        y = 0
        
    # añadir dato a la cola
    y_data.append(y)
    x_data = range(len(y_data))

    # actualizar datos de la gráfica
    line.set_data(x_data, y_data)

    return line,



# animación de la gráfica
ani = FuncAnimation(fig, update, frames=None, init_func=init, blit=True, interval=0.01)

# mostrar ventana con la gráfica
plt.title('Datos desde Puerto Serie')
plt.xlabel('Tiempo')
plt.ylabel('Magnitud')
plt.show()

# cerrar el puerto serie al cerrar la ventana
serialPort.close()
