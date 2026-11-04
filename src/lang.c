#include "lang.h"

#include <stdlib.h>
#include <string.h>

#include "epg.h"

/* Spanish (Spain), keyed by the English in the code. Kept in alphabetical
 * order of the English; scripts/check-lang.py checks it's complete. Text on
 * the TV must fit the same space as the English: toasts 18 characters, the
 * banner's NOW/NEXT 5, X menu values about 15, the welcome screen's right
 * column 39. */
static const struct { const char *en, *es; } ES[] = {
  { "  (retry %d)", "  (intento %d)" },
  { "%s  -  up to %d min more", "%s  -  hasta %d min más" },
  { "%s AVAILABLE: X MENU", "%s DISPONIBLE: MENÚ X" },
  { "-%s BEHIND LIVE", "DIFERIDO -%s" },
  { "1 kHz  LEFT", "1 kHz  IZQUIERDA" },
  { "1 kHz  LEFT + RIGHT", "1 kHz  IZQ + DCHA" },
  { "1 kHz  RIGHT", "1 kHz  DERECHA" },
  { "1. Put your playlist link on the first line of:", "1. Pon el enlace de tu lista en la primera línea de:" },
  { "10%", "10%" },
  { "12-hour", "12 horas" },
  { "2 seconds", "2 segundos" },
  { "2. For example, from Dispatcharr:", "2. Por ejemplo, de Dispatcharr:" },
  { "24-hour", "24 horas" },
  { "3. Close and reopen RetroTV.", "3. Cierra RetroTV y vuelve a abrirlo." },
  { "4 seconds", "4 segundos" },
  { "5%", "5%" },
  { "50 Hz", "50 Hz" },
  { "60 Hz", "60 Hz" },
  { "640x240 (240p)", "640x240 (240p)" },
  { "640x240 (240p) is progressive like old consoles on a CRT: no interlace flicker, half the vertical detail.",
    "640x240 (240p) es progresivo, como las consolas antiguas en un CRT: sin parpadeo de entrelazado y con la mitad de detalle vertical." },
  { "640x480", "640x480" },
  { "8 seconds", "8 segundos" },
  { "A INFO  B LAST  Y ZOOM  X MENU  START GUIDE", "A INFO  B ANTERIOR  Y ZOOM  X MENÚ  START GUÍA" },
  { "A short click when you change channel, like an old TV.", "Un clic corto al cambiar de canal, como en una tele antigua." },
  { "A: TEST CARD - check picture, sound and stereo", "A: CARTA DE AJUSTE - prueba imagen, sonido y estéreo" },
  { "Access denied by the server", "El servidor deniega el acceso" },
  { "Add channels to it (in Dispatcharr: add channels to the channel profile in the link).",
    "Añádele canales (en Dispatcharr: al perfil de canales del enlace)." },
  { "Allow this Pi's address for M3U/EPG in the server's network access settings, or add credentials.",
    "Permite la dirección de esta Pi para M3U/EPG en los ajustes de acceso de red del servidor, o añade credenciales." },
  { "Asks GitHub once a day whether a newer RetroTV is out, and says so on the banner and in the X menu. Installing is always your choice: X menu > Update.",
    "Pregunta a GitHub una vez al día si hay una versión nueva de RetroTV y lo avisa en el banner y en el menú X. Instalarla siempre lo decides tú: menú X > Actualizar." },
  { "Audio", "Audio" },
  { "Auto (follow the channel)", "Auto (según el canal)" },
  { "Auto switches to 50 Hz for 25/50 fps channels so motion is smooth. Lock it if your screen dislikes mode changes.",
    "Auto cambia a 50 Hz en los canales de 25/50 fps para que el movimiento sea fluido. Fíjalo si tu pantalla lleva mal los cambios de modo." },
  { "BUFFER FULL", "MEMORIA LLENA" },
  { "Back to live", "Volver al directo" },
  { "Banner timeout", "Duración del banner" },
  { "Banner, clock and on-screen hints.", "Banner, reloj y ayudas en pantalla." },
  { "Button hints", "Ayuda de botones" },
  { "CHANNEL LIST", "CANALES" },
  { "Cable Navy", "Azul cable" },
  { "Change channel from Zaparoo, a script or a phone.", "Cambiar de canal desde Zaparoo, un script o un móvil." },
  { "Channel-change sound", "Sonido al cambiar de canal" },
  { "Check for updates", "Buscar actualizaciones" },
  { "Check now", "Comprobar" },
  { "Check the address in the link and that the Pi and the server are on the same network.",
    "Revisa la dirección del enlace y que la Pi y el servidor estén en la misma red." },
  { "Check the link in retrotv/playlist.txt.", "Revisa el enlace en retrotv/playlist.txt." },
  { "Checking...", "Buscando..." },
  { "Classic snow", "Nieve clásica" },
  { "Clock", "Reloj" },
  { "Clock on the banner and in the guide.", "El reloj del banner y de la guía." },
  { "Close", "Cerrar" },
  { "Colour theme", "Colores" },
  { "Colours of the banner, channel list and guide.", "Colores del banner, la lista de canales y la guía." },
  { "Couldn't load the playlist", "No se pudo cargar la lista" },
  { "Create retrotv/playlist.txt on your ReplayOS storage with your M3U link on the first line.",
    "Crea retrotv/playlist.txt en el almacenamiento de ReplayOS con tu enlace M3U en la primera línea." },
  { "Done: reopen", "Hecho: reabre" },
  { "Downloading...", "Descargando..." },
  { "Evens out loud and quiet channels. Loud aims higher, closer to game volume.",
    "Iguala los canales altos y bajos. Alto apunta más arriba, cerca del volumen de los juegos." },
  { "FAVOURITES", "FAVORITOS" },
  { "FIT", "AJUSTAR" },
  { "Favourites", "Favoritos" },
  { "Failed: retry", "Error: repetir" },
  { "Fit", "Ajustar" },
  { "Fit shows the whole picture with black bars; Zoom fills the 4:3 screen. Y switches while watching.",
    "Ajustar muestra la imagen entera con bandas negras; Zoom llena la pantalla 4:3. Y cambia mientras ves la tele." },
  { "Green Phosphor", "Fósforo verde" },
  { "Home network", "Red de casa" },
  { "How long the channel banner stays up after a channel change. Off: only when you press A.",
    "Cuánto tiempo se ve el banner del canal tras cambiar de canal. No: solo al pulsar A." },
  { "II PAUSED", "II PAUSA" },
  { "Install %s", "Instalar %s" },
  { "Installing...", "Instalando..." },
  { "Interface", "Interfaz" },
  { "Is the server running? Check the address and port in the link (Dispatcharr uses 9191).",
    "¿Está en marcha el servidor? Revisa la dirección y el puerto del enlace (Dispatcharr usa el 9191)." },
  { "It returned something else (a web page?). Use the M3U link, e.g. Dispatcharr's /output/m3u/...",
    "Devolvió otra cosa (¿una página web?). Usa el enlace M3U, p. ej. /output/m3u/... de Dispatcharr." },
  { "Keep each channel's Fit/Zoom choice from the Y button.", "Guarda en cada canal la elección Ajustar/Zoom del botón Y." },
  { "LEFT/RIGHT", "IZQ/DCHA" },
  { "LEFT/RIGHT PAGE", "IZQ/DCHA PÁGINA" },
  { "LEFT/RIGHT CHANGE  A SELECT  B CLOSE", "IZQ/DCHA CAMBIAR  A ELEGIR  B CERRAR" },
  { "LEVELLER LOUD", "NIVELADOR ALTO" },
  { "LEVELLER NORMAL", "NIVELADOR NORMAL" },
  { "LEVELLER OFF", "NIVELADOR NO" },
  { "LIVE", "DIRECTO" },
  { "Leveller", "Nivelador" },
  { "Live", "Directo" },
  { "Loud", "Alto" },
  { "MENU", "MENÚ" },
  { "NEW VERSION", "NUEVA VERSIÓN" },
  { "NEXT", "LUEGO" },
  { "NO CHANNEL", "NO HAY CANAL" },
  { "NO CHANNEL %s", "NO HAY CANAL %s" },
  { "NO FAVOURITES YET", "AÚN NO HAY FAVORITOS" },
  { "NO SIGNAL", "SIN SEÑAL" },
  { "NO SIGNAL - %s", "SIN SEÑAL - %s" },
  { "NOW", "AHORA" },
  { "New versions of RetroTV.", "Versiones nuevas de RetroTV." },
  { "No information", "Sin información" },
  { "No playlist yet", "Aún no hay lista" },
  { "None", "Ninguno" },
  { "Normal", "Normal" },
  { "Off", "No" },
  { "On", "Sí" },
  { "Output", "Salida" },
  { "Overscan margin", "Margen de overscan" },
  { "PLAY -%s", "PLAY -%s" },
  { "PRESS X FOR ALL CHANNELS, THEN Y TO STAR ONE", "PULSA X PARA VER TODOS Y LUEGO Y PARA MARCAR UNO" },
  { "Pause", "Pausa" },
  { "Paused %s", "En pausa %s" },
  { "Paused picture", "Imagen en pausa" },
  { "Picture", "Imagen" },
  { "Playing -%s", "Diferido -%s" },
  { "Playlist file not found", "No está el archivo de la lista" },
  { "Playlist not found (404)", "Lista no encontrada (404)" },
  { "Playlist problem", "Problema con la lista" },
  { "Press A to start watching", "Pulsa A para empezar a ver" },
  { "Put your M3U link on the first line that doesn't start with #.", "Pon tu enlace M3U en la primera línea que no empiece por #." },
  { "Quiet", "Bajo" },
  { "RESTART RETROTV", "REINICIA RETROTV" },
  { "RETROTV - SET UP YOUR CHANNELS", "RETROTV - CONFIGURA TUS CANALES" },
  { "Refresh rate", "Frecuencia" },
  { "Remember zoom per channel", "Recordar el zoom de cada canal" },
  { "Remote control", "Mando a distancia" },
  { "Restart ReplayOS", "Falta reiniciar" },
  { "SCREEN %d", "PANTALLA %d" },
  { "SCREEN 2", "PANTALLA 2" },
  { "SCREEN 2 OFF", "PANTALLA 2 APAGADA" },
  { "SLEEP %d", "APAGAR EN %d" },
  { "SLEEP 1", "APAGAR EN 1" },
  { "SLEEP OFF", "SIN APAGADO" },
  { "STANDBY", "EN ESPERA" },
  { "SWAPPED", "CAMBIADAS" },
  { "Screen 2", "Pantalla 2" },
  { "Server didn't answer", "El servidor no responde" },
  { "Server refused the connection", "El servidor rechaza la conexión" },
  { "Settings: ReplayOS menu > System Settings", "Ajustes: menú de ReplayOS > System Settings" },
  { "Show controls", "Ver controles" },
  { "Show the button hints line on the banner.", "Muestra en el banner la línea de ayuda de los botones." },
  { "Shows the channel you're watching in the Zaparoo app (as a Video item). Only talks to Zaparoo on this device; does nothing without it.",
    "Muestra en la app de Zaparoo el canal que estás viendo (como un vídeo). Solo habla con Zaparoo en este aparato; sin él no hace nada." },
  { "Shrink the video so its edges stay visible on screens that cut them off. Menus are always inside the safe area.",
    "Reduce la imagen para que se vean los bordes en pantallas que los recortan. Los menús siempre quedan dentro de la zona segura." },
  { "Sleep", "Apagado" },
  { "Star channels with Y in the channel list and the guide; X then shows only them, and channel up/down goes through them. Stars on the phone remote too. Off: every channel, no stars.",
    "Marca canales con Y en la lista de canales y en la guía; X muestra solo esos, y subir y bajar de canal pasa por ellos. También estrellas en el mando del móvil. No: todos los canales, sin estrellas." },
  { "Sound", "Sonido" },
  { "Still picture", "Imagen fija" },
  { "Swap screens", "Intercambiar" },
  { "TUNING", "SINTONIZANDO" },
  { "TV guide", "guía de TV" },
  { "TV guide (Y: star, X: favourites)", "guía de TV (Y: favorito, X: favoritos)" },
  { "Teletext", "Teletexto" },
  { "Tell Zaparoo what's on", "Decir a Zaparoo qué se ve" },
  { "Test card", "Carta de ajuste" },
  { "That isn't a link or a path", "Eso no es un enlace ni una ruta" },
  { "That link isn't a playlist", "Ese enlace no es una lista" },
  { "The first line must start with http://, https:// or / (a file on the Pi).",
    "La primera línea debe empezar por http://, https:// o / (un archivo en la Pi)." },
  { "The name in the link can't be found: try the server's IP address instead.",
    "No se encuentra el nombre del enlace: prueba con la dirección IP del servidor." },
  { "The path in playlist.txt doesn't exist on the Pi. Use a full path or a link.",
    "La ruta de playlist.txt no existe en la Pi. Usa una ruta completa o un enlace." },
  { "The playlist has no channels", "La lista no tiene canales" },
  { "The server answered but the path is wrong: check the channel profile name in the link.",
    "El servidor responde, pero la ruta está mal: revisa el nombre del perfil de canales en el enlace." },
  { "This device only", "Solo este aparato" },
  { "Tune by web request on port 7474, e.g. /tune?ch=36. This device only: Zaparoo and scripts on the Pi. Home network: also phones and computers on your network, with no password.",
    "Cambia de canal por petición web en el puerto 7474, p. ej. /tune?ch=36. Solo este aparato: Zaparoo y scripts en la Pi. Red de casa: también móviles y ordenadores de tu red, sin contraseña." },
  { "Tuning picture", "Imagen al sintonizar" },
  { "Tuning static sound", "Ruido al sintonizar" },
  { "Two CRTs: a 1280x480 picture (1280x240 with Output 240p) for ReplayOS Multi screen = Dual horizontal (set it for RetroTV only). Connect both screens before starting. This and Output apply when RetroTV restarts. Keeps 60 Hz. Turn screen 2 on and off in the X menu.",
    "Dos CRT: una imagen de 1280x480 (1280x240 con Salida 240p) para Multi screen = Dual horizontal de ReplayOS (solo para RetroTV). Conecta las dos pantallas antes de empezar. Esto y Salida se aplican al reiniciar RetroTV. Siempre a 60 Hz. La pantalla 2 se enciende y apaga en el menú X." },
  { "Two screens (dual mode)", "Dos pantallas (modo dual)" },
  { "UP / DOWN", "ARR/ABAJO" },
  { "UP/DN CH  LEFT/RIGHT TIME  A WATCH  B CLOSE", "ARR/AB CANAL  IZQ/DCHA HORA  A VER  B CERRAR" },
  { "UP/DN CH  LEFT/RIGHT TIME  A WATCH  Y STAR  X FAVOURITES  B CLOSE", "ARR/AB CANAL  IZQ/DCHA HORA  A VER  Y FAVORITO  X FAVORITOS  B CERRAR" },
  { "UP/DN CH  LEFT/RIGHT TIME  A WATCH  Y UNSTAR  X ALL", "ARR/AB CANAL  IZQ/DCHA HORA  A VER  Y QUITAR  X TODOS" },
  { "UPDATE FAILED", "NO SE ACTUALIZÓ" },
  { "UPDATED", "ACTUALIZADO" },
  { "UPDATED: REOPEN RETROTV", "ACTUALIZADO: REABRE RETROTV" },
  { "UPDATED: RESTART REPLAYOS", "ACTUALIZADO: REINICIA REPLAYOS" },
  { "Unknown server name", "Nombre de servidor desconocido" },
  { "Up to date", "Al día" },
  { "Update", "Actualizar" },
  { "Updates", "Actualizaciones" },
  { "VHS", "VHS" },
  { "VHS tape", "Cinta VHS" },
  { "Video size, zoom and refresh rate.", "Tamaño de imagen, zoom y frecuencia." },
  { "Volume leveller", "Nivelador de volumen" },
  { "Volume leveller and tuning sounds.", "Nivelador de volumen y sonidos al sintonizar." },
  { "Volume of the hiss while a channel tunes.", "Volumen del ruido mientras se sintoniza un canal." },
  { "WELCOME TO RETROTV", "BIENVENIDO A RETROTV" },
  { "What a paused channel looks like: a VCR's shaky still frame with a noise bar, or a clean still.",
    "Cómo se ve un canal en pausa: la imagen temblorosa de un vídeo con una barra de ruido, o una imagen fija limpia." },
  { "What you see while a channel tunes.", "Lo que se ve mientras se sintoniza un canal." },
  { "X ALL CHANNELS", "X TODOS LOS CANALES" },
  { "X ALL CHANNELS  B CLOSE", "X TODOS LOS CANALES  B CERRAR" },
  { "X MENU > SCREEN 2 TO TURN ON", "MENÚ X > PANTALLA 2 PARA ENCENDER" },
  { "Y STAR  X FAVOURITES  L/R PAGE", "Y FAVORITO  X SOLO FAVORITOS  L/R PÁGINA" },
  { "Y UNSTAR  X ALL  L/R PAGE", "Y QUITAR  X TODOS  L/R PÁGINA" },
  { "ZOOM", "ZOOM" },
  { "Zoom", "Zoom" },
  { "change channel", "cambiar de canal" },
  { "channel list (left/right: a page)", "lista de canales (izq/dcha: página)" },
  { "channel list (Y: star, X: favourites)", "canales (Y: favorito, X: favoritos)" },
  { "last channel", "canal anterior" },
  { "menu: sleep, audio, leveller, test card", "menú: apagado, audio, nivelador, carta" },
  { "one track", "una pista" },
  { "pause, play / back to live", "pausa, seguir / volver al directo" },
  { "playlist.txt is empty", "playlist.txt está vacío" },
  { "screen 1 / screen 2 (sound follows)", "pantalla 1 / pantalla 2 (con su sonido)" },
  { "what's on (banner)", "qué hay ahora (banner)" },
  { "zoom / fit picture", "zoom / ajustar la imagen" },
};
#define NES (sizeof(ES) / sizeof(ES[0]))

