# Referências

## Visão geral / arquitetura
- [Handmade Hero — Episode Guide](https://guide.handmadehero.org/) — Série do Casey Muratori construindo um jogo/engine do zero em C. O arco da camada de plataforma (Win32) são os ~25 primeiros episódios; o guia tem timestamps por tópico.
- [Handmade Network](https://handmade.network/) — Comunidade de programação "handmade"; fórum bom para achar ports Linux e tirar dúvidas.
- [Game Engine Architecture (Jason Gregory)](https://www.gameenginebook.com/) — Livro de referência sobre sistemas de engine. Para consulta, não leitura linear.
- [Game Programming Patterns (Robert Nystrom)](https://gameprogrammingpatterns.com/) — Padrões de game loop, update, componentes etc. Gratuito online.
- [Semantic Compression / Compression-oriented programming (Casey Muratori)](https://caseymuratori.com/blog_0015) — Escreva o código concreto primeiro, extraia abstrações quando o padrão se repetir.

## Memória, performance, sistemas
- [Untangling Lifetimes: The Arena Allocator (Ryan Fleury)](https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator) — Por que e como usar arenas para gerenciar memória em C.
- [Data-Oriented Design and C++ (Mike Acton, CppCon 2014)](https://www.youtube.com/watch?v=rX0ItVEVjHc) — Palestra sobre pensar em dados, cache e o hardware real.
- [Computer Systems: A Programmer's Perspective](https://csapp.cs.cmu.edu/) — Memória, cache, linking, assembly. Base de sistemas.
- 
- [GCC — Options for Linking](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html) — Documentação oficial do `-l`, incluindo a observação de que a posição dele na linha de comando importa.
- [Computer Enhance / Performance-Aware Programming (Casey Muratori)](https://www.computerenhance.com/) — Curso sobre performance real: CPU, cache, SIMD, profiling.
- [RAD Debugger (código-fonte)](https://github.com/EpicGamesExt/raddebugger) — Codebase C moderno com camadas base/OS bem desenhadas. Referência de design, não tutorial.
- [stb (Sean Barrett)](https://github.com/nothings/stb) — Bibliotecas single-header em C. Ótimo para ler (ex.: stb_image, stb_truetype).

## Game loop / tempo
- [Fix Your Timestep! (Glenn Fiedler)](https://gafferongames.com/post/fix_your_timestep/) — Timestep fixo vs variável, acumulador, interpolação.

## Plataforma Linux (X11)
- [Xlib Programming Manual (Christophe Tronche)](https://tronche.com/gui/x/xlib/) — Especificação do Xlib em HTML navegável. Também: `sudo apt install libx11-doc` para ter as man pages (`man XCreateWindow`).
- [ICCCM (Christophe Tronche)](https://tronche.com/gui/x/icccm/) — Convenções entre clientes X e window manager; explica o protocolo WM_DELETE_WINDOW (fechar janela).
- [GLFW — src/x11_window.c](https://github.com/glfw/glfw/blob/master/src/x11_window.c) — Implementação de produção, pequena e legível, de janela + input em X11. Primeira consulta quando travar.
- [SDL — src/video/x11/](https://github.com/libsdl-org/SDL/tree/main/src/video/x11) — Backend X11 do SDL; mais completo, bom para casos de borda (MIT-SHM, teclado).
- `xev` (pacote x11-utils, já instalado) — Abre uma janela e imprime cada evento X11 recebido. Útil para entender o event loop antes de escrevê-lo.

## Plataforma Windows (Win32)
- [MinGW-w64](https://www.mingw-w64.org/) — Toolchain GCC que gera executáveis Windows; o pacote `mingw-w64` do apt permite cross-compilar do WSL.
- [Documentação Win32 (Microsoft Learn)](https://learn.microsoft.com/en-us/windows/win32/) — Referência oficial da API Win32 (janelas, mensagens, memória, timers).

## Renderização em software
### 2D
- [The Beauty of Bresenham's Algorithm (Alois Zingl)](http://members.chello.at/~easyfilter/bresenham.html) — Linhas, círculos, elipses e curvas com Bresenham, com derivação e código.
- [What every coder should know about gamma (John Novak)](https://blog.johnnovak.net/2016/09/21/what-every-coder-should-know-about-gamma/) — Por que blending e gradientes ficam errados sem considerar sRGB/gamma.
- Porter & Duff, "Compositing Digital Images" (SIGGRAPH 1984) — Artigo que define alpha compositing e alpha pré-multiplicado.

### Sombras
- [2D Visibility - Raycasting Shadow](https://redblobgames.com/articles/visibility/)
- [How to create 2D visibility/shadow effects for your game](https://ncase.me/sight-and-light/)

### 3D / rasterização
- [Computer Graphics from Scratch (Gabriel Gambetta)](https://gabrielgambetta.com/computer-graphics-from-scratch/) — Livro gratuito: raytracer e rasterizador do zero, só com "put_pixel". Melhor ponto de partida.
- [tinyrenderer (Dmitry Sokolov)](https://github.com/ssloy/tinyrenderer/wiki) — Rasterizador 3D em ~500 linhas, passo a passo: linhas, triângulos, z-buffer, câmera, shading.
- [Scratchapixel — Rasterization: a Practical Implementation](https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/overview-rasterization-algorithm.html) — Edge functions, coordenadas baricêntricas, perspectiva, z-buffer, com rigor matemático.
- [3D Math Primer for Graphics and Game Development](https://gamemath.com/) — Vetores, matrizes, transformações, quaternions. Gratuito online.
- [Pikuma — 3D Computer Graphics Programming](https://pikuma.com/courses/learn-3d-computer-graphics-programming) — Curso (pago) construindo um renderizador 3D em software em C.

### Rasterização rigorosa e otimização
- [Triangle rasterization in practice (Fabian Giesen)](https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/) — Edge functions, regra top-left, precisão de ponto fixo. Parte de uma série que continua com otimização (SIMD, incremental).
- [A trip through the Graphics Pipeline 2011 (Fabian Giesen)](https://fgiesen.wordpress.com/2011/07/09/a-trip-through-the-graphics-pipeline-2011-index/) — Como a GPU realmente funciona por dentro; ótimo para desenhar sua própria "API gráfica".
- Juan Pineda, "A Parallel Algorithm for Polygon Rasterization" (SIGGRAPH 1988) — Artigo original das edge functions, base de rasterizadores modernos.
- [Chris Hecker — Perspective Texture Mapping](https://www.chrishecker.com/Miscellaneous_Technical_Articles) — Série clássica sobre texturização com correção de perspectiva e regras de preenchimento.
- [Graphics Programming Black Book (Michael Abrash)](https://github.com/jagregory/abrash-black-book) — Clássico sobre otimização de renderização em software (inclui o renderizador do Quake). Hardware antigo, mentalidade atemporal.
- [Real-Time Rendering](https://www.realtimerendering.com/) — Referência enciclopédica de rendering em tempo real. Para depois.

## Livro-texto
- Foley, van Dam, Feiner, Hughes — *Computer Graphics: Principles and Practice, 2nd Edition in C* (Addison-Wesley, 1995) — Livro principal de estudo do projeto. Núcleo para a engine: caps. 3 (primitivas 2D, clipping, antialiasing), 5 (transformações), 6 (viewing 3D), 13 (cor), 15 (visibilidade/z-buffer), 16 (iluminação e shading), 17 (compositing, filtragem de imagem).

## Formatos de imagem / saída
- [Netpbm — especificação do formato PPM](https://netpbm.sourceforge.net/doc/ppm.html) — Spec oficial do PPM (P3 ASCII e P6 binário): cabeçalho, maxval, ordem das linhas e dos canais.
- 
- [Ray Tracing in One Weekend (Peter Shirley)](https://raytracing.github.io/books/RayTracingInOneWeekend.html) — Livro curto e gratuito que começa exatamente escrevendo um PPM; bom complemento (ray tracing em vez de rasterização).
- [The byte order fallacy (Rob Pike)](https://commandcenter.blogspot.com/2012/04/byte-order-fallacy.html) — Por que ler/gravar formatos binários com shifts e máscaras (`(b[0]<<8)|b[1]`) em vez de `#ifdef` de endianness ou cast de ponteiro: o código fica correto em qualquer máquina.

## GPU / transição software → GPU
- [Rasterization Rules — Direct3D 11 (Microsoft Learn)](https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-rasterizer-stage-rules) — Como GPUs modernas decidem quais pixels um triângulo cobre (centro do pixel em .5, regra top-left). Útil para escolher convenções no renderer em software que batam com a GPU.
