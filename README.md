# Asteroids_ESAT
Versión del videojuego arcade clásico de Atari Asteroids desarrollada por Jonathan Martínez Navarro a modo de proyecto de práctica para Escuela Superior de Arte y Tecnología (ESAT) bajo la tutela de Federico Sanjuan.

-- Trata de ser lo mas fiel posible al original en cuanto a gameplay junto a manejo local de usuarios, guardado de partidas sin finalizar, power-ups y modos de juego multijugador.

-- Dispone de 4 modos de juego:
  -- Un jugador
  -- Multijugador Alternando
  -- Multijugador Simultaneao Versus
  -- Multijugador Simultaneao Cooperativo

-- Funcionamiento óptimo a 60fps.

-- Consumo medio de memoria RAM -> 36MB


CONTROLES:

  - En cualquier momento

      ESC = Cerrar juego

  - En menús
      Control con el cursor del raton
      - Apuntar              Seleccionar elemento del menú 
      - Click izquierdo      (MB1) Confirmar selección

      Control con teclado
      - Flecha arriba  (🡩) Seleccionar elemento del menú anterior
      - Flecha abajo   (🡫) Seleccionar elemento del menú siguiente
      - Tabulador      (⮀) Seleccionar elemento del menú siguiente
      - Enter          (⏎) Confirmar selección

  - En gameplay
      - M     Volver al menú

        **Jugador 1:**
    - W          Acelerar
    - A          Girar a izquierda
    - D          Girar a derecha
    - SpaceBar   Disparar
    - G          Salto Hyperspacial

        **Jugador 2:**
    - Flecha Arriba       (🡩)  Acelerar
    - Flecha Izquierda    (**🡐**)  Girar a izquierda
    - Flecha Derecha      (➔)  Girar a derecha
    - Enter               (⏎)  Disparar
    - P                    Salto Hyperspacial
       


Para descargarlo y jugarlo directamente, se puede utilizar una de las versiones de "Release" ubicadas al lado derecho del proyecto en la página de <a href="https://github.com/martineznav-ESAT/Asteroids_ESAT_JMN" target="_blank">github</a>. 

Estas versiones consisten de un ".exe" ya compilado junto a la carpeta de "Assets" con lo necesario para su ejecución.

Si deseas descargar el código fuente y compilarlo por tu cuenta, una vez compilado la ejecución será la misma, pero necesitaras del ""Developer Command Prompt for VS 2022"" con acceso a compilacion de C++ mediante el comando "cl" para compilar.

Sin embargo, si se descarga todo el contenido de la rama correctamente, compilar es tan simple como ejecutar el comando "compiler.bat main.cc" desde la ubicación del código fuente tal y como se muestra en la siguiente imagen:

<img width="614" height="81" alt="image" src="https://github.com/user-attachments/assets/9c8bc913-9828-43b8-8966-d972c91f1a6f" />


Compilación correcta:

<img width="425" height="607" alt="image" src="https://github.com/user-attachments/assets/9ca77698-5f83-47b2-9c13-f59b24e098e3" />



Una vez compilado, debería generar un archivo “main.exe” junto a otros archivos más como se ve en la imagen inferior:	

<img width="889" height="582" alt="image" src="https://github.com/user-attachments/assets/2c92d024-33ee-4610-a7e1-71ff29757046" />


Los archivos de guardado se ubican en ./Assets/Files/Data. Si son eliminados se generarán nuevos en la próxima ejecución.
Estos archivos incluyen:
  - La lista de partidas (game_list.dat)
  - La lista de las mejores 10 puntuaciones (highscores.dat)
  - La lista de usuarios registrados (user_list.dat)
<img width="699" height="180" alt="image" src="https://github.com/user-attachments/assets/bd74feb7-3e5b-45d2-b9a0-93635694b613" />

Se debe tener en cuenta que las partidas van relacionadas con los usuarios, por lo que si se elimina la lista de usuarios, lo mas probable es que la lista de partidas y puntuaciones máximas pase a ser inutilizable.

Si se detecta que no hay usuarios registrados, la primera pantalla al abrir el juego se abrirá en modo registro donde se permitirá crear al primer usuario que a su vez será administrador

<img width="1000" height="801" alt="image" src="https://github.com/user-attachments/assets/5a77f3ed-6e4b-4e46-a2b7-f4c87baa38e3" />



**Bibliografía**


The FontStruction “Asteroids Display”
(https://fontstruct.com/fontstructions/show/1534627) by John Harris is licensed
under a Creative Commons Attribution license
(http://creativecommons.org/licenses/by/3.0/).
[ancestry]

https://www.dafont.com/es/neuropol.font
