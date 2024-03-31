//---State_Machine-------------------------------------------------
void State_Machine(){


if (stato != old_stato){

  Serial.print("Stato: ");
  Serial.println(stato);
old_stato=stato;
}

funzionestato[stato]();


}
