# RetroTV en español

**Haz zapping como en 1993.** RetroTV es un núcleo libretro que convierte una lista IPTV
en un descodificador de cable de los 90 para **ReplayOS en la Raspberry Pi 5**: subir y
bajar de canal, un banner con lo que echan ahora y después, una guía de TV, nieve entre
canales y la pantalla azul de SIN SEÑAL. Está pensado para televisores CRT, con un modo
240p de verdad, y usa el FFmpeg del propio ReplayOS, así que no hay que instalar nada más.

[Vídeo de demostración](https://youtu.be/OCvExU21ayg) · [README completo (inglés)](../README.md)

## Requisitos

- Raspberry Pi 5 con **ReplayOS 2.1.0 o posterior** y conexión a internet.
- Una lista de canales M3U: Dispatcharr, Tunarr, cualquier lista M3U con flujos MPEG-TS o
  HLS, o los canales gratuitos de Pluto TV ([guía de Pluto TV, en inglés](PLUTO_TV.md)).

## Instalación

**Opción 1: una línea por SSH** (la contraseña de root de ReplayOS es `replayos` si no la has cambiado):

```bash
ssh root@replay.local
```
```bash
curl -fsSL https://raw.githubusercontent.com/chris-skitch/retrotv-libretro/main/install.sh | bash
```

**Opción 2: desde la tele, sin SSH.** Copia
[`replayos/retrotv_update.sh`](../replayos/retrotv_update.sh) en `roms/_extra/` de tu
almacenamiento de ReplayOS (la tarjeta SD, un USB o una carpeta de red) y ejecuta
**Extra → retrotv_update.sh**.

El instalador descarga la última versión, comprueba sus sumas SHA-256, registra RetroTV
como un sistema de ReplayOS (**RetroTV** en la lista de sistemas) y reinicia ReplayOS.

## Tus canales: `playlist.txt`

1. Abre `retrotv/playlist.txt` en tu almacenamiento (por ejemplo `/media/sd/retrotv/playlist.txt`)
   y pon el enlace de tu lista en una línea que no empiece por `#`:

   ```
   # Dispatcharr (un perfil de canales)
   http://<dispatcharr>:9191/output/m3u/<perfil>

   # o Tunarr
   http://<tunarr>:8088/api/channels.m3u
   ```

2. En la tele, abre **RetroTV** en la lista de sistemas (una carpeta naranja, como todos
   los sistemas externos) y luego **RetroTV** dentro de ella.

**Varias listas.** Cada archivo `.txt` o `.m3u` de `roms/retrotv/` aparece como una lista
que puedes elegir, y cada una recuerda su último canal. Un `.txt` se escribe como
`playlist.txt`: un enlace y, si quieres, una línea `epg=`.

**Guía de TV.** RetroTV usa la guía que indica la lista (`x-tvg-url`). Para usar otra,
añade una línea `epg=` con un enlace o una ruta XMLTV (`.xml` o `.xml.gz`):

```
epg=http://ejemplo.org/guia.xml.gz
```

Si algo falla, RetroTV muestra una pantalla de configuración que explica qué pasa (servidor
no encontrado, 404, no es una lista...). Pulsa **A** ahí para ver la carta de ajuste, que
prueba la imagen, el sonido y el estéreo.

## Idioma

En el menú de ReplayOS durante el juego: **System Settings → Language / Idioma → Español**.
Las pantallas de RetroTV cambian al momento; los nombres del propio menú System Settings
cambian la próxima vez que abras RetroTV. El mando del móvil usa el idioma del teléfono.

## Controles

Los nombres de los botones son los de un mando tipo Xbox.

| Botón | Qué hace |
|---|---|
| Arriba / Abajo (o R / L) | Subir / bajar de canal (solo favoritos si la vista de favoritos está activa) |
| A | Qué echan ahora (banner) |
| B | Canal anterior |
| Y | Zoom: ajustar la imagen con bandas negras o llenar la pantalla; se recuerda por canal |
| X | Menú: pausa / directo, apagado automático, pista de audio, nivelador, imagen, actualizar, carta de ajuste |
| Start | Guía de TV: Arriba/Abajo canal, Izquierda/Derecha media hora, A para verlo, B para cerrar |
| Select | Lista de canales: Arriba/Abajo un canal, Izquierda/Derecha una página, A para verlo, B para cerrar |
| R2 / L2 | Pausa (hasta unos 15 minutos) y seguir / volver al directo |

**Favoritos.** En la lista de canales o en la guía, **Y** marca un canal como favorito
(o lo quita) y **X** muestra solo los favoritos. Mientras esa vista está activa, subir y
bajar de canal pasa solo por los favoritos. RetroTV la recuerda para cada lista.

## Mando desde el móvil

Activa **System Settings → Mando a distancia → Red de casa** y abre `http://<tu-pi>:7474/`
en el navegador del móvil. Verás el canal actual, botones de canal, pausa y directo, todos
los canales con lo que echan ahora y después, un buscador, estrellas para los favoritos y
los ajustes rápidos (⚙).

## Actualizar y desinstalar

RetroTV mira una vez al día si hay una versión nueva. Cuando la hay, lo dice el banner y
**X → Actualizar → Instalar** la instala sin reiniciar ReplayOS. También puedes usar
**Extra → retrotv_update.sh**. Para desinstalarlo: **Extra → retrotv_uninstall.sh**.
Las actualizaciones de ReplayOS no borran RetroTV.

## Problemas

- **RetroTV no sale en la lista de sistemas:** hace falta ReplayOS 2.1.0 o posterior y
  tener activado **Visibility → Show external systems**.
- **Sale la pantalla de configuración en vez de los canales:** explica qué falla. Prueba
  el enlace de `playlist.txt` desde otro ordenador.
- **No hay guía:** comprueba que la lista tenga `x-tvg-url`, o añade `epg=`.
- **Un canal se queda en SINTONIZANDO:** los canales IPTV en frío pueden tardar 5–10 s;
  RetroTV lo intenta 3 veces antes de mostrar SIN SEÑAL.
- **Un canal se congela unos segundos y sigue:** el flujo se cortó y RetroTV se volvió a
  conectar. Las listas no oficiales de Pluto TV se cortan más; mira un solo canal de Pluto
  a la vez.
- **Registro:** `/tmp/retrotv.log` en la Pi (`ssh root@replay.local`).

¿Ves algo mal traducido? Abre una incidencia o una pull request en
[GitHub](https://github.com/chris-skitch/retrotv-libretro): las traducciones están en
`src/lang.c` (la tele) y `src/webui.html` (el móvil).
