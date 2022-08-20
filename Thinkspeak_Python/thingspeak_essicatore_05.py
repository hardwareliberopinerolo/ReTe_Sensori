# thingspeak_essicatore_01.py

import matplotlib.pyplot as plt
import pandas as pd
import matplotlib.dates as mdates
import matplotlib.ticker as ticker
import easygui

plt.style.use('seaborn')

days= easygui.integerbox(msg='Giorni da analizzare', title='Essicatore', default=5, lowerbound=0, upperbound=120, image=None, root=None)
url="https://api.thingspeak.com/channels/1612409/feeds.csv?days=" +str(days)
data = pd.read_csv(url)

# cambia tipo data
'''
data['created_at']=pd.to_datetime(data['created_at'])
print (data['created_at'].dt.tz)
'''

#data['created_at']=pd.to_datetime(data['created_at'], utc=True).dt.tz_convert('Europe/Rome')

data.loc[:,'created_at']=pd.to_datetime(data['created_at'], utc=True).dt.tz_convert('Europe/Rome')
#print (data['created_at'].dt.tz)
#print (data['created_at'])


# filtra dati
#data = data.loc[(data['created_at'] >= '2022-06-19 00:00') & (data['created_at'] < '2022-07-06 00:00')]

# cambia nomi e setta data come indice
new_names = {
    'created_at' : 'giorno_ora',
    'field1': 'Temperatura Ventole',
    'field2': 'Temperatura Esterna',
    'field3': 'Temperatura Interna',
    'field4': 'Umiditá Relativa',
    'field7':'Tensione'
}

data = data.rename(columns=new_names).set_index('giorno_ora')

data['DeltaT']= data['Temperatura Interna'] - data['Temperatura Esterna']



message = "Valore minimo:" + str(round(data['DeltaT'].min(),1))+"\n \n" + "Valore Max:" + str(round(data['DeltaT'].max(),1))+ "\n \n" +"Temperatura max interna:" +str(round(data['Temperatura Interna'].max(),1))+"\n \n" +"Temperatura min interna:" +str(round(data['Temperatura Interna'].min(),1))+"\n \n" + "Temperatura max esterna:" +str(round(data['Temperatura Esterna'].max(),1))+"\n \n" + "Umidità Interna minima:" + str(round(data['Umiditá Relativa'].min(),1))+"\n \n"

title = " Delta Temperature essicatore"

easygui.msgbox(message, title)


url1="https://api.thingspeak.com/channels/1578204/feeds.csv?days="+str(days)

data1 = pd.read_csv(url1)

# cambia tipo data

data1.loc[:,'created_at']=pd.to_datetime(data1['created_at'], utc=True).dt.tz_convert('Europe/Rome')
# filtra dati
#data = data.loc[(data['created_at'] >= '2022-06-19 00:00') & (data['created_at'] < '2022-07-06 00:00')]




# cambia nomi e setta data come indice
new_names1 = {
    'created_at' : 'giorno_ora',
    'field1': 'Temperatura Esterna',
    'field2': 'Temperatura Scatola',
    'field3': 'Segnale Wifi',
    'field4': 'Tensione',
}


data1 = data1.rename(columns=new_names1).set_index('giorno_ora')

'''
data1['DeltaT']= data['Temperatura Interna'] - data1['Temperatura Esterna']

print(data1['DeltaT'])
'''

# genera plot

fig, ax = plt.subplots(nrows=3,figsize=[20,12])

#fig, ax = plt.subplots(nrows=2)


# primo plot : temps
data.filter(like='Temperatura',axis=1).plot(ax=ax[0])

data[['DeltaT']].plot(ax=ax[0])

# secondo plot
data[['Umiditá Relativa']].plot(ax=ax[1])
# terzo plot
data[['Tensione']].plot(ax=ax[2])

#data1.filter(like='Temperatura',axis=1).plot(ax=ax[0])

# setta il formato delle data come vuoi su tutti gli assi
# NOTA = _ é una variabile che viene cestinata da python
plt.rcParams['timezone'] = 'Europe/Rome'
_ = [a.xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M")) for a in ax]
# plt.gcf().autofmt_xdate() # serve?
plt.tight_layout()
plt.show()




fig, ax = plt.subplots(figsize=[20,12])

ax.plot(data['Temperatura Interna'], label="Temperatura Essicatore")
ax.plot(data1['Temperatura Scatola'], label="Temperatura Scatola")

plt.legend()
plt.title("Temperature")
plt.xlabel("Giorno - Ora ")
plt.ylabel("Temperatura °C")


plt.rcParams['timezone'] = 'Europe/Rome'
plt.gca().xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M"))
plt.grid(True)
plt.tight_layout()
plt.show()


fig, ax = plt.subplots(nrows=2,figsize=[20,12], sharex=True)

ax[0].plot(data['Temperatura Interna'])
ax[1].plot(data['Umiditá Relativa'])

ax[0].set_title('Temperatura Essicatore')
ax[1].set_title('Umiditá Relativa Essicatore')

ax[0].set(xlabel='Giorno - Ora', ylabel='°C')
ax[1].set(xlabel='Giorno - Ora', ylabel='%')
plt.rcParams['timezone'] = 'Europe/Rome'
_ = [a.xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M")) for a in ax]
plt.grid(True)
plt.tight_layout()
plt.show()

