const char paginaHTML[] = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <title>Arduino Ajax I/O</title>
    <script>
      strIntervMod="";
      function GetArduinoIO(){
        nocache = "&nocache=" + Math.random() * 100000;
        var request = new XMLHttpRequest();
        request.onreadystatechange = function(){    //Evento "onreadystatechange"
            if (this.readyState == 4) {  //request finito, rispota "ready"
                if (this.status == 200) {  //OK.
                    if (this.responseXML != null) {
                        var t_att_letture=this.responseXML.getElementsByTagName("tempi")[0].childNodes[0].nodeValue;
                        document.getElementById("intervAtt").value = t_att_letture;
                    }
                }    
            }
        } 
        // Invio richiesta HTTP GET con lo stato delle uscite.
        request.open("GET", "comandi?"+strIntervMod+  nocache,true);
        request.send(null);
        setTimeout('GetArduinoIO()', 250);
        strIntervMod="";
      }
      //---------------------------
      function invio(){
          var v_intervMod  = document.getElementById("intervMod").value;
          if (v_intervMod<1 || v_intervMod>230){
              alert(" " + v_intervMod +" Intervallo fuori range, valido 1-230 minuti");
               return;
          }
          strIntervMod = "&intervMod=" + v_intervMod;
      }   
    </script>
    <style>
      .titolo {
        background-color:yellow;
        font-size: 32;
        margin: 0px 0px 10px 0px;
        border: 10px solid red;
        padding: 10px 0px 2px 10px;
        width: 400px;
        height:50;
        color: blue;
      }
      .PUL {
        border-radius: 50px;    
        font-size: 200%;
        height: 60px;
        width: 250px;
        margin: 4px 0px 0px 25px;
        padding: 1px 0px 5px 0px;
        background-color:cyan;
        color: blue;
      }
      .l_uno{
        font-size: 22px;    
      } 
      .t_uno{
        height:40px;  
        width:80px;
        font-size: 22px;   
      } 
      .L_3{
        font-size:22px;
        background-color:peru;
        color:white;
         text-align: center;
        width: 360px;
        height:50px;
      } 
      .param{
         background-color:chartreuse; 
         height:32px; 
         width:100px;
          font-size: 22px; 
      }       
    </style>
  </head>
  <body onload="GetArduinoIO()">
    <br>
    <h1>&emsp;&emsp;&emsp;&emsp;&emsp;L' ortolano B</h1>
    <table border='1px'>
      <tr style="background-color:sienna;">
        <td colspan="3 "align="center">  
        <div class="titolo">
            Controllo temperature
          </div>  
        </td>
      </tr>
      <tr style="background-color:aquamarine;">
      <td colspan="3" style="text-align: center;font-size:22px;">Parametri</td>
      </tr>
      <tr style="background-color:aquamarine;">
        <td></td>
        <td>
            <input type="text"  style="text-align:center;background-color:chartreuse" value="Attuale" size="7">
        </td>
        <td>
            <input type="text"  style="text-align:center;background-color:chartreuse" value="Imposta" size="7">
        </td>    
      </tr>
      <tr style="background-color:aquamarine;"> 
        <td style="text-align:right;font-size:22px" >Frequenza letture min. </td>
        <td>
            <input type="text" id="intervAtt"  class="param" size="5">
            </td>
          <td>
              <input type="text" id="intervMod"class="param"  size="5" >
          </td>
      </tr>
      <tr>    
       <td colspan="3" style="background-color:Wheat;text-align:center">    
          <button type="button" class="PUL"  onclick="invio();">lnvio</button>
       </td>
      </table> 
   </body>
</html>
)=====";