static const char *const DAYS[LANG_COUNT][7] = {
  { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" },
  { "DOM", "LUN", "MAR", "MI\xc9", "JUE", "VIE", "S\xc1" "B" },      /* Latin-1: MIÉ, SÁB */
};
static const char *const MONTHS[LANG_COUNT][12] = {
  { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" },
  { "ENE", "FEB", "MAR", "ABR", "MAY", "JUN", "JUL", "AGO", "SEP", "OCT", "NOV", "DIC" },
};

static int cur;
static size_t order[NES];          /* ES sorted by strcmp (the table is sorted by eye, not by byte) */
static int sorted;
static char *latin1[NES];          /* Latin-1 copies of ES[].es, made when first shown */
static char *ascii[NES];           /* ASCII copies for the frontend's menus */

static int cmp_order(const void *a, const void *b) { return strcmp(ES[*(const size_t *)a].en, ES[*(const size_t *)b].en); }

static long find(const char *en)
{
  if (!sorted) {
    for (size_t i = 0; i < NES; i++) order[i] = i;
    qsort(order, NES, sizeof(order[0]), cmp_order);
    sorted = 1;
  }
  size_t lo = 0, hi = NES;
  while (lo < hi) {
    size_t mid = (lo + hi) / 2;
    int c = strcmp(en, ES[order[mid]].en);
    if (!c) return (long)order[mid];
    if (c < 0) hi = mid; else lo = mid + 1;
  }
  return -1;
}

void lang_set(int lang) { cur = lang >= 0 && lang < LANG_COUNT ? lang : LANG_EN; }
int lang_get(void) { return cur; }
const char *lang_code(int lang) { return lang == LANG_ES ? "es" : "en"; }
int lang_parse(const char *code) { return code && !strcmp(code, "es") ? LANG_ES : LANG_EN; }

const char *TU(const char *en)
{
  if (cur == LANG_EN || !en) return en;
  long i = find(en);
  if (i < 0) return en;
  if (!ascii[i]) {
    size_t n = strlen(ES[i].es) + 1;
    char *t = malloc(n);
    if (!t) return en;
    epg_ascii(ES[i].es, t, n);
    ascii[i] = t;
  }
  return ascii[i];
}

const char *T(const char *en)
{
  if (cur == LANG_EN || !en) return en;
  long i = find(en);
  if (i < 0) return en;
  if (!latin1[i]) {
    size_t n = strlen(ES[i].es) + 1;               /* Latin-1 is never longer than UTF-8 */
    char *t = malloc(n);
    if (!t) return en;
    epg_latin1(ES[i].es, t, n);
    latin1[i] = t;
  }
  return latin1[i];
}

const char *lang_day(int wday) { return DAYS[cur][wday >= 0 && wday < 7 ? wday : 0]; }
const char *lang_month(int mon) { return MONTHS[cur][mon >= 0 && mon < 12 ? mon : 0]; }
