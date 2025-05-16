#ifndef UI_JOYSTICK_H
#define UI_JOYSTICK_H

String getHTML() {
  return R"rawliteral(

<!DOCTYPE html>
<html>

  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body {
        min-height: 100vh;
        margin: 0;
        display: grid;
        place-items: center;
        background: #f8f8f8;
      }

      .layout {
        display: grid;
        grid-template-columns: 1fr;
        gap: 32px;
        align-items: center;
      }

      @media (orientation: landscape) {
        .layout {
          grid-template-columns: 1fr 1fr;
          grid-template-rows: 1fr;
        }
      }

      button {
        width: 50px;
        height: 50px;
        border-width: 0px;
        border-radius: 13px;
        font-size: 20px;
        margin: 5px;
      }

      .grid {
        display: grid;
        grid-template-columns: repeat(3, 1fr);
        grid-template-rows: repeat(3, 1fr);
        gap: 5px;
        justify-items: center;
        align-items: center;
        background: #fff;
        padding: 16px;
        border-radius: 12px;
        box-shadow: 0 2px 8px #0001;
      }

      .extras {
        display: grid;
        grid-template-columns: repeat(2, 1fr);
        grid-template-rows: repeat(2, 1fr);
        gap: 10px;
        justify-items: center;
        background: #fff;
        padding: 16px;
        border-radius: 12px;
        box-shadow: 0 2px 8px #0001;
      }

      @media (orientation: landscape) {
        .extras {
          grid-template-columns: repeat(2, 1fr);
          grid-template-rows: repeat(2, 1fr);
        }
      }
    </style>
  </head>

  <body>
    <h2 style="text-align:center;">Joystick ESP32</h2>
    <div class="layout">
      <div class="grid">
        <div></div>
        <button onclick="enviar('adelante')">&#x2B06;</button>
        <div></div>

        <button onclick="enviar('izquierda')">&#x2B05;</button>
        <button onclick="enviar('stop')">&#x23F9;</button>
        <button onclick="enviar('derecha')">&#x27A1;</button>

        <div></div>
        <button onclick="enviar('atras')">&#x2B07;</button>
        <div></div>
      </div>

      <div class="extras">
        <button onclick="enviar('btn1')">B1</button>
        <button onclick="enviar('btn2')">B2</button>
        <button onclick="enviar('btn3')">B3</button>
        <button onclick="enviar('btn4')">B4</button>
      </div>
    </div>

    <script>
      function enviar(comando) {
        fetch(`/cmd?dir=${comando}`);
      }
    </script>
  </body>
  
</html>

  )rawliteral";
}

#endif