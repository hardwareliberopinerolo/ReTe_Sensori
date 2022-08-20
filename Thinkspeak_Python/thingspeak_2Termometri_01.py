# thingspeak_essicatore_01.py

import matplotlib.pyplot as plt
import pandas as pd
import matplotlib.dates as mdates
import matplotlib.ticker as ticker
 

import easygui

'''
conda activate p37
'''



plt.style.use('seaborn')

#url="https://api.thingspeak.com/channels/1612409/feeds.csv?days=10"


days= easygui.integerbox(msg='Giorni da analizzare', title='Giorni', default=5, lowerbound=0, upperbound=120, image=None, root=None)


url="https://api.thingspeak.com/channels/1578204/feeds.csv?days="+str(days)



data = pd.read_csv(url)

# cambia tipo data
data['created_at']=pd.to_datetime(data['created_at'])


# filtra dati
#data = data.loc[(data['created_at'] >= '2022-06-19 00:00') & (data['created_at'] < '2022-07-06 00:00')]

# cambia nomi e setta data come indice
new_names = {
    'created_at' : 'giorno_ora',
    'field1': 'Temperatura Esterna',
    'field2': 'Temperatura Scatola',
    'field3': 'Segnale Wifi',
    'field4': 'Tensione',
}

data = data.rename(columns=new_names).set_index('giorno_ora')

data['deltaT']= data['Temperatura Scatola'] - data['Temperatura Esterna']

# genera plot
fig, ax = plt.subplots(nrows=3, figsize=[20,12])
# primo plot : temps
data.filter(like='Temperatura',axis=1).plot(ax=ax[0])

data[['deltaT']].plot(ax=ax[0])


# secondo plot
data[['Segnale Wifi']].plot(ax=ax[1])
# terzo plot
data[['Tensione']].plot(ax=ax[2])
plt.rcParams['timezone'] = 'Europe/Rome'
# setta il formato delle data come vuoi su tutti gli assi
# NOTA = _ é una variabile che viene cestinata da python
_ = [a.xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M")) for a in ax]
# plt.gcf().autofmt_xdate() # serve?
plt.tight_layout()
plt.show()



fig, ax = plt.subplots(figsize=[20,12])


#data.filter(like='Temperatura',axis=1).plot()

ax.plot(data['deltaT'], label="DeltaT")
ax.plot(data['Temperatura Esterna'], label="Temperatura Esterna")
ax.plot(data['Temperatura Scatola'], label="Temperatura Scatola")

plt.legend()
plt.title("Temperature")
plt.xlabel("Giorno - Ora ")
plt.ylabel("Temperatura °C")
plt.grid(True)


plt.gca().xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M"))
plt.tight_layout()
plt.show()



#fig, ax = plt.subplots(nrows=1, figsize=[20,12])


#data[['Tensione']].plot(ax=ax[0])
data[['Tensione']].plot()

# setta il formato delle data come vuoi su tutti gli assi
plt.gca().xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M"))


# NOTA = _ é una variabile che viene cestinata da python
#_ = [a.xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M")) for a in ax]
#plt.gcf().autofmt_xdate() # serve?
plt.tight_layout()
plt.show()


