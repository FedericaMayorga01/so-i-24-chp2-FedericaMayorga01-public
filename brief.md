# Preguntas y respuestas sobre el sistema de archivos /proc y métricas en Linux

1. **¿Dónde se encuentran los pipes en el sistema de archivos y qué atributos tienen?**

    Los pipes (anónimos) no aparecen directamente en el sistema de archivos, ya que existen solo en memoria mientras los procesos que los usan están vivos. En cambio, los pipes con nombre (named pipes o FIFOs) sí aparecen como archivos especiales en el sistema de archivos.

    Se pueden crear con:
    ```bash
    mkfifo /tmp/mipipe
    ```

    Luego aparecen en el FS como tipo `p` (pipe) si listás con `ls -l`, indicando que es un FIFO:
    ```bash
    prw-r--r-- 1 usuario grupo 0 fecha /tmp/mipipe
    ```
    Internamente, el kernel los representa como estructuras de buffer circular para la comunicación entre procesos.


2. **¿Qué mecanismos de IPC existen en Linux y en qué situaciones es apropiado usar cada uno?**

    Linux tiene varios mecanismos de Inter-Process Communication (IPC):

    - **Pipes (anónimos):** Comunicación unidireccional entre procesos relacionados (padre-hijo).
    - **Pipes con nombre (FIFOs):** Comunicación unidireccional entre procesos no relacionados.
    - **Sockets:** Comunicación bidireccional entre procesos. Pueden ser locales (Unix domain) o de red (TCP/UDP).
    - **Shared Memory:** Área de memoria compartida entre procesos.
    - **Message Queues:** Envío de mensajes estructurados entre procesos.
    - **Semáforos:** Controlan acceso concurrente a recursos compartidos.
    - **Signals:** Señales para notificar eventos (como interrupciones).


3. **¿Cómo funcionan los pipes con nombre (named pipes) y en qué se diferencian de los pipes anónimos?**

    **Pipes anónimos:**

    - Se crean con `pipe()`.
    - Son temporales y existen solo en memoria, entre procesos con relación padre-hijo (típicamente).
    - No aparecen en el sistema de archivos.

    **Named pipes (FIFOs):**

    - Se crean con `mkfifo()` o el comando `mkfifo`.
    - Se acceden como archivos en el sistema (`/tmp/pipe`, por ejemplo).
    - Permiten comunicación entre procesos independientes.
    - Bloquean si no hay un lector o escritor conectado.


4. **¿Cuál es el rol de CMake en la gestión de proyectos y cómo facilita la compilación en múltiples plataformas?**

    CMake es una herramienta de generación de scripts de construcción (build). Su función principal es generar archivos que sistemas como make, ninja, Visual Studio, etc., puedan usar para compilar tu código.

    Facilita la compilación en múltiples plataformas por varias razones:
    - Es multiplataforma, lo mismo corre en Linux, Windows, macOS.
    - Gestión automática de dependencias.
    - Puedes crear configuraciones de debug/release fácilmente.
    - Permite definir flags de compilación, tests, instalación, etc.


5. **¿Cómo se pueden especificar y gestionar las dependencias externas en un proyecto usando CMake?**

    En CMake, las dependencias externas se gestionan principalmente a través de:

    - **find_package:** Busca e incluye bibliotecas instaladas en el sistema.
      ```cmake
      find_package(LibName REQUIRED)
      ```

    - **add_subdirectory:** Incluye subdirectorios que contienen otros proyectos CMake.
      ```cmake
      add_subdirectory(external/libname)
      ```

    - **FetchContent:** Descarga y construye proyectos automáticamente desde repositorios remotos.
      ```cmake
      include(FetchContent)
      FetchContent_Declare(
      cJSON
      GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
      )
      FetchContent_MakeAvailable(cJSON)
      ```
    - **Target Link Libraries:** Vincula bibliotecas a tu ejecutable o biblioteca.
      ```cmake
      target_link_libraries(my_target PRIVATE LibName)
      ```


6. **¿Qué es el framework de testing Unity en C y cómo se integra en un proyecto para realizar pruebas unitarias?**

    Unity es un framework de testing para C que permite escribir y ejecutar pruebas unitarias de manera sencilla. Se integra en un proyecto de la siguiente manera:

    1. Clonás Unity o lo copiás en `external/unity`.
    2. Creás un archivo de test, por ejemplo `test_mi_funcion.c`.
    3. Lo compilás con Unity y tu código.
    4. Corrés el binario de tests.


7. **¿Cómo se mide y mejora la cobertura de código en pruebas unitarias utilizando herramientas como gcov y lcov junto con Unity?**

    Para medir la cobertura de código con Unity y herramientas como gcov y lcov, seguís estos pasos:

    1. **Compilación con flags de cobertura:**
       ```bash
       set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
       ```

    2. **Ejecución de test:**
       Ejecutás tus pruebas unitarias con Unity.
       ```bash
       ctest ./test_mi_funcion
       ```

    3. **Generación de informes con lcov:**
       Usás `lcov` para generar un informe de cobertura.
       ```bash
       lcov --directory . --capture --output-file coverage.info
       ```

    4. **Uso de lcov para limpir rutas innecesarias:**
        Podés limpiar el informe de cobertura para eliminar rutas no deseadas, como las de librerías del sistema o archivos de prueba.
        ```bash
        lcov --remove coverage.info '/usr/*' '*/tests/*' --output-file cleaned.info
        ```
       
    5. **Revisar el informe HTML:**
       Abrís el archivo HTML generado en un navegador para ver la cobertura del código.
       ```bash
       firefox coverage/index.html
       ```

    Con estos pasos, podés identificar qué partes del código están cubiertas por las pruebas y cuáles necesitan más atención.