-----------------------------------------------------
  Instituto Tecnológico de Costa Rica
  Procesamiento y Análisis de Imágenes Digitales
  Profesor: Dr-Ing. Pablo Alvarado
  Estudiantes:
    Luis Leon
    Javier Sancho
  Tarea 4: Convolución
-----------------------------------------------------

Instrucciones de uso:

  Sintaxis:
    ./tarea4 -m MODE -s IMAGESHOW [files]
  
  Donde MODE es:
    0 para Benchmark
    1 para Divergencia de imágenes filtradas
  
  [files] es:
    Un conjunto de imágenes
  
  Y IMAGESHOW es:
    0 para no mostrar imágenes en divergencia
    1 para mostrar imágenes en divergencia

  Ejemplos de uso:
    - Para Benchmark:
      ./tarea4 images/220x220.png images/384x256.jpg images/480x361.jpg images/512x512.png images/766x395.jpg images/800x600.jpg images/960x540.png images/1166x778.jpg images/1200x800.jpg images/1920x1080.jpg
    - Para Divergencia sin imágenes:
      ./tarea4 -m 1 images/220x220.png images/384x256.jpg images/480x361.jpg 
    - Para Divergencia sin imágenes:
      ./tarea4 -m 1 -s 1 images/220x220.png images/384x256.jpg images/480x361.jpg 

Instrucciones de instalación:

  1. Cree el makefile con:
  $ cmake .
  2. Instale
  $ make
  3. Listo para usar. El ejecutable es "tarea4"
