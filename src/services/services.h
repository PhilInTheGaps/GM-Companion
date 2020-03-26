#ifndef SERVICES_H
#define SERVICES_H

/* --- GENERAL --- */
#define DEFAULT_SERVER_URL "https://gm-companion.rophil.lol"

/* --- SPOTIFY --- */
#define LIBRESPOT_DEVICE_NAME "GM-Companion"

#define SPOTIFY_MAX_REQUESTS 2
#define SPOTIFY_BATCH_SIZE 20

#define SPOTIFY_LOCAL_PORT 59991
#define SPOTIFY_SERVER_PORT 59992

#define SPOTIFY_SCOPE "user-library-read playlist-read-private streaming user-modify-playback-state user-read-currently-playing user-read-playback-state"

/* --- GOOGLE --- */
#define GOOGLE_LOCAL_PORT 59993

#endif // SERVICES_H
