# 2018 Emir Erbasan (humanova)
# MIT License, see LICENSE for more details

import os,sys
import atexit
import matplotlib as mpl
import matplotlib.pyplot as plt 
import matplotlib.animation as animation
from matplotlib import style

#enable/disable recording mode
#enabling causes plotter not to
#render in realtime
RECORD_MODE = False

mpl.rcParams['toolbar'] = 'None'
style.use('dark_background')

fig = plt.figure() 
ax1 = fig.add_subplot(1,1,1)
fig.canvas.set_window_title('Neural-Net Error Graph')

#if RECORD_MODE:
    #Writer = animation.writers['ffmpeg']
    #writer = Writer(fps=30, metadata=dict(artist='humanova'), bitrate=1800)

try:
    os.chdir("Neural-Net-Settings")
except:
    print("Couldn't find 'Neural-Net-Setting' folder\n Please create one in the same directory with 'xor neural-net.exe'")

done = False

def refresh(i):
    global done

    graph_data = open('rmseErrorShort(percent).txt','r').read()
    lines = graph_data.split('\n')[:-1]

    if not done:
        xs = []
        ys = [] 
        for line in lines:
            if len(line) > 1:
                x, y = line.split(' ')
                ys.append(float(y))
                xs.append(float(x))
        
        ax1.clear()
        ax1.set_title('Neural-Net Error Graph')
        ax1.set_xlabel('Epoch' + '(' + str(xs[-1])[:-2] + ')             last error: ' + str(ys[-1])  )
        ax1.set_ylabel('Root Mean Square Error')

        ax1.plot(xs, ys)

        if xs[-1] % 100 == 0:
            plt.savefig('Neural-Net Error Graph')

        if graph_data.split('\n')[-1] == '-end-':
            done = True   

    else:
        ani.event_source.stop()


ani = animation.FuncAnimation(fig, refresh, interval = 20)

#if RECORD_MODE:
    #ani.save('Error Graph Animation.mp4', writer=writer)

plt.show()
