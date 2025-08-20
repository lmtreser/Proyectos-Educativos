let colores = [];
let botonCambio, botonSonido, botonMover;
let sliderTam, sliderVel;
let texto = "Hola 👋";
let sonido;
let mover = false;

let img;           // variable para la imagen
let imgX, imgY;    // posición de la imagen
let offsetX, offsetY;
let arrastrando = false;

function preload() {
  sonido = loadSound("sonido.mp3");
  img = loadImage("heman.jpg");
}

function setup() {
  createCanvas(600, 400).parent(document.body);

  // Colores iniciales
  for (let i = 0; i < 6; i++) {
    colores.push(color(random(255), random(255), random(255)));
  }

  botonCambio = createButton("Cambiar Colores");
  botonCambio.mousePressed(cambiarColores);
  botonCambio.position(windowWidth / 2 - 250 , 400);
  botonCambio.size(120, 50);
  botonCambio.style('background-color', '#FF5733');
  botonCambio.style('color', 'white');
  botonCambio.style('font-size', '18px');
  botonCambio.style('border-radius', '12px');
  botonCambio.style('border', '2px solid black');
  
  botonCambio.style('transition', '0.3s');
  botonCambio.mouseOver(() => botonCambio.style('background-color', '#FFC300'));
  botonCambio.mouseOut(() => botonCambio.style('background-color', '#FF5733'));

  botonSonido = createButton("Reproducir Sonido 🎵");
  botonSonido.mousePressed(() => {
    if (sonido.isPlaying()) sonido.stop();
    sonido.play();
  });

  botonSonido.position(windowWidth / 2 - 60 , 400);
  botonSonido.size(120, 50);
  botonSonido.style('background-color', '#33ffccff');
  botonSonido.style('color', 'black');
  botonSonido.style('font-size', '18px');
  botonSonido.style('border-radius', '12px');
  botonSonido.style('border', '2px solid black');

  botonSonido.style('transition', '0.3s');
  botonSonido.mouseOver(() => botonSonido.style('background-color', '#66ffcc'));
  botonSonido.mouseOut(() => botonSonido.style('background-color', '#3385ffff'));

  createP("");
  botonMover = createButton("Mover figuras");
  botonMover.mousePressed(() => (mover = !mover));

  botonMover.position( (windowWidth / 2) + 130 , 400);
  botonMover.size(120, 50);
  botonMover.style('background-color', '#63ff33ff');
  botonMover.style('color', 'black');
  botonMover.style('font-size', '18px');
  botonMover.style('border-radius', '12px');
  botonMover.style('border', '2px solid black');

  botonMover.style('transition', '0.3s');
  botonMover.mouseOver(() => botonMover.style('background-color', '#63ff33ff'));
  botonMover.mouseOut(() => botonMover.style('background-color', '#ff3377ff'));

  createP("Tamaño de cuadros:");
  sliderTam = createSlider(20, 150, 60, 5);
  
  createP("Velocidad de animación:");
  sliderVel = createSlider(1, 20, 5, 1);

  imgX = width / 2 - img.width / 2;
  imgY = height / 2 - img.height / 2;

}

function draw() {
  background(240);

  image(img, imgX, imgY, 100, 100);

  let tam = sliderTam.value();
  let vel = sliderVel.value();

  // Dibujar cuadros
  for (let i = 0; i < colores.length; i++) {
    fill(colores[i]);
    let x = 100 + i * 70;
    let y = height / 2 + (mover ? sin(frameCount * 0.05 * vel + i) * 40 : 0);
    rect(x, y, tam, tam, 15);
  }

  // Texto
  textAlign(CENTER, CENTER);
  textSize(24);
  fill(50);
  text(texto, width / 2, 40);
}

// Cambiar colores (para botón o toque)
function cambiarColores() {
  for (let i = 0; i < colores.length; i++) {
    colores[i] = color(random(255), random(255), random(255));
  }
}

// detectar si clickeaste la imagen
function mousePressed() {
  if (mouseX > imgX && mouseX < imgX + img.width &&
      mouseY > imgY && mouseY < imgY + img.height) {
    arrastrando = true;
    offsetX = mouseX - imgX;
    offsetY = mouseY - imgY;
  }
}

// soltar la imagen
function mouseReleased() {
  arrastrando = false;
}

// arrastrar la imagen
function mouseDragged() {
  if (arrastrando) {
    imgX = mouseX - offsetX;
    imgY = mouseY - offsetY;
  }
}