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

days= easygui.integerbox(msg='Giorni da analizzare', title='Giorni', default=5, lowerbound=0, upperbound=120, image=None, root=None)

url="https://api.thingspeak.com/channels/1612407/feeds.csv?days="+str(days)


data = pd.read_csv(url)


# cambia tipo data
data['created_at']=pd.to_datetime(data['created_at'])


# filtra dati
#data = data.loc[(data['created_at'] >= '2022-06-19 00:00') & (data['created_at'] < '2022-07-06 00:00')]

# cambia nomi e setta data come indice
new_names = {
    'created_at' : 'giorno_ora',
    'field1': 'Ingresso1',
    'field4': 'Ingresso2',
}


data = data.rename(columns=new_names)
data = data.set_index('giorno_ora')


data1=data[['Ingresso2']].dropna().copy()
data2=data[['Ingresso1']].dropna().loc[data['Ingresso1'] < 1e6 ].copy()

#print(data['Ingresso1'].min()," - ", data['Ingresso1'].max()  )

'''
print(type(data))
print(type(data2))
'''

#data3=data2.loc[data2['Ingresso1'] < 24000 ].copy()

#print(type(data2))
#data3 = data3.set_index('giorno_ora')


plt.plot(data1)
plt.gca().xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M"))
plt.tight_layout()
plt.show()
plt.plot(data2)
plt.gca().xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M"))
plt.tight_layout()
plt.show()

'''
data1.plot()
data2.plot()
#data3.plot()


data1['Ingresso2']=data['Ingresso2'].dropna()
data1['Ingresso1']=data['Ingresso1'].dropna()
data1['Ingresso2'].plot()
data1['Ingresso1'].plot()
'''






'''
# genera plot
fig, ax = plt.subplots(nrows=2, figsize=[20,12])
# primo plot : temps
data[['Ingresso1']].plot(ax=ax[0])
# secondo plot
data[['Ingresso2']].plot(ax=ax[1])


# setta il formato delle data come vuoi su tutti gli assi
# NOTA = _ é una variabile che viene cestinata da python
_ = [a.xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d %H:%M")) for a in ax]
# plt.gcf().autofmt_xdate() # serve?
plt.tight_layout()
plt.show()
'''

