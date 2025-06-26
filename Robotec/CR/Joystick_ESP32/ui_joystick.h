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
        width: 100vw;
        max-width: 100vw;
        padding: 0 10vw;
        box-sizing: border-box;
      }

      @media (orientation: landscape) {
        .layout {
          grid-template-columns: 1fr 1fr;
          grid-template-rows: 1fr;
          padding: 0 5vw;
        }
      }

      button {
        width: 70px;
        height: 70px;
        border-width: 0px;
        border-radius: 13px;
        font-size: 28px;
        margin: 8px;
        transition: background 0.2s, box-shadow 0.2s, color 0.2s;
      }

      button:active, button.pressed {
        background: #b3d1ff;
        box-shadow: 0 0 0 2px #1976d2;
        color: #1976d2;
      }

      .grid {
        display: grid;
        grid-template-columns: repeat(3, 1fr);
        grid-template-rows: repeat(3, 1fr);
        gap: 8px;
        justify-items: center;
        align-items: center;
        background: #fff;
        padding: 8px 0;
        border-radius: 12px;
        box-shadow: 0 2px 8px #0001;
        width: 100%;
        max-width: 350px;
      }

      .extras {
        display: grid;
        grid-template-columns: repeat(2, 1fr);
        grid-template-rows: repeat(2, 1fr);
        gap: 12px;
        justify-items: center;
        background: #fff;
        padding: 8px 0;
        border-radius: 12px;
        box-shadow: 0 2px 8px #0001;
        width: 100%;
        max-width: 350px;
      }

      /* CSS para disposición en cruz de los botones extras */
      .extras.cruz {
        display: grid;
        grid-template-areas:
          ". arriba ."
          "izquierda . derecha"
          ". abajo .";
        grid-template-columns: 1fr 1fr 1fr;
        grid-template-rows: 1fr 1fr 1fr;
        gap: 12px;
        justify-items: center;
        align-items: center;
        background: #fff;
        padding: 8px 0;
        border-radius: 12px;
        box-shadow: 0 2px 8px #0001;
        width: 100%;
        max-width: 350px;
      }

      .btn-ps {
        width: 70px;
        height: 70px;
        font-size: 32px;
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
    <div class="layout">
      <div class="grid">
        <div></div>
        <button onpointerdown="enviar('adelante')" onpointerup="enviar('stop')" onpointerleave="enviar('stop')">&#x2B06;</button>
        <div></div>

        <button onpointerdown="enviar('izquierda')" onpointerup="enviar('stop')" onpointerleave="enviar('stop')">&#x2B05;</button>
        <button onpointerdown="enviar('stop')">&#x23F9;</button>
        <button onpointerdown="enviar('derecha')" onpointerup="enviar('stop')" onpointerleave="enviar('stop')">&#x27A1;</button>

        <div></div>
        <button onpointerdown="enviar('atras')" onpointerup="enviar('stop')" onpointerleave="enviar('stop')">&#x2B07;</button>
        <div></div>
      </div>

      <div class="extras cruz">
        <button class="btn-ps" style="grid-area: arriba;" onpointerdown="enviar('btn1')" onpointerup="enviar('stop')" onpointerleave="enviar('stop')">&#x25B3;</button> <!-- Triángulo -->
        <button class="btn-ps" style="grid-area: izquierda;" onpointerdown="enviar('btn2')" onpointerup="enviar('stop')" onpointerleave="enviar('stop')">&#x25A0;</button> <!-- Cuadrado -->
        <button class="btn-ps" style="grid-area: derecha;" onpointerdown="enviar('btn3')" onpointerup="enviar('stop')" onpointerleave="enviar('stop')">&#x25CF;</button> <!-- Círculo -->
        <button class="btn-ps" style="grid-area: abajo;" onpointerdown="enviar('btn4')" onpointerup="enviar('stop')" onpointerleave="enviar('stop')">&#x2716;</button> <!-- Cruz -->
      </div>
    </div>

    <script>
      function enviar(comando) {
        console.log(comando);
        fetch(`/cmd?dir=${comando}`);
      }
      // Retroalimentación visual para botones presionados
      document.querySelectorAll('button').forEach(btn => {
        btn.addEventListener('pointerdown', function() {
          btn.classList.add('pressed');
        });
        btn.addEventListener('pointerup', function() {
          btn.classList.remove('pressed');
        });
        btn.addEventListener('pointerleave', function() {
          btn.classList.remove('pressed');
        });
        btn.addEventListener('pointercancel', function() {
          btn.classList.remove('pressed');
        });
      });
    </script>
  </body>
</html>

  )rawliteral";
}
#endif
