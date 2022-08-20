# thingspeak_essicatore_01.py

import matplotlib.pyplot as plt
import pandas as pd
import datetime as dt
import matplotlib.dates as mdates
import matplotlib.ticker as ticker
 

import easygui

'''
conda activate p37
'''



plt.style.use('seaborn')

#url="https://api.thingspeak.com/channels/1612409/feeds.csv?days=10"


days= easygui.integerbox(msg='Giorni da analizzare', title='Giorni', default=1, lowerbound=0, upperbound=120, image=None, root=None)


url="https://api.thingspeak.com/channels/1612410/feeds.csv?days="+str(days)



data = pd.read_csv(url)

# cambia tipo data
data.loc[:,'created_at']=pd.to_datetime(data['created_at'], utc=True).dt.tz_convert('Europe/Rome')
'''
data['created_at']=pd.to_datetime(data['created_at'], utc=True)
data = data.set_index('created_at').tz_convert('Europe/Rome')
'''
#print (data.dt.tz)
#print (data['created_at'].tail())
#print (data.tail())

# filtra dati
#data = data.loc[(data['created_at'] >= '2022-06-19 00:00') & (data['created_at'] < '2022-07-06 00:00')]

# cambia nomi e setta data come indice
new_names = {
    'created_at' : 'giorno_ora',
    'field1': 'Temperatura A',
    'field2': 'Umidità A',
    'field3': 'Segnale Wifi A',
    'field4': 'Tensione A',
    'field5': 'Temperatura 1 B',
    'field6': 'Temperatura 2 B',
    'field7': 'Umidità B',
    'field8': 'Temperatura Amb B',
}

data = data.rename(columns=new_names)
data = data.set_index('giorno_ora')

#data1=data[['Temperatura 1 B', 'Temperatura 2 B', 'Umidità B', 'Temperatura Amb B']].dropna().copy()
data1=data[['Temperatura 1 B', 'Temperatura 2 B', 'Umidità B']].dropna().copy()

'''
print ( data1.head() )
print ( data1.tail() )
'''

# genera plot
fig, ax = plt.subplots(nrows=2, figsize=[20,12])

# primo plot
data1[['Temperatura 1 B']].plot(ax=ax[0])
data1[['Temperatura 2 B']].plot(ax=ax[0])
#data1[['Temperatura Amb B']].plot(ax=ax[0])
# secondo plot
data1[['Umidità B']].plot(ax=ax[1])

ax[0].set_title('Temperature Incubatrice')
ax[1].set_title('Umiditá Relativa Incubatrice')

ax[0].set(xlabel='Giorno - Ora', ylabel='°C')
ax[1].set(xlabel='Giorno - Ora', ylabel='%')

# setta il formato delle data come vuoi su tutti gli assi
# NOTA = _ é una variabile che viene cestinata da python
plt.rcParams['timezone'] = 'Europe/Rome'
_ = [a.xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M")) for a in ax]
# plt.gcf().autofmt_xdate() # serve?
plt.tight_layout()
plt.show()

