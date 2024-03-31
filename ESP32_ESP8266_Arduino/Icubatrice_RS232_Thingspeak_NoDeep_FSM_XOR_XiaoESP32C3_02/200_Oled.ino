void oled(){
    u8x8.setFont(u8x8_font_8x13B_1x2_r);
    u8x8.setCursor(0, 0);
    u8x8.print("TMP: SERBATOIO");
    u8x8.setFont(u8x8_font_inr33_3x6_f);
    u8x8.setCursor(2, 2);
    u8x8.print(String(Messaggio[6]));
    u8x8.print("'C");
    delay (3000);
    oled_clean();
    
    u8x8.setFont(u8x8_font_8x13B_1x2_r);
    u8x8.setCursor(0, 0);
    u8x8.print("TMP: INTERNA");
    u8x8.setFont(u8x8_font_inr33_3x6_f);
    u8x8.setCursor(2, 2);
    u8x8.print(String(Messaggio[7]));
    u8x8.print("°C");
    delay (3000);
    oled_clean();

    u8x8.setFont(u8x8_font_8x13B_1x2_r);
    u8x8.setCursor(0, 0);
    u8x8.print("TMP: ESTERNA");
    u8x8.setFont(u8x8_font_inr33_3x6_f);
    u8x8.setCursor(2, 2);
    u8x8.print(String(Messaggio[9]));
    u8x8.print("'C");
    delay (3000);
    oled_clean();

    u8x8.setFont(u8x8_font_8x13B_1x2_r);
    u8x8.setCursor(0, 0);
    u8x8.print("   % UMIDITA");
    u8x8.setFont(u8x8_font_inr33_3x6_r);
    u8x8.setCursor(3, 2);
    u8x8.print(String(Messaggio[8])+("%"));
    delay (3000);
    oled_clean();
    
    u8x8.setFont(u8x8_font_8x13B_1x2_r);
    u8x8.setCursor(0, 3);
    u8x8.print("INCUBATOR-RETE");
}


void oled_clean(){
  u8x8.clearDisplay();
}
